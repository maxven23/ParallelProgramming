//#include "bmp.h"
//#include <unistd.h>
//#include <iostream>
//#include <fstream>
//
//// Variant's numbers
//const int A = 2;
//const int B = 9;
//
//// Global task number (R*G*B < N)
//const int N = 1000;
//
//// Path to a BMP file
//const char *filename = "sample.bmp";
//
//BMP_Image *img = NULL;
//
//// Counter
//int count = 0;
//
//struct TASK
//{
//	// Index of start pixel (should be multiplied by bytes per pixel) 
//	int start;
//};
//
//// Function to count pixels with R*G*B < N in a thread/process
//int countPixels(void* arg) {
//	int		ans = 0;
//	int		pxl;
//	TASK*	t = (TASK*)arg;
//
//	unsigned int	bytes_per_pixel = img->bytes_per_pixel; // Bpp
//	unsigned char*	data = img->data;
//
//	const int start_of_data = img->header.offset - sizeof(BMP_Header);
//
//	// Moving forward on picture with step = B pixels (B*Bpp bytes)
//	for (pxl = start_of_data + t->start * bytes_per_pixel; pxl < img->data_size; pxl += B * bytes_per_pixel) {
//		unsigned char red = data[pxl + 2];
//		unsigned char green = data[pxl + 1];
//		unsigned char blue = data[pxl];
//		// Counting pixels, for which R*G*B is lesser than given N
//		if (red * green * blue < N) {
//			ans++;
//		}
//	}
//
//	return ans;
//};
//
//int main(int argc, char **argv) {
//	TASK task;
//	int  thread = -1;
//
//	img = BMP_open(filename);
//	if (img == NULL) {
//		printf("Error reading image");
//		return 1;
//	}
//	//printf("Image read successfully");
//
//	for (int i = 0; i < B; i++)
//	{
//		if (fork()) {
//			thread = i;
//			break;
//		}
//	}
//
//	if (thread == -1) {
//		int rres;
//		for (int i = 0; i < B; i++) {
//			char fn[100];
//			sprintf(fn, "%d.txt", i);
//
//			FILE *f = fopen(fn, "r");
//			fread(&rres, sizeof(int), 1, f);
//			fclose(f);
//
//			printf("\nProcess %d:\t%d pixels\n", i + 1, rres);
//			count += rres;
//		}
//		printf("\nTotal count:\t%d pixels\n", count);
//	}
//	else {
//		task.start = thread;
//		int res = countPixels(&task);
//		//printf("Thread %d: %d pixels\n", thread+1, res);
//
//		char fn[100];
//		sprintf(fn, "%d.txt", thread);
//		FILE *f = fopen(fn, "w");
//		fwrite(&res, sizeof(int), 1, f);
//		fclose(f);
//	}
//
//	return 0;
//}