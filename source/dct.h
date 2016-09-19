#ifndef DCT_H
#define DCT_H

#include "def.h"
#include "func.h"
using namespace func;

void DCT(const unsigned char*, double*);					// Digital Cousine Transformation
															//
															// DCT(i_image, o_image)
															// i_image: 8-bit raw file | o_image: floating-point coefficient array

void iDCT(const double*, unsigned char*);					// Inverse Digital Cousine Transformation
															//
															// iDCT(i_image, o_image)
															// i_image: floating-point coefficient array | o_image: 8-bit raw file

void Quantization(const double*, unsigned char*,			// Quantization
				  int*, bool);								//
															// Quantization(i_image, o_image, truncation, dead_zone)
															// i_image: floating-point coefficient array | o_image: 8-bit raw file | truncation: the truncated bits of DC and AC| dead_zone: TRUE for ignorance of -7 ~ 7

void iQuantization(const unsigned char*, double*, int*);	// inverse Quantization
				   											//
															// iQuantization(i_image, o_image, truncation)
															// i_image: 8-bit raw file | o_image: floating-point coefficient array | truncation: the truncated bits of DC and AC

void fDCT(const unsigned char*, double*, bool);					// (block-wise) fast DCT
															//
															// fDCT(i_image, o_image, blockwise)
															// i_image: 8-bit raw file | o_image: floating-point coefficient array | blockwise: true for blockwise

void ifDCT(const double*, unsigned char*, bool);			// (block-wise) fast iDCT
															//
															// fDCT(i_image, o_image, blockwise)
															// i_image: floating-point coefficient array | o_image: 8-bit raw file | blockwise: true for blockwise
template<class T>
void OneDimDCT(const T*, double*, int);						// 1D DCT
															//
															// OneDimDCT(i_image, o_image, size)
															// i_image: 8-bit raw file | o_image: floating-point coefficient array | size: size of the array

template<class T>
void iOneDimDCT(const double*, T*, int);					// 1D iDCT
															//
															// iOneDimDCT(i_image, o_image, size)
															// i_image:  floating-point coefficient array | o_image: floating-point coefficient array | size: size of the array

template<class T>
void transpose(T*, const int &, const int &);				// transpose
															// 
															// transpose(array, height, width)
															// array: array to transpose | height: the row number of array | width: the column number of array

void normalization(const double*, unsigned char*, 			// normalization
				   const int&);								//
															// normalization(i_image, o_image, size)
															// i_image: floating-point coefficient array | o_image: 8-bit raw file | size: the size of image

#endif