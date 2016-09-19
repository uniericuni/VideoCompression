#ifndef RLC_H
#define RLC_H

#include <cstdio>
//#include "huffman.h"
#include "def.h"
#include "func.h"
using namespace func;

void rlc(const char*, string);
void rld(char*, string);
void zigzag(int& , int& , const int&);
void text(ifstream&, vector<string>&);

//FREQTABLE buildFreqTable(string);
//HUFFTABLE buildHuffTable(string);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>

typedef struct {
	FILE *fp;
	long low;
	long high;
	long fbits;
	int buffer;
	int bits_to_go;
	long total_bits;
} ac_encoder;

typedef struct {
	FILE *fp;
	long value;
	long low;
	long high;
	int buffer;
	int bits_to_go;
	int garbage_bits;
} ac_decoder;

typedef struct {
	int nsym;
	int *freq;
	int *cfreq;
	int adapt;
} ac_model;

void ac_encoder_init (ac_encoder *, const char *);
void ac_encoder_done (ac_encoder *);
void ac_decoder_init (ac_decoder *, const char *);
void ac_decoder_done (ac_decoder *);
void ac_model_init (ac_model *, int, int *, int);
void ac_model_done (ac_model *);
long ac_encoder_bits (ac_encoder *);
void ac_encode_symbol (ac_encoder *, ac_model *, int);
int ac_decode_symbol (ac_decoder *, ac_model *);

#endif