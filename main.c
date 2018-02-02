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
	// int height_inner = height / 2;
	// int width_inner = width / 2;
	
	// // Determining the greatest common divisor in order to reduce a single step of the cross
	// int gcd = 1;
	// int max_gcd = height_inner > width_inner ? width_inner : height_inner;
	// for(int i = 2; i <= max_gcd; i++){
	// 	if(height_inner % i == 0 && width_inner % i == 0){
	// 		gcd = i;
	// 	}
	// }

	// int vertical = height_inner/(gcd);
	// int horizontal = width_inner/(gcd);

	// printf("%d\t%d\t%d\n", gcd, vertical, horizontal);

	const int START_Y = (int)(0.25 * height);
	const int START_X = (int)(0.25 * width);

	const float BOUNDARY_Y = 0.75 * height;
	const float BOUNDARY_X = 0.75 * width;

	int cur_y = START_Y;
	int cur_x = START_X;

	while(cur_y < BOUNDARY_Y || cur_x < BOUNDARY_X){

		pbm_image->image[cur_y][cur_x] = 3;					
		// pbm_image->image[(int)BOUNDARY_Y - (cur_y - START_Y)][(int)BOUNDARY_X - (cur_x - START_X)] = 3;					

		if ((cur_y + 1.0) / BOUNDARY_Y <= (cur_x + 1.0) / BOUNDARY_X){
			cur_y++;
		} 

		if ((cur_y + 1.0) / BOUNDARY_Y >= (cur_x + 1.0) / BOUNDARY_X) {
			cur_x++;
		} 
	}


	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			printf(" %d", pbm_image->image[i][j]);
		}
		printf("\n");
	}

	save_PBM_Image(pbm_image, image_name, image_format);
	free_PBM_Image(pbm_image);

}