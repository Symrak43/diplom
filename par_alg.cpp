#include <iostream>
#include <mpi.h>
#include <cmath>
#include <limits.h>
#include <cstdlib> // для функций rand() и srand()
#include <ctime> // для функции time()

using namespace std;


void InitMas(int **graph, int n, int RangeMin)
{
	srand(time(0));
	int randRange = 0;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			graph[i][j] = 0;
	// random

	for (int i = 0; i < n; i++)
	{		
		for (int j = 0; j < n; j++)
		{	
			if(graph[i][j] == 0)
			{
				randRange = RangeMin + rand() % 30;
				graph[i][j] = randRange;
				graph[j][i] = graph[i][j];
			}	
		}
		graph[i][i] = 0;
	}
		
	/*
	//задаем граф сами
	int test_graph[5][5] = {
		{0, 2, 8, 4, 5},
		{2, 0, 2, 1, 3},
		{8, 2, 0, 6, 8},
		{4, 1, 6, 0, 7},
		{5, 3, 8, 7, 0}
	};
	


	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			graph[i][j] = test_graph[i][j];
	*/
	/*
	cout << "\n";;
	cout << "Исходная матрица:\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << graph[i][j] << " ";
		cout << "\n";
	}
	cout << "\n";
	*/
}
	

void printMST(int b[], int n, int **result, int V)
{
    int i;
    printf("\nВывод минимального MST:");
    printf("\nРебро   Вес\n");
    for (i = 1; i < V; i++)
        printf("%d - %d    %d \n", b[i], i, result[i][b[i]]);
}

int main(int argc, char *argv[])
{
	int n = 25; //номер узла
	int y;
	int **graph;
	int **result;

	int* used;
	int* b;

	double start, end1;
	int rank, size; //размер группы и номер вызывающей задачи
	float h;

	int RangeMin = 5; //минимальная дальность 

	MPI_Init(&argc, &argv); //инициализация
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //порядковый номер вызывающей задачи
	MPI_Comm_size(MPI_COMM_WORLD, &size); //сообщают размер группы (то есть общее количество задач, подсоединенных к ее области связи)
	/*
	if(rank == 0)
	{
		cout << "Input min Range \n";
		cin >> RangeMin;

		cout << "Input number of nodes \n";
		cin >> n;
	}
	*/
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

	graph = new int* [n];
	for (int i = 0; i < n; i++)
		graph[i] = new int [n];

	b = new int [n];
	for (int i = 0; i < n; i++)
		b[i] = 0;

	if(rank == 0)
	{
		result = new int* [n];
		for(int i = 0; i < n; i++)
			result[i] = new int [n];

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				result[i][j] = 0;

		InitMas(graph, n, RangeMin);

	}

	for(int i = 0; i < n; i++)
		MPI_Bcast(graph[i], n, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Bcast(&RangeMin, 1, MPI_INT, 0, MPI_COMM_WORLD);

	used = new int[n];
	used[0] = 1;
	for (int i = 1; i < n; i++)
		used[i] = 0;

	h = n / (float)size;
	int p[3];

	MPI_Barrier(MPI_COMM_WORLD);
	start = MPI_Wtime();

	for (int k = 1; k < n; k++)
	{
		p[2] = INT_MAX;
		for(int i = ceil(rank * h); i < (rank + 1) * h; i++)
			if(used[i] != 1)
				if( (graph[i][b[i]]) && (graph[i][b[i]] < p[2]))
				{
					p[0] = i;
					p[1] = b[i];
					p[2] = graph[i][b[i]];
				}

			if (rank != 0)
			{
				MPI_Send(p, 3, MPI_INT, 0, 6, MPI_COMM_WORLD);
				MPI_Recv(&p[0], 1, MPI_INT, MPI_ANY_SOURCE, 13, MPI_COMM_WORLD, &status);
				y = 2;
				used[p[0]] = 1;
			}
			else
			{
				for (int i = 1; i < size; i++)
				{
					int d[3];
					MPI_Recv(d, 3, MPI_INT, MPI_ANY_SOURCE, 6, MPI_COMM_WORLD, &status);
					if (d[2] > p[2])
					{
						p[0] = d[0];
						p[1] = d[1];
						p[2] = d[2];
					}
				}

				used[p[0]] = 1;
				result[p[0]][p[1]] = graph[p[0]][p[1]];
				result[p[1]][p[0]] = graph[p[1]][p[0]];

				for (int i = 1; i < size; i++)
					MPI_Send(&p[0], 1, MPI_INT, i, 13, MPI_COMM_WORLD);
			}

			for (int i = ceil(rank * h); i < (rank + 1) * h; i++)
				if(used[i] != 1)
					if( (graph[i][p[0]]) && (graph[i][p[0]] < graph[i][b[i]]) )
						b[i] = p[0];
	}

	MPI_Barrier(MPI_COMM_WORLD);
	double end = MPI_Wtime();

	if (rank == 0)
	{
		double time = end - start;

		cout << "\n";
		/*
		cout << "Вывод результативной матрицы:\n";
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
				cout << result[i][j] << " ";
			cout << "\n";
		}
		*/
		printMST(b, n, result, n);
		printf("\nВремя: %f \n", time);
		/*
		cout << "\n";
		char let;
		cin >> let;
		*/
		delete result;
	}

	delete graph, used;
	MPI_Finalize();

}