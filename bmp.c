

#include <stdlib.h>
#include "color.h"
#include <math.h>
#include "bmp.h"
#include "scene3D.h"
#include "json.h"

void bmp_save(const char* fname, size_t width, size_t height, size_t depth, color_t (*pixel)(size_t i, size_t j, void** args), void** args){
	FILE* fdesc = fopen(fname, "wb");
	char* bm = "BM";
	fwrite(bm, 2*sizeof(char),1,fdesc);
	

	size_t R = 4 * ((depth * width +31)/32);
	size_t M = height * R;
	size_t C = 0;
	size_t P = 4*C;
	if(depth == 1 || depth == 8){
		C = pow(2,depth); 
		P = 4 * C;
	}


	int filesize = 54 + P + M ;
	fwrite(&filesize, sizeof(int), 1, fdesc);
	int reserved = 0;
	fwrite(&reserved, sizeof(int), 1, fdesc);
	int offset = 54 + P;
	fwrite(&offset, sizeof(int), 1, fdesc);
	int DIBsize = 40;
	fwrite(&DIBsize, sizeof(int), 1, fdesc);
	int Width = width;
	fwrite(&Width, sizeof(int), 1, fdesc);
	int Height = height;
	fwrite(&Height, sizeof(int), 1, fdesc);
	short plane = 1;
	fwrite(&plane, sizeof(short), 1, fdesc);
	short Depth = depth;
	fwrite(&Depth, sizeof(short), 1, fdesc);
	int compression = 0;
	fwrite(&compression, sizeof(int), 1, fdesc);
	int datasize = M;
	fwrite(&datasize, sizeof(int), 1, fdesc);
	int Xresolution = 2835;
	fwrite(&Xresolution, sizeof(int), 1, fdesc);
	int Yresolution = 2835;
	fwrite(&Yresolution, sizeof(int), 1, fdesc);
	int numbercolor = C;
	fwrite(&numbercolor, sizeof(int), 1, fdesc);
	int numberofimportantcolor = 0;
	fwrite(&numberofimportantcolor, sizeof(int), 1, fdesc);


	unsigned char* ligne = NULL;
	ligne =calloc (R, sizeof(unsigned char));
	if(ligne == NULL) exit(EXIT_FAILURE);

	if(depth == 24){
		unsigned char matrice24[width][height][3];
		for(int j=height-1; j>=0 ;j--){
			int x = 0;
			for(size_t i=0;i<width;i++){
				color_to_BGR(pixel(i,j,args), matrice24[i][j]);
				for(int k = 0; k<3;k++){
					ligne[x] = matrice24[i][j][k];
					x++;
				}
			}
			fwrite(ligne, R, 1, fdesc);
		}
	}

	else if(depth == 8){
		unsigned char palette8[256][4];
		for(int i =0 ; i<256; i++){
			for(unsigned char j=0;j<3;j++){
				palette8[i][j]=i;
			}
			palette8[i][3] =0;
		}
	fwrite(palette8,sizeof(palette8),1,fdesc);

	unsigned char matrice8[width][height];
	for(int j=height-1; j>=0 ;j--){
		for(size_t i=0;i<width;i++){
			matrice8[i][j] = color_to_graylevel(pixel(i,j,args));
				ligne[i] = matrice8[i][j];
		}
		fwrite(ligne, R, 1, fdesc);
	}

	}
	else if(depth == 1){
	unsigned char palette1[2][4];
	for(int i = 0; i < 4; i++) palette1[0][i] = 0;
	for(int i = 0; i < 3; i++) palette1[1][i] = 255;
	palette1[1][3] = 0;
	fwrite(palette1,sizeof(palette1), 1, fdesc);

	for(int j=height-1; j>=0 ;j--){
		for(size_t i=0;i<width;i++){
			if(color_to_BW(pixel(i,j,args)))
				ligne[i/8] = ligne[i/8] | color_to_BW(pixel(i,j,args))<<(7-(i%8));
			else
				ligne[i/8] = ligne[i/8] & color_to_BW(pixel(i,j,args))<<(7-(i%8));
		}
		fwrite(ligne, R, 1, fdesc);
	}

	}


	
	fclose(fdesc);
	free(ligne);

}

	



