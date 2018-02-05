#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libpnm.h"


void program_1(int width, int height, char* image_name, int image_format);
void program_2(int width, int height, char* image_name, int image_format);
void program_3(int width, int height, char* image_name, int image_format);


int main(int argc, char *argv[]){

	// Checking that the appropriate number of args were passed
	if(argc != 6){
		printf("Please ensure you have entered five arguments. You entered: %d\n", argc - 1);
		exit(0);
	}

	// Converting args to their proper types
	int code = atoi(argv[1]);
	int width = atoi(argv[2]);
	int height = atoi(argv[3]);
	char* image_name = argv[4];
	bool image_format = atoi(argv[5]);

	// Checking that the height conforms to specifications
	if (height % 4 != 0 || height < 4) {
		printf("Please enter a valid height.\n");
		exit(0);
	}

	// Checking that the width and code conform to specifications
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

	// Checking that the image format and code conform to specifications
	if (image_format != 0 && image_format != 1){
		printf("Please ensure you have entered a valid format.\n");
		exit(0);
	}

	// Running the appropriate program, as specified by the code
	if(code == 1){
		program_1(width, height, image_name, image_format);
	} else if (code == 2){
		program_2(width, height, image_name, image_format);
	} else {
		// program_3(width, height, image_name, image_format);
	}

	return 0;
}

// Determines the min of two numbers
int min(int a, int b){
	return a > b ? b : a;
}

// Determines the max of two numbers
int max(int a, int b){
	return a > b ? a : b;
}


void program_1(int width, int height, char* image_name, bool image_format){
	
	// Initalize a pbm image and input it's parameters
	struct PBM_Image *pbm_image = malloc(sizeof(struct PBM_Image));
	create_PBM_Image(pbm_image, width, height);

	// Building the outer rectangle
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){

			// If we are outside the middle rectangle, we make the region black, otherwise we make the inner rectangle white.
			if(j < width * 0.25 || j >= width * 0.75 || i < height * 0.25 || i >= height * 0.75){
				pbm_image->image[i][j] = BLACK;
			} else{
				pbm_image->image[i][j] = WHITE;				
			}
		}
	}

	/*** Building the "x" ***/

	// X and Y starting positions for the inner rectngle
	const int START_Y = (int)(0.25 * height);
	const int START_X = (int)(0.25 * width);
	
	// The width and height of the inner rectangle
	const int SIZE_Y = (int)(0.5 * height);
	const int SIZE_X = (int)(0.5 * width);

	// The X and Y end positions for the inner rectangle
	const float BOUNDARY_Y = 0.75 * height;
	const float BOUNDARY_X = 0.75 * width;

	// Current position variables
	int cur_y = 0;
	int cur_x = 0;

	// When finish when we have traversed enough x and y positions to cover the entire inner rectangle's width and height
	while(cur_y < SIZE_Y || cur_x < SIZE_X){

		// We find the current position and make it black
		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + min(SIZE_X, cur_x)] = BLACK;	
		
		// We do the same for a lower line which will run bottom to top, rather than top to bottom
		pbm_image->image[START_Y + min(SIZE_Y, cur_y)][START_X + SIZE_X - min(SIZE_X, cur_x) - 1] = BLACK;	

		// Booleans for determining when to move to the right or down by increasing our position variables
		bool inc_y = false;
		bool inc_x = false;

		// If the percentage that we have traversed y is less than or equal to the percentage that
		// we have traversed x, than we want to move down since we have more vertical space to 
		// finish traversing than horizontal
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) <= (cur_x + 1.0) / (BOUNDARY_X - 1)){
			inc_y = true;
		} 

		// If the percentage that we have traversed y is greater than or equal to the percentage that
		// we have traversed x, than we want to move right since we have more horizontal space to 
		// finish traversing than vertical
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) >= (cur_x + 1.0) / (BOUNDARY_X - 1)) {
			inc_x = true;
		} 

		// Perform the movements by increasing the position variables
		if(inc_y){
			cur_y++;
		}
		if(inc_x){
			cur_x++;
		}
	}

	// Save the image and clear allocated memory
	save_PBM_Image(pbm_image, image_name, image_format);
	free_PBM_Image(pbm_image);
	free(pbm_image);
}


void program_2(int width, int height, char* image_name, int image_format){

	// Initalize a pgm image and input it's parameters
	struct PGM_Image *pgm_image = malloc(sizeof(struct PGM_Image));
	create_PGM_Image(pgm_image, width, height, MAX_GRAY_VALUE);

	// Building the outer rectangle
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){

			// If we are outside the middle rectangle, we make the region black, otherwise we make the inner rectangle white.
			if(j < width * 0.25 || j >= width * 0.75 || i < height * 0.25 || i >= height * 0.75){
				pgm_image->image[i][j] = 0;
			} else{
				pgm_image->image[i][j] = MAX_GRAY_VALUE;				
			}
		}
	}

	/*** Building the gradients ***/

	// X and Y starting positions for the inner rectngle
	const int START_Y = (int)(0.25 * height);
	const int START_X = (int)(0.25 * width);
	
	// The width and height of the inner rectangle
	const int SIZE_Y = (int)(0.5 * height);
	const int SIZE_X = (int)(0.5 * width);

	// The X and Y end positions for the inner rectangle
	const float BOUNDARY_Y = 0.75 * height;
	const float BOUNDARY_X = 0.75 * width;

	// The step sizes with which each pixel of the triangles will advance, minimum set at 1
	const int STEP_Y = max(1, 255/((SIZE_Y)/2));
	const int STEP_X = max(1, 255/((SIZE_X)/2 - 1));

	// Current position variables
	int cur_y = 0;
	int cur_x = 0;

	// Current pixel intensity variables
	int cur_x_gray = MAX_GRAY_VALUE;
	int cur_y_gray = MAX_GRAY_VALUE;

	// We only need to traverse the first quarter quadrant of the image since this will be mirrored
	// SO once each of our position variables pass this section we can finish
	while(cur_y < SIZE_Y/2 || cur_x < SIZE_X/2){




		pgm_image->image[START_Y + min(SIZE_Y/2, cur_y)][START_X + min(SIZE_X/2, cur_x)] = (cur_x_gray + cur_y_gray) / 2;	
		pgm_image->image[(int)BOUNDARY_Y - min(SIZE_Y/2, cur_y) - 1][START_X + min(SIZE_X/2, cur_x)] = (cur_x_gray + cur_y_gray) / 2;	

		pgm_image->image[START_Y + min(SIZE_Y/2, cur_y)][START_X + SIZE_X - min(SIZE_X/2, cur_x) - 1] = (cur_x_gray + cur_y_gray) / 2;
		pgm_image->image[(int)BOUNDARY_Y - min(SIZE_Y/2, cur_y) - 1][START_X + SIZE_X - min(SIZE_X/2, cur_x) - 1] = (cur_x_gray + cur_y_gray) / 2;




		// Booleans for determining when to move to the right or down by increasing our position variables
		bool inc_y = false;
		bool inc_x = false;

		// If the percentage that we have traversed y is less than or equal to the percentage that
		// we have traversed x, than we want to move down since we have more vertical space to 
		// finish traversing than horizontal
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) <= (cur_x + 1.0) / (BOUNDARY_X - 1)){
			inc_y = true;

			// Vertical triangles, moving across when we know that x boundary is finalized
			for(int j = START_X + cur_x + 1; j < BOUNDARY_X -  min(SIZE_X, cur_x) - 1; j++){



				pgm_image->image[START_Y + min(SIZE_Y, cur_y)][j] = cur_y_gray;	
				pgm_image->image[(int)BOUNDARY_Y - cur_y - 1][j] = cur_y_gray;	



			}

			// Alter the colour of the next set of pixels in the top and bottom triangles
			// The minimum value these triangles can have is 0
			cur_y_gray = max(0, cur_y_gray - STEP_Y);
		} 

		// If the percentage that we have traversed y is greater than or equal to the percentage that
		// we have traversed x, than we want to move right since we have more horizontal space to 
		// finish traversing than vertical
		if ((cur_y + 1.0) / (BOUNDARY_Y - 1) >= (cur_x + 1.0) / (BOUNDARY_X - 1)) {
			inc_x = true;

			// Side-ways triangles, moving down when we know that y boundary is finalized
			for(int j = START_Y + cur_y + 1; j <= BOUNDARY_Y -  min(SIZE_Y, cur_y) - 1; j++){



				pgm_image->image[j][START_X + min(SIZE_X, cur_x)] = cur_x_gray;	
				pgm_image->image[j][(int)BOUNDARY_X - cur_x - 1] = cur_x_gray;	



			}

			// Alter the colour of the next set of pixels in the left and right triangles
			// The minimum value these triangles can have is 0
			cur_x_gray = max(0, cur_x_gray - STEP_X);
		} 

		// Perform the movements by increasing the position variables
		if(inc_y){
			cur_y++;
		}
		if(inc_x){
			cur_x++;
		}
	}

	// for(int i = 0; i < height; i++){
	// 	for(int j = 0; j < width; j++){
	// 		printf(" %4d", pgm_image->image[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// Save the image and clear allocated memory
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
