#ifndef MEMC_h
#define MEMC_h

#include "def.h"
#include "func.h"
using namespace func;

void fQuantization(const double*, char*, const bool);
void ifQuantization(const char*, double*, const bool);
void ME(const unsigned char*, const unsigned char*, char*, char*);
void fME(const unsigned char*, const unsigned char*, char*, char*);
void MC(unsigned char*, const unsigned char*, char*, char*);
void imgAdd(unsigned char*, const unsigned char*, const bool *);
void imgSub(unsigned char*, const unsigned char*, bool *);

#endif