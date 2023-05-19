#include "bmp.h"
#include <thread>
#include <mutex>
#include <windows.h>
#include <stdbool.h>

// Variant's numbers
const int A = 2;
const int B = 9;

// Global task number (R*G*B < N)
const int N = 1000;

// Path to a BMP file
const char *filename = "C://Users//1//Desktop//VS_C_C++//C//ParallelProgramming//sample.bmp";

std::mutex mtx;
BMP_Image *img = NULL;

// Counter
int count = 0;

struct TASK
{
	// Index of start pixel (should be multiplied by bytes per pixel) 
	int start;
};

// Function to count pixels with R*G*B < N in a thread
DWORD __stdcall countPixels(void* arg) {
	// extern int count;
	int		ans = 0;
	int		pxl;
	TASK*	t = (TASK*)arg;

	unsigned int	bytes_per_pixel = img->bytes_per_pixel; // Bpp
	unsigned char*	data = img->data;

	const int start_of_data = img->header.offset - sizeof(BMP_Header);

	// Moving forward on picture with step = B pixels (B*Bpp bytes)
	for (pxl = start_of_data + t->start * bytes_per_pixel; pxl < img->data_size; pxl += B * bytes_per_pixel) {
		unsigned char red = data[pxl + 2];
		unsigned char green = data[pxl + 1];
		unsigned char blue = data[pxl];
		// Counting pixels, for which R*G*B is lesser than given N
		if (red * green * blue < N) {
			ans++;
		}
	}
	mtx.lock();
	printf("\nThread %d:\t%d pixels\n", t->start + 1, ans);
	count += ans;
	mtx.unlock();

	return 0;
};

int main(int argc, char *argv[]) {
	std::thread	threads[B];
	TASK	tasks[B];

	img = BMP_open(filename);
	if (img == NULL) {
		printf("Error reading image");
		return 1;
	}
	printf("Image read successfully");

	{
		// Creating threads to do task in every of them
		for (int i = 0; i < B; i++)
		{
			tasks[i].start = i;
			threads[i] = std::thread(countPixels, &tasks[i]);
		}
		// Waiting for all threads are done with its' task
		for (int i = 0; i < B; i++)
		{
			threads[i].join();
		}
	}

	printf("\nCount:\t%d", count);

	return 0;
}