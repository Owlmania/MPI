#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<mpi.h>

int main(int argc,char** argv) 
{
	long i, n;
	double sum, step, pi, x, tsum;
	int cont;
	int myrank, nprocs, ierr, is, ie;

	// � ���� �����ϴ°�?
	MPI_Status status;
	cont = 1; ierr = MPI_Init(&argc, &argv);

	/*
		���μ����� ���� Ŀ�´�Ƽ �ȿ� �־�� �޽����� ��/���� �� �� �ִ�.
		�ڵ忡�� � �κ��� Ŀ�´�Ƽ�� �����ϴ°�?
	*/

	// MPI_Comm_size �� �ǹ̴�?
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	while (cont) 
	{
		if (myrank == 0) 
		{
			printf("Enter the Number of Intervals : (n < 1 :quits)\n");
			scanf_s("%ld", &n);

			for (i = 1; i < nprocs; i++) 
			{
				ierr = MPI_Send(&n, 1, MPI_LONG, i, 1, MPI_COMM_WORLD);
			}
		}
		else
		{
			ierr = MPI_Recv(&n, 1, MPI_LONG, 0, 1, MPI_COMM_WORLD, &status);
		}

		if (n <= 0) 
		{
			cont = 0;
			break;
		}

		step = 1.0 / (double)n;
		sum = 0.0;
		tsum = 0.0;
		for (i = myrank; i < n; i = i + nprocs) 
		{
			x = ((double)i + 0.5)*step;
			sum = sum + 4.0 / (1.0 + x*x);
		}
		
		if (myrank == 0)
		{
			tsum = sum;
			for (i = 1; i < nprocs; i++)
			{
				ierr = MPI_Recv(&sum, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
				tsum = tsum + sum;
			}
			pi = step * tsum;
			printf("Pi = %.15f", pi, fabs(acos(-1.0) - pi));
			printf("-----------------------------\n");
		}
		else 
		{
			ierr = MPI_Send(&sum, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
		}
	}
	ierr = MPI_Finalize();
}