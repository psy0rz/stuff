extern "C" {
#include <netpbm/pnm.h>
#include <ctime>
}

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <cstdlib>

#define W 80
#define H 8

const int scrollDelay = 100000;

int main(int argc, char *argv[])
{
	float t;
	float r;
	int x, y;	
	uint8_t img[H][W][3];

	srand(time(NULL));

	int max = 50;
	if (argc == 2) {
		max = atoi(argv[1]);
	}

	int loop = 0;
	t = (rand()%30)/100.1;	
	r = (rand()%50)/100.1;	
	t=0;
	r=0;

	memset(img, 0, sizeof(img));

	while (loop < max) {

		//new 
		for (int l=0; l<2; l++)
    	{
    		int add_y=rand()%H;
    		int add_x=rand()%W;
			img[add_y][add_x][0] = 255;
			img[add_y][add_x][1] = 255;
			img[add_y][add_x][2] = 255;
		}

		for (y=H-1; y>=0; y--) {
	        for (x=0; x<W; x++) {

				//fall
				if (y<H-1 && img[y][x][0]>10)
				{
					img[y+1][x][0]=img[y][x][0];
					img[y+1][x][1]=img[y][x][1];
					img[y+1][x][2]=img[y][x][2];

				}

	        	//dim
				img[y][x][0]=img[y][x][0]/3;
				img[y][x][1]=img[y][x][1]/1.1;
				img[y][x][2]=img[y][x][2]/3;

       		}
    	}

		write(1, img, sizeof img);

		usleep(scrollDelay);
		loop++;
	}
}
