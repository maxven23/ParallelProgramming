//#include "bmp.h"
//#include <thread>
//#include <stdbool.h>
//#include <omp.h>
//
//// Variant's numbers
//const int A = 2;
//const int B = 9;
//
//// Global task number (R*G*B < N)
//const int N = 1000;
//
//// Path to a BMP file
//const char *filename = "C://Users//1//Desktop//VS_C_C++//C//ParallelProgramming//sample.bmp";
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
//int __stdcall countPixels(void* arg) {
//	// extern int count;
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
//int main(int argc, char *argv[]) {
//	TASK	tasks[B];
//	int ans[B];
//
//	img = BMP_open(filename);
//	if (img == NULL) {
//		printf("Error reading image");
//		return 1;
//	}
//	printf("Image read successfully");
//
//	{
//		// Creating processes to do task in every of them
//		#pragma omp parallel num_threads(B)
//		{
//			int i = omp_get_thread_num();
//			tasks[i].start = i;
//			int c = countPixels(&tasks[i]);
//			#pragma omp critical
//			{
//				printf("\nThread %d:\t%d pixels\n", i + 1, c);
//				ans[i] = c;
//				
//			}
//		}
//		for (int j = 0; j < B; j++) {
//			count += ans[j];
//		}
//	}
//
//	printf("\nTotal count:\t%d pixels", count);
//
//	return 0;
//}