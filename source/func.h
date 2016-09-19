#ifndef FUNC_H
#define FUNC_H

#include "def.h"

namespace func {

long double psnr(unsigned char*, unsigned char*, int);		// PSNR
															//
															// psnr(image1, image2, size)
															// image1: 8-bit raw file1 | image2: 8-bit raw file2 | size: the size of the file


const int BitNum(const int &);								// Bit Number Counting
															//
															// BitNum(int)
															// num: the integer to count bit | return: bit number of int
}

#endif
