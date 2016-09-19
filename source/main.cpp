#include <iostream>
#include "MEMC.h"
#include "dct.h"
#include "func.h"
#include "rlc.h"
using namespace std;
using namespace func;


const string raw_file = FILEPATH + FILENAME + FILEFORMAT;
const string dif_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + "_dif.y";
const string res_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + "_res.y";
const string rec_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + "_rec.y";
const string out_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + ".y";
const string dat_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + ".dat";
const string cyc_file = OUTPATH + FILENAME + "_W" + to_string(SRANGE/2) + ".cyc";


int main(void){

	cout << endl;

	unsigned char *i_image	= new unsigned char [FRAME_SIZE];
	unsigned char *i_imagec	= new unsigned char [FRAME_SIZE];
	unsigned char *o_image	= new unsigned char [FRAME_SIZE];
	unsigned char *buff		= new unsigned char [FRAME_SIZE];
	char *q_image			= new char [FRAME_SIZE];
	double *d_image 		= new double [FRAME_SIZE];
	double *id_image 		= new double [FRAME_SIZE];

	const int v_sum = (HEIGHT/BLOCK_SIZE)*(WIDTH/BLOCK_SIZE);
	char* ME_x = new char [v_sum];
	char* ME_y = new char [v_sum];
	char *temp = new char [1024];
	bool* signs = new bool[FRAME_SIZE];
	bool i_frame;
	clock_t begin, end;

	strcpy(temp, raw_file.c_str());
	FILE *readPtr = fopen( temp, "rb");
	fseek(readPtr, 0, SEEK_END);
	const int frame_num = ftell(readPtr)/FRAME_SIZE;
	fseek(readPtr, 0, SEEK_SET);

	double *psnrs = new double [frame_num];
	double *times = new double [frame_num];
	ofstream dif_ofile, res_ofile, rec_ofile, out_ofile, dat_ofile, cyc_ofile;
	dif_ofile.open(dif_file);
	res_ofile.open(res_file);
	rec_ofile.open(rec_file);
	out_ofile.open(out_file);
	dat_ofile.open(dat_file);
	cyc_ofile.open(cyc_file);

	cout << "Vide encoding ..." << endl << endl;
	for (int i=0; i<frame_num; ++i) {
		begin = clock();
		
		if(i % GOP_SIZE == 0)
			i_frame = false;
		else
			i_frame = false;
		
		fread(i_image, 1, FRAME_SIZE, readPtr);
		for(int j=0; j<FRAME_SIZE; ++j)
			i_imagec[j] = i_image[j];
		if(i % GOP_SIZE != 0){
			ME(i_image, buff, ME_x, ME_y);
			MC(o_image, buff, ME_x, ME_y);
			imgSub(i_image, o_image, signs);
		}
		
		fDCT(i_image, d_image, 1);
		fQuantization(d_image, q_image, i_frame);
		ifQuantization(q_image, id_image, i_frame);
		ifDCT(id_image, buff, 1);

		for(int j=0; j<FRAME_SIZE; ++j)
			res_ofile << buff[j];

		if(i % GOP_SIZE != 0)
			imgAdd(buff, o_image, signs);
		
		for(int j=0; j<FRAME_SIZE; ++j){
			dif_ofile << i_image[j];
			rec_ofile << o_image[j];
			out_ofile << buff[j];
		}

		end = clock();
		times[i] = end - begin;
		psnrs[i] = psnr(buff, i_imagec, FRAME_SIZE);
		
		cout << (char)8 << (char)8 << (char)8 << (char)8 << (char)8 << setw(3) << 100*i/frame_num << " %";
		cout.flush();

	}
	cout << (char)8 << (char)8 << (char)8 << (char)8 << (char)8 << setw(3) << 100 << " %" << endl << endl;
	cout.flush();
	fclose(readPtr);

	cout << endl;
	for(int i=0; i<frame_num; ++i){
		cout << "psnr of the " << setw(2) << i << " th frame: " << setw(6) << times[i] << "cycles | " << psnrs[i] << " dB" << endl;
		dat_ofile << psnrs[i] << "\n";
		cyc_ofile << times[i] << "\n";
	}
	cout << endl;

	delete [] ME_x;
	delete [] ME_y;
	delete [] d_image;
	delete [] q_image;
	delete [] i_image;

	return 0;
}
