#include "bmp.h"

// Path to a BMP file 
const char* filename = "C://Users//1//Desktop//VS_C_C++//C//ParallelProgramming//sample.bmp";

// Global task number (R*G*B < N)
const int   N = 1000;

int main(int argc, char *argv[]) {
	BMP_Image *img = NULL;

	img = BMP_open(filename);
	if (img == NULL) {
		printf("Error reading image");
		return 1;
	}
	printf("Image read successfully");

	int pxl;
	unsigned int bytes_per_pixel = img->bytes_per_pixel; // Bpp
	unsigned char *data = img->data;

	// Counter
	int count = 0;

	// Moving forward on picture with step = 1 pixels (Bpp bytes)
	for (pxl = img->header.offset - sizeof(BMP_Header); pxl < (img->data_size); pxl += bytes_per_pixel) {
		unsigned char red	= data[pxl + 2];
		unsigned char green = data[pxl + 1];
		unsigned char blue	= data[pxl];
		// Counting pixels, for which R*G*B is lesser than given N
		if (red * green * blue < 1000) {
			count++;
		}
	}
	printf("\nCount:\t%d\n", count);

	return 0;
}
