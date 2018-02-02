#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"

void program_1(int width, int height, char* image_name, int image_format);
void program_2(int width, int height, char* image_name, int image_format);
void program_3(int width, int height, char* image_name, int image_format);

int main(int argc, char *argv[]){
	if(argc != 6){
		printf("Please ensure you have entered five arguments. You entered: %d\n", argc - 1);
		exit(0);
	}

	int code = atoi(argv[1]);
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	char* image_name = argv[4];
	int image_format = atoi(argv[5]);


	if (height % 4 != 0 || height < 4) {
		printf("Please enter a valid height.\n");
		exit(0);
	}

	if(code == 1 || code == 2){
		if (width % 4 != 0 || width < 4){
			printf("Please enter a valid width.\n");
			exit(0);
		}
	} else if (code == 3){
		if (width % 6 != 0 || width < 6){
			printf("Please enter a valid width.\n");
			exit(0);
		}
	} else{
		printf("Please ensure you have entered a valid code.\n");
		exit(0);		
	}

	if (image_format != 0 && image_format != 1){
		printf("Please ensure you have entered a valid format.\n");
		exit(0);
	}

	if(code == 1){
		program_1(width, height, image_name, image_format);
	} else if (code == 2){
		// program_2(width, height, image_name, image_format);
	} else {
		// program_3(width, height, image_name, image_format);
	}


	return 0;
}

int min(int a, int b){
	return a > b ? b : a;
}

void program_1(int width, int height, char* image_name, int image_format){
	
	// Initalize a 2D array to store the image
	struct PBM_Image *pbm_image = malloc(sizeof(struct PBM_Image));
	create_PBM_Image(pbm_image, width, height);

	// Building the outer box
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){

			// Building the cross in the middle
			if(j < width * 0.25 || j >= width * 0.75 || i < height * 0.25 || i >= height * 0.75){
				pbm_image->image[i][j] = BLACK;
			} else{
				pbm_image->image[i][j] = WHITE;				
			}
		}
	}

	// // Building "x"
	const int START_Y = (int)(0.25 * height);
	const int START_X = (int)(0.25 * width);
	const int SIZE_Y = (int)(0.5 * height);
	const int SIZE_X = (int)(0.5 * width);
	const float BOUNDARY_Y = 0.75 * height;
	const float BOUNDARY_X = 0.75 * width;

	int cur_y = 0;
	int cur_x = 0;

	while(cur_y < SIZE_Y || cur_x < SIZE_X){

		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + min(SIZE_X, cur_x)] = 3;	
		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + SIZE_X - min(SIZE_X, cur_x) - 1] = 3;	


		int inc_y = false;
		int inc_x = false;
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) <= (cur_x + 1.0) / (BOUNDARY_X - 1)){
			inc_y = true;
		} 

		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) >= (cur_x + 1.0) / (BOUNDARY_X - 1)) {
			inc_x = true;
		} 

		if(inc_y){
			cur_y++;
		}
		if(inc_x){
			cur_x++;
		}
	}

	// for(int i = 0; i < height; i++){
	// 	for(int j = 0; j < width; j++){
	// 		printf(" %d", pbm_image->image[i][j]);
	// 	}
	// 	printf("\n");
	// }

	save_PBM_Image(pbm_image, image_name, image_format);
	free_PBM_Image(pbm_image);

}


void program_2(int width, int height, char* image_name, int image_format){

	/*
	For this question what I can do is go from top to bottom and left to right filling in 
	values but the tricky part will be knowing when each triangle ends.

	I think I can "cheese" this question by making an image similar to how I did in 
	program-1, except now when I go left to write I'll either wait until I'm outside 
	the two lines (for the left/right triangles), or I'll wait until I'm within 
	them (for the up/down triangles).
	*/
	
	return;
}