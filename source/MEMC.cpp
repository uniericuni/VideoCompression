#include "MEMC.h"

void fQuantization(const double* d_image, char* q_image, const bool i_frame){
	for(int i=0; i<HEIGHT/BLOCK_SIZE; ++i){
		for(int j=0; j<WIDTH/BLOCK_SIZE; ++j){
			for(int k=0; k<BLOCK_SIZE*BLOCK_SIZE; ++k){
				int pos = i*BLOCK_SIZE*WIDTH + j*BLOCK_SIZE + WIDTH*(k/BLOCK_SIZE) + k%BLOCK_SIZE;
				double val;
				if(i_frame)
					val = (d_image[pos]/QTableI[k]);
				else
					val = (d_image[pos]/QTableP[k]);
				val = (val<-128)? -128: (val>127)? 127: val;
				q_image[pos] = (char) val;
			}
		}
	}
}

void ifQuantization(const char* q_image, double* d_image, const bool i_frame){
	for(int i=0; i<HEIGHT/BLOCK_SIZE; ++i){
		for(int j=0; j<WIDTH/BLOCK_SIZE; ++j){
			for(int k=0; k<BLOCK_SIZE*BLOCK_SIZE; ++k){
				int pos = i*BLOCK_SIZE*WIDTH + j*BLOCK_SIZE + WIDTH*(k/BLOCK_SIZE) + k%BLOCK_SIZE;
				if(i_frame)
					d_image[pos] = (double)(q_image[pos]) * (double)(QTableI[k]);
				else
					d_image[pos] = (double)(q_image[pos]) * (double)(QTableP[k]);
			}
		}
	}
}

void ME(const unsigned char* img, const unsigned char* buffer, char* ME_x, char* ME_y){
	int SAD, minSAD, Vx=0, Vy=0, pos, loc1, loc2;
	for(int m=0; m<HEIGHT/BLOCK_SIZE; ++m){
		for(int n=0; n<WIDTH/BLOCK_SIZE; ++n){
			Vx = 0;
			Vy = 0;
			minSAD = INF;
			pos = m*BLOCK_SIZE*WIDTH + n*BLOCK_SIZE;
			for(int j=(-1)*(SRANGE/2); j<(SRANGE/2); ++j){
				if(pos/WIDTH+j<0 || pos/WIDTH+BLOCK_SIZE+j>HEIGHT)
					continue;
				for(int i=(-1)*(SRANGE/2); i<(SRANGE/2); ++i){
					if(pos%WIDTH+i<0 || pos%WIDTH+BLOCK_SIZE+i>WIDTH)
						continue;
					SAD = 0;
					for(int k=0; k<BLOCK_SIZE; ++k){
						for(int l=0; l<BLOCK_SIZE; ++l){
							loc1 = pos + l + k*WIDTH;
							loc2 = loc1 + i + j*WIDTH;
							SAD += abs(buffer[loc1]-img[loc2]);
						}
					}
					if(SAD < minSAD){
						minSAD = SAD;
						Vx = i;
						Vy = j;
					}
				}
			}
			ME_x[m*(WIDTH/BLOCK_SIZE)+n] = (char)Vx;
			ME_y[m*(WIDTH/BLOCK_SIZE)+n] = (char)Vy;
		}
	}
}

void fME(const unsigned char* img, const unsigned char* buffer, char* ME_x, char* ME_y){
	int SAD, minSAD, Vx=0, Vy=0, pos, loc1, loc2, range;
	for(int m=0; m<HEIGHT/BLOCK_SIZE; ++m){
		for(int n=0; n<WIDTH/BLOCK_SIZE; ++n){
			pos = m*BLOCK_SIZE*WIDTH + n*BLOCK_SIZE;
			for(int stage=1; stage<=3; ++stage){
				Vx = 0;
				Vy = 0;
				minSAD = INF;
				range = SRANGE/pow(2,stage);
				for(int j=(-1)*range; j<=range; j=j+range){
					if(pos/WIDTH+j<0 || pos/WIDTH+BLOCK_SIZE+j>HEIGHT)
						continue;
					for(int i=(-1)*range; i<=range; i=i+range){
						if(pos%WIDTH+i<0 || pos%WIDTH+BLOCK_SIZE+i>WIDTH)
							continue;
						SAD = 0;
						for(int k=0; k<BLOCK_SIZE; ++k){
							for(int l=0; l<BLOCK_SIZE; ++l){
								loc1 = pos + l + k*WIDTH;
								loc2 = loc1 + i + j*WIDTH;
								SAD += abs(buffer[loc1]-img[loc2]);
							}
						}
						if(SAD < minSAD){
							minSAD = SAD;
							Vx = i;
							Vy = j;
						}
					}
				}
				pos = m*BLOCK_SIZE*WIDTH + n*BLOCK_SIZE + Vy*WIDTH + Vx;
			}
			ME_x[m*(WIDTH/BLOCK_SIZE)+n] = (char)Vx;
			ME_y[m*(WIDTH/BLOCK_SIZE)+n] = (char)Vy;
		}
	}
}

void MC(unsigned char* img, const unsigned char* buffer, char* ME_x, char* ME_y){
	for(int i=0; i<FRAME_SIZE; ++i)
		img[i] = buffer[i];

	for(int i=0; i<HEIGHT/BLOCK_SIZE; ++i){
		for(int j=0; j<WIDTH/BLOCK_SIZE; ++j){
			int Vx = (int)ME_x[i*(WIDTH/BLOCK_SIZE)+j];
			int Vy = (int)ME_y[i*(WIDTH/BLOCK_SIZE)+j];
			//cout << "Vx:"<< Vx << " , Vx:"<<Vy << endl;
			int pos = i*BLOCK_SIZE*WIDTH + j*BLOCK_SIZE;
			for(int m=0; m<BLOCK_SIZE; ++m)
				for(int n=0; n<BLOCK_SIZE; ++n)
					img[pos + m*WIDTH + n + Vy*WIDTH + Vx] = buffer[pos + m*WIDTH + n];
		}
	}
}

void imgAdd(unsigned char* img1, const unsigned char* img2, const bool* signs){
	for(int i=0; i<FRAME_SIZE; ++i){
		int val1 = img1[i];
		int val2 = img2[i];
		int val = (signs[i])? val2-val1 : val1+val2;
		val = (val>255)? 255:(val<0)? 0:val;
		img1[i] = (unsigned char)val;
	}
}

void imgSub(unsigned char* img1, const unsigned char* img2, bool* signs){
	for(int i=0; i<FRAME_SIZE; ++i){
		int val1 = img1[i];
		int val2 = img2[i];
		int val = (val1>val2)? val1-val2 : val2-val1;
		signs[i] = (val1>val2)? 0:1;
		img1[i] = (unsigned char)val;
	}
}
