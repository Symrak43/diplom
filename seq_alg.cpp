#include <iostream>
#include <cmath>
#include <stdio.h>
#include <time.h>
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
				randRange = RangeMin + rand() % 10;
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
	int **graph;
	int **result;

	int* used;
	int* b;

	//double start;
	int RangeMin = 5; //минимальная дальность 
/*
	cout << "Input min Range \n";
	cin >> RangeMin;

	cout << "Input number of nodes \n";
	cin >> n;
*/
	graph = new int* [n];
	for (int i = 0; i < n; i++)
		graph[i] = new int [n];

	b = new int [n];
	for (int i = 0; i < n; i++)
		b[i] = 0;

	result = new int* [n];
	for(int i = 0; i < n; i++)
		result[i] = new int [n];

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			result[i][j] = 0;

	InitMas(graph, n, RangeMin);

	used = new int[n];
	used[0] = 1;
	for (int i = 1; i < n; i++)
		used[i] = 0;

	int p[3];

	clock_t start = clock();

	for (int k = 1; k < n; k++)
	{
		p[2] = INT_MAX;
		for(int i = 1; i < n; i++)
			if(used[i] != 1)
				if( (graph[i][b[i]]) && (graph[i][b[i]]) < p[2])
				{
					p[0] = i;
					p[1] = b[i];
					p[2] = graph[i][b[i]];
				}

			used[p[0]] = 1;
			result[p[0]][p[1]] = graph[p[0]][p[1]];
			result[p[1]][p[0]] = graph[p[1]][p[0]];

			for (int i = 1; i < n; i++)
				if(used[i] != 1)
					if( (graph[i][p[0]]) && (graph[i][p[0]] < graph[i][b[i]]) )
						b[i] = p[0];
	}

    clock_t end = clock();
	double time = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "\n";

	cout << "Вывод результативной матрицы:\n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << result[i][j] << " ";
		cout << "\n";
	}
	cout << "\n";

	printMST(b, n, result, n);
	printf("\nВремя: %f \n", time);
	/*
	cout << "\n";
	char let;
	cin >> let;
	*/
	delete result, graph;
}