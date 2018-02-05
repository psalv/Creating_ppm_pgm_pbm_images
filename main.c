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
	bool image_format = atoi(argv[5]);


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
		program_2(width, height, image_name, image_format);
	} else {
		// program_3(width, height, image_name, image_format);
	}

	return 0;
}


int min(int a, int b){
	return a > b ? b : a;
}

int max(int a, int b){
	return a > b ? a : b;
}


void program_1(int width, int height, char* image_name, bool image_format){
	
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

		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + min(SIZE_X, cur_x)] = BLACK;	
		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + SIZE_X - min(SIZE_X, cur_x) - 1] = BLACK;	


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

	save_PBM_Image(pbm_image, image_name, image_format);
	free_PBM_Image(pbm_image);
	free(pbm_image);
}


void program_2(int width, int height, char* image_name, int image_format){

	// Initalize a 2D array to store the image
	struct PGM_Image *pgm_image = malloc(sizeof(struct PGM_Image));
	create_PGM_Image(pgm_image, width, height, MAX_GRAY_VALUE);

	// Building the outer box
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){

			// Building the cross in the middle
			if(j < width * 0.25 || j >= width * 0.75 || i < height * 0.25 || i >= height * 0.75){
				pgm_image->image[i][j] = 0;
			} else{
				pgm_image->image[i][j] = MAX_GRAY_VALUE;				
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
	const int STEP_Y = max(1, 255/((SIZE_Y)/2));
	const int STEP_X = max(1, 255/((SIZE_X)/2 - 1));


	// Cross (from program 1)
	int cur_y = 0;
	int cur_x = 0;
	int cur_x_gray = MAX_GRAY_VALUE;
	int cur_y_gray = MAX_GRAY_VALUE;

	while(cur_y < SIZE_Y/2 || cur_x < SIZE_X/2){

		pgm_image->image[START_Y + min(SIZE_Y/2, cur_y)][START_X + min(SIZE_X/2, cur_x)] = (cur_x_gray + cur_y_gray) / 2;	
		pgm_image->image[(int)BOUNDARY_Y - min(SIZE_Y/2, cur_y) - 1][START_X + min(SIZE_X/2, cur_x)] = (cur_x_gray + cur_y_gray) / 2;	

		pgm_image->image[START_Y + min(SIZE_Y/2, cur_y)][START_X + SIZE_X - min(SIZE_X/2, cur_x) - 1] = (cur_x_gray + cur_y_gray) / 2;
		pgm_image->image[(int)BOUNDARY_Y - min(SIZE_Y/2, cur_y) - 1][START_X + SIZE_X - min(SIZE_X/2, cur_x) - 1] = (cur_x_gray + cur_y_gray) / 2;

		int inc_y = false;
		int inc_x = false;
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) <= (cur_x + 1.0) / (BOUNDARY_X - 1)){
			inc_y = true;

			// Vertical triangles, moving across when we know that x boundary is finalized
			for(int j = START_X + cur_x + 1; j < BOUNDARY_X -  min(SIZE_X, cur_x) - 1; j++){

				pgm_image->image[START_Y + min(SIZE_Y, cur_y)][j] = cur_y_gray;	
				pgm_image->image[(int)BOUNDARY_Y - cur_y - 1][j] = cur_y_gray;	
			}

			cur_y_gray = max(0, cur_y_gray - STEP_Y);
		} 


		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) >= (cur_x + 1.0) / (BOUNDARY_X - 1)) {
			inc_x = true;

			// Side-ways triangles, moving down when we know that y boundary is finalized
			for(int j = START_Y + cur_y + 1; j <= BOUNDARY_Y -  min(SIZE_Y, cur_y) - 1; j++){

				pgm_image->image[j][START_X + min(SIZE_X, cur_x)] = cur_x_gray;	
				pgm_image->image[j][(int)BOUNDARY_X - cur_x - 1] = cur_x_gray;	
			}

			cur_x_gray = max(0, cur_x_gray - STEP_X);
		} 

		if(inc_y){
			cur_y++;
		}
		if(inc_x){
			cur_x++;
		}
	}

	
	// Horizontal gradient
	// int cur_x_gray = MAX_GRAY_VALUE;

	// for(int i = START_X; i < START_X + (int)((SIZE_X + 1)/2) ; i++){
	// 	for(int j = START_Y; j < BOUNDARY_Y; j++){

	// 		pgm_image->image[j][i] = cur_x_gray;	
	// 		pgm_image->image[j][(int)BOUNDARY_X - (i - START_X) - 1] = cur_x_gray;	
	// 	}

	// 	cur_x_gray -= STEP_X;
	// }

 
	// for(int i = 0; i < height; i++){
	// 	for(int j = 0; j < width; j++){
	// 		printf(" %4d", pgm_image->image[i][j]);
	// 	}
	// 	printf("\n");
	// }

	save_PGM_Image(pgm_image, image_name, image_format);
	free_PGM_Image(pgm_image);
	free(pgm_image);
}





void program_3(int width, int height, char* image_name, int image_format){

/*

	// // Building "x"
	const int START_Y = (int)(0.25 * height);
	const int START_X = (int)(0.25 * width);
	const int SIZE_Y = (int)(0.5 * height);
	const int SIZE_X = (int)(0.5 * width);
	const float BOUNDARY_Y = 0.75 * height;
	const float BOUNDARY_X = 0.75 * width;
	const int STEP_Y = 255/((SIZE_Y)/2);
	const int STEP_X = 255/((SIZE_X)/2 - 1);
	
	// Vertical gradient
	// int cur_y_gray = MAX_GRAY_VALUE;
	// for(int i = START_Y; i < START_Y + (int)((SIZE_Y + 1)/2) + 1; i++){
		
	// 	for(int j = START_X; j < BOUNDARY_X; j++){
	// 		pgm_image->image[i][j] = cur_y_gray;	
	// 		pgm_image->image[(int)BOUNDARY_Y - (i - START_Y) - 1][j] = cur_y_gray;	
	// 	}

	// 	cur_y_gray -= STEP_Y;
	// }

*/	
}
