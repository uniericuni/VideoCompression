#ifndef DEF_H
#define DEF_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <iomanip>
#include <cassert>
#include <vector>
using namespace std;

#ifndef PI
#define PI 3.14159265
#endif

#ifndef DC_LIMIT
#define DC_LIMIT 16
#endif

#ifndef AC_LIMIT
#define AC_LIMIT 8
#endif

#ifndef BLOCK_SIZE
#define BLOCK_SIZE 8
#endif

#ifndef HEIGHT
#define HEIGHT 288
#endif

#ifndef WIDTH
#define WIDTH 352
#endif

#ifndef FRAME_SIZE
#define FRAME_SIZE 101376
#endif

#ifndef SRANGE
#define SRANGE 32
#endif

#ifndef GOP_SIZE
#define GOP_SIZE 16
#endif

#ifndef INF
#define INF 2147483647
#endif

const char END = 1;

const string FILEPATH = "./../resource/";
const string FILENAME = "weather";
const string OUTPATH = "./../output/";
const string FILEFORMAT = ".y";

const int QTableI[BLOCK_SIZE*BLOCK_SIZE] = { 8, 16, 19, 22, 26, 27, 29, 34,
			 								16, 16, 22, 24, 27, 29, 34, 37,
			 								19, 22, 26, 27, 29, 34, 34, 38,
			 								22, 22, 26, 27, 29, 34, 37, 40,
			 								22, 26, 27, 29, 32, 25, 40, 48,
			 								26, 27, 29, 32, 35, 40, 48, 58,
			 								26, 27, 29, 34, 38, 46, 56, 69,
			 								27, 29, 35, 38, 46, 59, 69, 83 };

const int QTableP[BLOCK_SIZE*BLOCK_SIZE] = {16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16,
			 								16, 16, 16, 16, 16, 16, 16, 16 };

#endif