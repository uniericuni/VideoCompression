#include "func.h"

//----------------------- psnr ------------------------//
long double func::psnr(unsigned char* i_image, unsigned char* o_image, int num){
	unsigned long mse = 0;
	long double mse_2 = 0;
	long double psnr = 0;

	for (int i = 0; i < num; ++i){
		unsigned long i_val = (unsigned long)i_image[i];
		unsigned long o_val = (unsigned long)o_image[i];
		mse += (i_val > o_val)? pow((i_val-o_val),2):pow((o_val-i_val),2);
	}
	mse_2 = mse/(HEIGHT*WIDTH);

	if(mse_2 == 0)
		psnr = -1;
	else
		psnr = 10*log10( (255*255)/mse_2 );

	return psnr;
}
//----------------------- psnr ------------------------//

//---------------------- BitNum -----------------------//
const int func::BitNum(const int &n){			

	// bit number doesn't include sign bit
	
	int num = 0;
	int m = abs(n);
	if(n==0)
		return 1;
	while(m>0){
		m = m>>1;
		num++;
	}

	return num;
}
//---------------------- BitNum -----------------------//