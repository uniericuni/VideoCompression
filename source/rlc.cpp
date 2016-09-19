#include "rlc.h"

void rlc(const char* image, string filename) {

	ofstream ofile;
	ofile.open(filename);
	if(!ofile)
		cout << "Fail to create " + FILENAME + ".rlc !" << endl;

	// Block level scan
	for (int i = 0; i < HEIGHT/BLOCK_SIZE; ++i){
		for (int j = 0; j < WIDTH/BLOCK_SIZE; ++j){

			// Pixel level scan
			int blk_pos 	= j*BLOCK_SIZE + i*BLOCK_SIZE*WIDTH;
			int cur_pos 	= blk_pos;
			int zero_count = 0, zig_x = 0, zig_y = 0;

			while(zig_x != BLOCK_SIZE && zig_y != BLOCK_SIZE){
				int val = (int)image[cur_pos];
				if(!val)
					zero_count++;
				else{
					ofile << (char)zero_count  << (char)val;
					zero_count = 0;
				}
				zigzag(zig_x, zig_y, BLOCK_SIZE);
				cur_pos = blk_pos + zig_x + zig_y*WIDTH;
			}
			ofile << '\n';
		}
	}
	ofile.close();
}

void rld(char* image, string filename){

	ifstream ifile;
	ifile.open(filename);
	vector<string> texts;
	text(ifile,texts);

	// Block level scan
	for (int i = 0; i < HEIGHT/BLOCK_SIZE; ++i){
		for (int j = 0; j < WIDTH/BLOCK_SIZE; ++j){
			
			// Pixel level scan
			int blk_pos 	= j*BLOCK_SIZE + i*BLOCK_SIZE*WIDTH;
			int cur_pos 	= blk_pos;
			int zig_x = 0, zig_y = 0;
			vector<int> zero_nums;
			vector<int> vals;

			string str = texts[i*HEIGHT/BLOCK_SIZE+j];
			
			while(!str.empty()){
				size_t pos1 = str.find("(");
				size_t pos2 = str.find(",");
				size_t pos3 = str.find(")");
				zero_nums.push_back( stoi(str.substr(pos1+1,pos2)) );
				vals.push_back( stoi(str.substr(pos2+1,pos3)) );
				
				str = str.substr(pos3+1);
			}

			while(zig_x != BLOCK_SIZE && zig_y != BLOCK_SIZE){
				if(!zero_nums.empty()){
					vector<int>::iterator it = zero_nums.begin();
					int num = zero_nums.front();
					zero_nums.erase(it);
					it = vals.begin();
					int val = vals.front();
					vals.erase(it);

					for (int k = num; k >= 0; --k){
						image[cur_pos] = 0;
						if(k==0) image[cur_pos] = (int)val;
						zigzag(zig_x, zig_y, BLOCK_SIZE);
						cur_pos = blk_pos + zig_x + zig_y*WIDTH;
					}
				}
				else{
					image[cur_pos] = 0;
					zigzag(zig_x, zig_y, BLOCK_SIZE);
					cur_pos = blk_pos + zig_x + zig_y*WIDTH;	
				}
			}
		}
	}
	ifile.close();
}
/*
FREQTABLE buildFreqTable(string ifilename){
	
	char temp[1024], val;
	
	FILE *ifile;
	FREQTABLE freq_table;
	strcpy(temp, ifilename.c_str());
	ifile = fopen(temp, "rb");

	while((val=fgetc(ifile)) != EOF){
		if(freq_table.find(val) == freq_table.end())
			freq_table.insert( pair<char,int>(val,1) );
		else
			freq_table.find(val)->second += 1;
	}
	//freq_table.insert(pair<char, int>(DIV, freq_table.size()));
	//freq_table.insert(pair<char, int>(START, 1));
	
	fclose(ifile);

	return freq_table;
}

HUFFTABLE buildHuffTable(string ifilename){

	char temp[1024];
	char val;
	
	FILE *ifile;
	HUFFTABLE huff_table;
	strcpy(temp, ifilename.c_str());
	ifile = fopen(temp, "rb");

	int state = 0, c_size;
	char key;
	char* ptrEnd;
	string str, code;		

	while((val=fgetc(ifile)) != EOF){
		str = str + bitset<8>((int)val).to_string();

		if(state == 0){
			const char* c_str = (str.substr(0, 8)).data();
			long int c_int = strtol(c_str, &ptrEnd, 2);
			key = (char)c_int;
			
			state = 1;
			str = str.substr(8);
		}
		else if( state == 1){
			const char* c_str = (str.substr(0, 8)).data();
			long int c_int = strtol(c_str, &ptrEnd, 2);
			c_size = (int)((char)c_int - '0');

			state = 2;
			str = str.substr(8);
		}
		else if( state == 2 && str.size() >= c_size){
			code = str.substr(0, c_size);
			huff_table.insert(pair<char, string>(key, code));
			
			state = 0;
			str = str.substr(c_size);
			
		}
		if( c_size == 0)
			break;
	}
	fclose(ifile);

	return huff_table;
}
*/

void zigzag(int& x, int& y, const int& block_size) {
	if((y==0 && x%2==0) || (y==block_size-1 && x%2==0))			{ x++; }
	else if((x==0 && y%2==1) || (x==block_size-1 && y%2==1))  	{ y++; }
	else if((x+y)%2==0) 									   	{ x++; y--; }
	else			 										   	{ x--; y++; }
}

void text(ifstream& file, vector<string>& text){
	while(!file.eof()){
		string str;
		getline(file, str);
		text.push_back(str);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#define Code_value_bits 16

#define Top_value (((long)1<<Code_value_bits)-1)
#define First_qtr (Top_value/4+1)
#define Half		(2*First_qtr)
#define Third_qtr (3*First_qtr)
#define Max_frequency 16383

//----------------------------------------------
// For debug constant variables (SKII, 04/09/25)
static int _Code_value_bits = Code_value_bits;
static int _Top_value = Top_value;
static int _First_qtr = First_qtr;
static int _Half = Half;
static int _Third_qtr = Third_qtr;
static int _Max_frequency = Max_frequency;
//----------------------------------------------

static void output_bit (ac_encoder *, int);
static void bit_plus_follow (ac_encoder *, int);
static int input_bit (ac_decoder *);
static void update_model (ac_model *, int);

#define error(m)                                            \
do	{                                                       \
	fflush (stdout);                                        \
	fprintf (stderr, "%s:%d: error: ", __FILE__, __LINE__); \
	fprintf (stderr, m);                                    \
	fprintf (stderr, "\n");                                 \
	exit (1);                                               \
}  while (0)

#define check(b,m)                                          \
do	{                                                       \
	if (b)                                                  \
		error (m);                                          \
}  while (0)

static void
output_bit (ac_encoder *ace, int bit)
{
	ace->buffer >>= 1;
	if (bit)
		ace->buffer |= 0x80;
	ace->bits_to_go -= 1;
	ace->total_bits += 1;
	if (ace->bits_to_go==0)  {
		if (ace->fp)
			putc (ace->buffer, ace->fp);
		ace->bits_to_go = 8;
	}

	return;
}

static void
bit_plus_follow (ac_encoder *ace, int bit)
{
	output_bit (ace, bit);
	while (ace->fbits > 0)	{
		output_bit (ace, !bit);
		ace->fbits -= 1;
	}

	return;
}

static int
input_bit (ac_decoder *acd)
{
	int t;

	if (acd->bits_to_go==0)  {
		acd->buffer = getc(acd->fp);
		if (acd->buffer==EOF)  {
			acd->garbage_bits += 1;
			if (acd->garbage_bits>Code_value_bits-2)
				error ("arithmetic decoder bad input file");
		}
		acd->bits_to_go = 8;
	}

	t = acd->buffer&1;
	acd->buffer >>= 1;
	acd->bits_to_go -= 1;

	return t;
}

static void
update_model (ac_model *acm, int sym)
{
	int i;

	if (acm->cfreq[0]==Max_frequency)  {
		int cum = 0;
		acm->cfreq[acm->nsym] = 0;
		for (i = acm->nsym-1; i>=0; i--)	{
			acm->freq[i] = (acm->freq[i] + 1) / 2;
			cum += acm->freq[i];
			acm->cfreq[i] = cum;
		}
	}

	acm->freq[sym] += 1;
	for (i=sym; i>=0; i--)
		acm->cfreq[i] += 1;

	return;
}

void
ac_encoder_init (ac_encoder *ace, const char *fn)
{

	if (fn)  {
		ace->fp = fopen (fn, "wb"); /* open in binary mode */
		check (!ace->fp, "arithmetic encoder could not open file");
	}  else  {
		ace->fp = NULL;
	}

	ace->bits_to_go = 8;

	ace->low = 0;
	ace->high = Top_value;
	ace->fbits = 0;
	ace->buffer = 0;

	ace->total_bits = 0;

	return;
}

void
ac_encoder_done (ac_encoder *ace)
{
	ace->fbits += 1;
	if (ace->low < First_qtr)
		bit_plus_follow (ace, 0);
	else
		bit_plus_follow (ace, 1);
	if (ace->fp)
		putc (ace->buffer >> ace->bits_to_go, ace->fp);

	if (ace->fp)
		fclose (ace->fp);

	return;
}

void
ac_decoder_init (ac_decoder *acd, const char *fn)
{
	int i;

	acd->fp = fopen (fn, "rb"); /* open in binary mode */
	check (!acd->fp, "arithmetic decoder could not open file");

	acd->bits_to_go = 0;
	acd->garbage_bits = 0;

	acd->value = 0;
	for (i=1; i<=Code_value_bits; i++)
		acd->value = 2*acd->value + input_bit(acd);
	acd->low = 0;
	acd->high = Top_value;

	return;
}

void
ac_decoder_done (ac_decoder *acd)
{
	fclose (acd->fp);

	return;
}

void
ac_model_init (ac_model *acm, int nsym, int *ifreq, int adapt)
{
	int i;

	acm->nsym = nsym;
	acm->freq = (int *) (void *) calloc (nsym, sizeof (int));
	check (!acm->freq, "arithmetic coder model allocation failure");
	acm->cfreq = (int *) (void *) calloc (nsym+1, sizeof (int));
	check (!acm->cfreq, "arithmetic coder model allocation failure");
	acm->adapt = adapt;

	if (ifreq)	{
		acm->cfreq[acm->nsym] = 0;
		for (i=acm->nsym-1; i>=0; i--)	{
			acm->freq[i] = ifreq[i];
			acm->cfreq[i] = acm->cfreq[i+1] + acm->freq[i];
		}
		if (acm->cfreq[0] > Max_frequency)
			error ("arithmetic coder model max frequency exceeded");
	}  else  {
		for (i=0; i<acm->nsym; i++) {
			acm->freq[i] = 1;
			acm->cfreq[i] = acm->nsym - i;
		}
		acm->cfreq[acm->nsym] = 0;
	}

	return;
}

void
ac_model_done (ac_model *acm)
{
	acm->nsym = 0;
	free (acm->freq);
	acm->freq = NULL;
	free (acm->cfreq);
	acm->cfreq = NULL;

	return;
}

long
ac_encoder_bits (ac_encoder *ace)
{
	return ace->total_bits;
}

void
ac_encode_symbol (ac_encoder *ace, ac_model *acm, int sym)
{
	long range;

	check (sym<0||sym>=acm->nsym, "symbol out of range");

	range = (long)(ace->high-ace->low)+1;
	ace->high = ace->low + (range*acm->cfreq[sym])/acm->cfreq[0]-1;
	ace->low = ace->low + (range*acm->cfreq[sym+1])/acm->cfreq[0];

	for (;;)	{
		if (ace->high<Half)  {
			bit_plus_follow (ace, 0);
		}  else if (ace->low>=Half)  {
			bit_plus_follow (ace, 1);
			ace->low -= Half;
			ace->high -= Half;
		}  else if (ace->low>=First_qtr && ace->high<Third_qtr)  {
			ace->fbits += 1;
			ace->low -= First_qtr;
			ace->high -= First_qtr;
		}  else
			break;
		ace->low = 2*ace->low;
		ace->high = 2*ace->high+1;
	}

	if (acm->adapt)
		update_model (acm, sym);

	return;
}

int
ac_decode_symbol (ac_decoder *acd, ac_model *acm)
{
	long range;
	int cum;
	int sym;

	range = (long)(acd->high-acd->low)+1;
	cum = (((long)(acd->value-acd->low)+1)*acm->cfreq[0]-1)/range;

	for (sym = 0; acm->cfreq[sym+1]>cum; sym++)
		/* do nothing */ ;

	check (sym<0||sym>=acm->nsym, "symbol out of range");

	acd->high = acd->low + (range*acm->cfreq[sym])/acm->cfreq[0]-1;
	acd->low = acd->low +  (range*acm->cfreq[sym+1])/acm->cfreq[0];

	for (;;)	{
		if (acd->high<Half)  {
			/* do nothing */
		}  else if (acd->low>=Half)  {
			acd->value -= Half;
			acd->low -= Half;
			acd->high -= Half;
		}  else if (acd->low>=First_qtr && acd->high<Third_qtr)  {
			acd->value -= First_qtr;
			acd->low -= First_qtr;
			acd->high -= First_qtr;
		}  else
			break;
		acd->low = 2*acd->low;
		acd->high = 2*acd->high+1;
		acd->value = 2*acd->value + input_bit(acd);
	}

	if (acm->adapt)
		update_model (acm, sym);

	return sym;
}