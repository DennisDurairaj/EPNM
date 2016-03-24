#include <mpi.h>
#include <stdio.h>

#define SIZE 8

void inputData(int* nrows, int* ncols) {
	int rows;
	printf("%s\n", "Enter number of rows and columns for the matrix");
	scanf("%d", rows);
	*nrows = rows;
	*ncols = rows;
}

int fillMatrix(int nrows, int ncols, int mat[nrows][ncols]) {
	// printf("%s\n", "Matrix");
	int count = 1;
	for (int i = 0; i < nrows; i++) {
		for (int j = 0; j < ncols; j++) {
			mat[i][j] = count++;
		}
	}
	return mat[nrows][ncols];
}

int fillVector(int nrows, int vec[nrows]) {
	int count = 1;
	// printf("%s\n","Vector");
	for (int i = 0; i < nrows; i++) {
		vec[i] = count++;
	}
	return vec[nrows];
}

void printMatrix(int nrows, int ncols, int mat[nrows][ncols]) {
	for (int i = 0; i < nrows; ++i) {
		for (int j = 0; j < ncols; ++j) {
			printf("%d ",mat[i][j]);
		}
		printf("\n");
	}
}

void printVector(int nrows, int vec[nrows]) {
	for (int i = 0; i < nrows; ++i) {
		printf("%d ", vec[i]);
	}
	printf("\n");
}

void calcMult(int nrows, int ncols, int mat[nrows][ncols], int vec[nrows], int result[nrows], int begin, int end) {
	for (int i = begin; i < end; ++i) {
		for (int j = 0; j < nrows; ++j) {
			result[i] += mat[i][j]*vec[j];
		}
	}
}

int main(int argc, char *argv[]) {
	int nrows = SIZE;
	int ncols = SIZE;
	int result[SIZE] = {0};
	int mult[SIZE] = {0};
	int mat[nrows][ncols];
	int vec[nrows];

	int myid, nprocs;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	int begin = myid * nrows/nprocs;
	int end = (myid+1) * nrows/nprocs;

	if (myid == 0) {
		mat[nrows][ncols] = fillMatrix(nrows, ncols, mat);
		vec[nrows] = fillVector(nrows, vec);
	}

	MPI_Bcast(vec, nrows, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(mat, nrows*ncols/nprocs, MPI_INT, mat[begin], nrows*ncols/nprocs, MPI_INT, 0, MPI_COMM_WORLD);

	calcMult(nrows, ncols, mat, vec, result, begin, end);

	MPI_Gather(&result[begin], nrows/nprocs, MPI_INT, &mult[begin], nrows/nprocs, MPI_INT, 0, MPI_COMM_WORLD);

	if (myid == 0) {
		printVector(nrows, vec);
		printVector(nrows, mult);
	}

	MPI_Finalize();
	return 0;
}