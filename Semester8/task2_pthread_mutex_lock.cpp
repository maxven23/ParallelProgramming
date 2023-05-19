#include "bmp.h"
#include <pthread.h>
#include <stdlib.h>

// Variant's numbers
const int A = 2;
const int B = 9;

// Global task number (R*G*B < N)
const int N = 1000;

// Path to a BMP file
const char *filename = "img01.bmp";

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
BMP_Image *img = NULL;

// Counter
int count = 0;

struct TASK
{
	// Index of start pixel (should be multiplied by bytes per pixel) 
	int start;
};

// Function to count pixels with R*G*B < N in a thread
void* countPixels(void* arg) {
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

	pthread_mutex_lock(&mtx);
	printf("\nThread %d:\t%d pixels\n", t->start + 1, ans);
	count += ans;
	pthread_mutex_unlock(&mtx);

	return 0;
};

int main(int argc, char *argv[]) {
	pthread_t   threads[B];
	int         result_code[B];
	TASK	    tasks[B];

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
			result_code[i] = pthread_create(&threads[i], NULL, &countPixels, &tasks[i]);
		}
		// Waiting for all
		for (int i = 0; i < B; i++)
		{
			pthread_join(threads[i], NULL);
		}
	}

	printf("\nAll threads:\t%d pixels", count);

	return 0;
}