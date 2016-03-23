#include <mpi.h>
#include <stdio.h>

int fillMatrix(int nrows, int ncols, int mat[nrows][ncols]) {
	// printf("%s\n", "Matrix");
	int count = 1;
	for (int i = 0; i < nrows; i++)
	{
		for (int j = 0; j < ncols; j++)
		{
			mat[i][j] = count++;
			// printf("%d ",mat[i][j]);
		}
	// printf("\n");
	}
	return mat[nrows][ncols];
}

int fillVector(int nrows, int vec[nrows]) {
	int count = 1;
	// printf("%s\n","Vector");
	for (int i = 0; i < nrows; i++)
	{
		vec[i] = count++;
		// printf("%d ",vec[i]);
	}
	// 	printf("\n");
	return vec[nrows];
}

void printMatrix(int nrows, int ncols, int mat[nrows][ncols]) {
	for (int i = 0; i < nrows; ++i)
	{
		for (int j = 0; j < ncols; ++j)
		{
			printf("%d ",mat[i][j]);
		}
		printf("\n");
	}
}

void printVector(int nrows, int vec[nrows]) {
	for (int i = 0; i < nrows; ++i)
	{
		printf("%d ", vec[i]);
	}
	printf("\n");
}

void calcMult(int nrows, int ncols, int mat[nrows][ncols], int vec[nrows], int result[nrows], int begin, int end) {
	for (int i = begin; i < end; ++i)
	{
		for (int j = 0; j < nrows; ++j)
		{
			result[i] += mat[i][j]*vec[j];
		}
	}
}

int main(int argc, char *argv[]) {
	int nrows = 8;
	int ncols = 8;
	int result[8] = {0};
	int mult[8] = {0};
	int mat[nrows][ncols];
	int vec[nrows];
	// int tag = 666;
	// mat[8][8] = fillMatrix(nrows, ncols, mat);
	// vec[8] = fillVector(ncols, vec);
	// calcMult(mat, vec, mult);

	int myid, nprocs;
	MPI_Init (&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	int begin = myid * nrows/nprocs;
	int end = (myid+1) * nrows/nprocs;

	if (myid == 0) {
		mat[nrows][ncols] = fillMatrix(nrows, ncols, mat);
		vec[nrows] = fillVector(nrows, vec);
		// MPI_Send(mat, 8*8/nprocs, MPI_INT, 1, tag, MPI_COMM_WORLD);
		// for (int i = begin; i < end; ++i)
		// {
		// 	for (int j = 0; j < 8; ++j)
		// 	{
		// 		result[i] += mat[i][j]*vec[j];
		// 	}
		// }
		// MPI_Recv(result, 8/nprocs, MPI_INT, 1, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	// if (myid == 1) {
	// 	MPI_Recv(mat, 8*8/nprocs, MPI_INT, 0, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	// 	for (int i = begin; i < end; ++i)
	// 	{
	// 		for (int j = 0; j < 8; ++j)
	// 		{
	// 			result[i] += mat[i][j]*vec[j];
	// 		}
	// 	}
	// 	MPI_Send(result, 8/nprocs, MPI_INT, 0, tag, MPI_COMM_WORLD);
	// }

	MPI_Bcast(vec, nrows, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(mat, nrows*ncols/nprocs, MPI_INT, mat[begin], nrows*ncols/nprocs, MPI_INT, 0, MPI_COMM_WORLD);

	calcMult(nrows, ncols, mat, vec, result, begin, end);
	// for (int i = begin; i < end; i++) {
	// 	for (int j = 0; j < 8; j++) {
	// 		printf("%d ",mat[i][j] );
	// 	}
	// }

	MPI_Gather(&result[begin], nrows/nprocs, MPI_INT, &mult[begin], nrows/nprocs, MPI_INT, 0, MPI_COMM_WORLD);

	if (myid == 0)
	{
		printVector(nrows, vec);
		printVector(nrows, mult);

	}
		// printf("\n\n");
		// int count = 0;
		// for (int i=0; i < 8; i++) {
		// 	printf("%d ", result[i]);
		// 	count++;
		// }
		// printf("\n%d\n", count);
		// printMatrix(mat);

	MPI_Finalize();
	return 0;
}