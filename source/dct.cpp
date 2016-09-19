#include"dct.h"

//----------------------- DCT -----------------------//
void DCT(const unsigned char* i_image, double* o_image){

	for (int v = 0; v < HEIGHT; ++v){
		for (int u = 0; u < WIDTH; ++u){
			
			double sum = 0;
			
			for (int y = 0; y < HEIGHT; ++y){
				for (int x = 0; x < WIDTH; ++x){
					
					double val = i_image[y*HEIGHT+x];
					double para1 = ((2*x+1) * PI * u) / (2*HEIGHT);
					double para2 = ((2*y+1) * PI * v) / (2*HEIGHT);
					sum += val * cos(para1) * cos(para2);

				}
			}

			double c_u = (u==0)? 1/sqrt(2):1;
			double c_v = (v==0)? 1/sqrt(2):1;
			double o_val = (2 * c_v * c_u * sum)/HEIGHT;
			o_image[v*HEIGHT+u] = o_val;
		}
	}
}
//----------------------- DCT -----------------------//

//----------------------- iDCT ----------------------//
void iDCT(const double* i_image, unsigned char* o_image){

	for (int v = 0; v < HEIGHT; ++v){
		for (int u = 0; u < WIDTH; ++u){
			
			double sum = 0;
			
			for (int y = 0; y < HEIGHT; ++y){
				for (int x = 0; x < WIDTH; ++x){
					
					double val = i_image[y*HEIGHT+x];
					double para1 = ((2*u+1) * PI * x) / (2*HEIGHT);
					double para2 = ((2*v+1) * PI * y) / (2*HEIGHT);
					double c_x = (x==0)? 1/sqrt(2):1;
					double c_y = (y==0)? 1/sqrt(2):1;
			
					sum += c_x * c_y * val * cos(para1) * cos(para2);

				}
			}

			double o_val = (2 * sum)/HEIGHT;
			o_val = (o_val>255)? 255:(o_val<0)? 0:o_val;			
			o_image[v*HEIGHT+u] = (unsigned char)o_val;
		}
	}
}
//----------------------- iDCT ----------------------//

//------------------- iQuantization ------------------//
void iQuantization(const unsigned char* i_image, double* o_image, int* truncates){

	int dc_val1 = (int)i_image[0];
	int dc_val2 = (int)i_image[1];
	int dc_val = (dc_val1<<8) + dc_val2;
	dc_val = (dc_val - pow(2,DC_LIMIT-1));
	dc_val = dc_val<<truncates[0];
	o_image[0] = dc_val;
	
	for (int i = 2; i < HEIGHT*WIDTH+1; ++i){
		int ac_val = (int)i_image[i];
		//cout << ac_val << " , ";
		ac_val = (ac_val - pow(2,AC_LIMIT-1));
		//cout << ac_val << " , ";
		ac_val = (ac_val * pow(2,truncates[1]));
		//cout << ac_val << " , ";
		o_image[i-1] = (double)ac_val;
		//cout << o_image[i-1] << endl;
	}
}
//------------------- iQuantization ------------------//

//------------------- Quantization -------------------//
void Quantization(const double* i_image, unsigned char* o_image, int* truncates, bool downsampling){

	int DC_num = 0;
	int AC_num = 0;
	int truncated_DC = 0;
	int truncated_AC = 0;
	int * tmp_image = new int [WIDTH*HEIGHT];

	for (int i = 0; i < HEIGHT*WIDTH; ++i){
		double tmp_val = i_image[i];
		int val = floor(tmp_val);
		int bit_num = BitNum(val);
		if(i==0)
			DC_num = bit_num;
		else
			AC_num = (bit_num>AC_num)? bit_num:AC_num;
	}

	truncated_DC = (DC_num>DC_LIMIT-1)? DC_num-DC_LIMIT+1:0;
	truncated_AC = (AC_num>AC_LIMIT-1)? AC_num-AC_LIMIT+1:0;
	
	for (int i = 0; i < HEIGHT*WIDTH; ++i){
		int val = floor(i_image[i]);
			if(i==0)
				tmp_image[0] = (val>>truncated_DC);
			else{
				if(downsampling) val = (abs(val) < 8)? 0:val;
				tmp_image[i] = (abs(val)>>truncated_AC);
				tmp_image[i] = (val>0)? tmp_image[i]:(-1)*tmp_image[i];
			}
	}

	int tmp = tmp_image[0] + pow(2,DC_LIMIT-1);
	o_image[0] = (unsigned char) (tmp>>8 & 0x000000ff);
	o_image[1] = (unsigned char) (tmp    & 0x000000ff);

	for (int i = 2; i < HEIGHT*WIDTH+1; ++i){
		tmp = tmp_image[i-1] + pow(2,AC_LIMIT-1);
		o_image[i] = (unsigned char) (tmp & 0x000000ff);
	}
	truncates[0] = truncated_DC;
	truncates[1] = truncated_AC;
	
	delete [] tmp_image;

}
//------------------- Quantization -------------------//

//----------------------- fDCT -----------------------//
void fDCT(const unsigned char* i_image, double* o_image, bool blockwise){
	if(blockwise){
		for(int i=0; i<HEIGHT/BLOCK_SIZE; ++i){
			for(int j=0; j<WIDTH/BLOCK_SIZE; ++j){
				int pos = i*BLOCK_SIZE*WIDTH + j*BLOCK_SIZE;
				unsigned char *block1 = new unsigned char [BLOCK_SIZE*BLOCK_SIZE];
				double *block2 = new double [BLOCK_SIZE*BLOCK_SIZE];
				double *block3 = new double [BLOCK_SIZE*BLOCK_SIZE];
				for(int k=0; k<BLOCK_SIZE; ++k)
					for(int l=0; l<BLOCK_SIZE; ++l)
						block1[k*BLOCK_SIZE+l] = i_image[pos+k*WIDTH+l];
				for(int m=0; m<BLOCK_SIZE; ++m)
					OneDimDCT(block1+m*BLOCK_SIZE, block2+m*BLOCK_SIZE, BLOCK_SIZE);
				transpose(block2, BLOCK_SIZE, BLOCK_SIZE);
				for(int m=0; m<BLOCK_SIZE; ++m)
					OneDimDCT(block2+m*BLOCK_SIZE, block3+m*BLOCK_SIZE, BLOCK_SIZE);
				transpose(block3, BLOCK_SIZE, BLOCK_SIZE);
				for(int k=0; k<BLOCK_SIZE; ++k)
					for(int l=0; l<BLOCK_SIZE; ++l)
						o_image[pos+k*WIDTH+l] = block3[k*BLOCK_SIZE+l];
				delete [] block3;
				delete [] block2;
				delete [] block1;
			}
		}
	}
	else{
		double * tmp_image = new double [WIDTH*HEIGHT];
		for(int i = 0; i < HEIGHT; ++i)
			OneDimDCT( 	i_image + i*WIDTH, tmp_image + i*WIDTH, WIDTH);
		transpose(tmp_image, HEIGHT, WIDTH);
		for(int i = 0; i < WIDTH; ++i)
			OneDimDCT( 	i_image + i*WIDTH, tmp_image + i*WIDTH, WIDTH);
		transpose(o_image, HEIGHT, WIDTH);
		delete [] tmp_image;
	}
}
//----------------------- fDCT -----------------------//

//---------------------- ifDCT -----------------------//
void ifDCT(const double* i_image, unsigned char* o_image, bool blockwise){
	if(blockwise){
		for(int i=0; i<HEIGHT/BLOCK_SIZE; ++i){
			for(int j=0; j<WIDTH/BLOCK_SIZE; ++j){
				int pos = i*BLOCK_SIZE*WIDTH + j*BLOCK_SIZE;
				double *block1 = new double [BLOCK_SIZE*BLOCK_SIZE];
				double *block2 = new double [BLOCK_SIZE*BLOCK_SIZE];
				unsigned char *block3 = new unsigned char [BLOCK_SIZE*BLOCK_SIZE];
				for(int k=0; k<BLOCK_SIZE; ++k)
					for(int l=0; l<BLOCK_SIZE; ++l)
						block1[k*BLOCK_SIZE+l] = i_image[pos+k*WIDTH+l];
				for(int m=0; m<BLOCK_SIZE; ++m)
					iOneDimDCT(block1+m*BLOCK_SIZE, block2+m*BLOCK_SIZE, BLOCK_SIZE);
				transpose(block2, BLOCK_SIZE, BLOCK_SIZE);
				for(int m=0; m<BLOCK_SIZE; ++m)
					iOneDimDCT(block2+m*BLOCK_SIZE, block3+m*BLOCK_SIZE, BLOCK_SIZE);
				transpose(block3, BLOCK_SIZE, BLOCK_SIZE);
				for(int k=0; k<BLOCK_SIZE; ++k)
					for(int l=0; l<BLOCK_SIZE; ++l)
						o_image[pos+k*WIDTH+l] = block3[k*BLOCK_SIZE+l];
				delete [] block3;
				delete [] block2;
				delete [] block1;
			}
		}
	}
	else{
		double * tmp_image = new double [WIDTH*HEIGHT];
		for(int i = 0; i < HEIGHT; ++i)
			iOneDimDCT( 	i_image + i*WIDTH, tmp_image + i*WIDTH, WIDTH);
		transpose(tmp_image, HEIGHT, WIDTH);
		for(int i = 0; i < WIDTH; ++i)
			iOneDimDCT( 	i_image + i*WIDTH, tmp_image + i*WIDTH, WIDTH);
		transpose(o_image, HEIGHT, WIDTH);
		delete [] tmp_image;
	}
}
//---------------------- ifDCT -----------------------//

//--------------------- OneDimDCT ---------------------//
template<class T>
void OneDimDCT(const T* i_image, double* o_image, int num){

	for (int u = 0; u < num; ++u){

		double sum = 0;

		for (int x = 0; x < num; ++x){
			double val = (double)i_image[x];
			double para = ((2*x+1) * PI * u) / (2*num);
			sum += val * cos(para);
		}

		double c = (u==0)? 1/sqrt(2):1;
		double N = sqrt((double)2/(double)num);
		o_image[u] = (double)N * c * sum;
	}
}
//--------------------- OneDimDCT ---------------------//

//-------------------- iOneDimDCT ---------------------//
template<class T>
void iOneDimDCT(const double* i_image, T* o_image, int num){

	for (int x = 0; x < num; ++x){

		double sum = 0;

		for (int u = 0; u < num; ++u){
			double val = i_image[u];
			double para = ((2*x+1) * PI * u) / (2*num);
			double c = (u==0)? 1/sqrt(2):1;
			sum += val * c * cos(para);
		}
		
		double N = sqrt((double)2/(double)num);
		double val = N * sum;
		if(sizeof(T)==1)
			val = (val>255)? 255:(val<0)? 0:val;
		o_image[x] = (T)val;

	}
}

//-------------------- iOneDimDCT ---------------------//

//--------------------- transpose ---------------------//
template<class T>
void transpose(T* block, const int & height, const int & width){

	T* temp = new T [height*width];
	for (int i = 0; i < height; ++i){
		for (int j = 0; j < width; ++j){
			temp[j*width + i] = block[i*width + j];
		}
	}

	for (int k = 0; k < height*width; ++k){
		block[k] = temp[k];
	}
}
//--------------------- transpose ---------------------//

//------------------- normalization -------------------//
void normalization(const double* i_image, unsigned char* o_image, const int& size){

	for (int i = 0; i < size; ++i)
	{
		double val = i_image[i];
		val = (val>127)? 127:(val<-128)? -128: val;
		o_image[i] = (unsigned char)(val+128);
	}
}
//------------------- normalization -------------------//