#include "bmp.h"
#include "mpi.h"

// Variant's numbers
const int A = 2;
const int B = 9;

// Global task number (R*G*B < N)
const int N = 1000;

// Path to a BMP file
const char *filename = "C://Users//1//Desktop//VS_C_C++//C//ParallelProgramming//sample.bmp";

BMP_Image *img = NULL;

// Counter
int count = 0;

struct TASK
{
	// Index of start pixel (should be multiplied by bytes per pixel) 
	int start;
};

 // Function to count pixels with R*G*B < N in a thread/process
int __stdcall countPixels(void* arg) {
	int		ans = 0;
	int		pxl;
	TASK*	t = (TASK*)arg;

	unsigned int	bytes_per_pixel = img->bytes_per_pixel; // Bpp
	unsigned char*	data = img->data;

	const int start_of_data = img->header.offset - sizeof(BMP_Header);

	// Moving forward on picture with step = B pixels (B*Bpp bytes)
	for (pxl = start_of_data + t->start * bytes_per_pixel; pxl < img->data_size; pxl += B * bytes_per_pixel) {
		unsigned char red	= data[pxl + 2];
		unsigned char green = data[pxl + 1];
		unsigned char blue	= data[pxl];
		// Counting pixels, for which R*G*B is lesser than given N
		if (red * green * blue < N) {
			ans++;
		}
	}

	return ans;
};

int main(int argc, char **argv) {
	int rank, i, res;

	MPI_Status status;
	MPI_Request req[B];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		for (int i = 0; i < B; i++)
		{
			int start = i;
			MPI_Isend(&start, 1, MPI_INT, i+1, 111, MPI_COMM_WORLD, &req[i]);
		}
		MPI_Barrier(MPI_COMM_WORLD);

		int rres[B];
		for (i = 0; i < B; i++) {
			MPI_Recv(&res, 1, MPI_INT, MPI_ANY_SOURCE, 222, MPI_COMM_WORLD, &status);
			int index = status.MPI_SOURCE-1;
			//printf("Process 0 recieved Res from %d.", status.MPI_SOURCE);
			rres[index] = res;
			printf("\nProcess %d:\t%d pixels\n", index + 1, rres[index]);
		}

		for (i = 0; i < B; i++) {
			count += rres[i];
		}

		printf("\nTotal count:\t%d pixels", count);
	}
	else {
		img = BMP_open(filename);
		if (img == NULL) {
			printf("Error reading image");
			return 1;
		}
		//printf("Image read successfully");
		//printf("Process %d started.", rank);

		int start;
		MPI_Irecv(&start, 1, MPI_INT, 0, 111, MPI_COMM_WORLD, &req[rank-1]);
		
		//printf("Process %d recieved Start number.", rank);
		MPI_Barrier(MPI_COMM_WORLD);
		TASK task;
		task.start = start;

		int res = countPixels(&task);

		MPI_Send(&res, 1, MPI_INT, 0, 222, MPI_COMM_WORLD);
		//printf("Process %d sent Res number.", rank);
	}

	MPI_Finalize();
	return 0;
}