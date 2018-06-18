#include <iostream>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
	int thread_count = strtol(argv[1], NULL, 10);
	int convfilas = 5;
	int convcolumnas = 5;

	int filas = 2000;
	int columnas =2000;

	//Matriz de convolucion
	int **convolucion = new int*[convfilas];
	for (int i = 0; i < filas; i++)
	{
		convolucion[i] = new int[convcolumnas];
	}
	for (int i = 0; i < convfilas; i++)
	{
		for (int j = 0; j < convcolumnas; j++)
		{
			convolucion[i][j] = 1;
		}
	}

	//Matriz de imagen
	int **matriz = new int*[filas];
	for (int i = 0; i < filas; i++)
	{
		matriz[i] = new int[columnas];
	}

	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			matriz[i][j] = i;
		}
	}

	//Resultado
	float **resultado = new float*[filas];
	for (int i = 0; i < filas; i++)
	{
		resultado[i] = new float[columnas];
	}
	
	
	double start = omp_get_wtime();
	//Aplicando la matriz de convolucion
	#pragma omp parallel for num_threads(thread_count)
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			int valor = 0;
			int count = 0;
			if((i>1 && i<filas-2) && (j>1 && j<columnas-2))
			{
				count = 25;
				for (int ci = 0; ci < convfilas; ci++)
				{
					for (int cj = 0; cj < convcolumnas; cj++)
					{
						int ii = i - 2 + ci;
						int jj = j - 2 + cj;
						valor += matriz[ii][jj] * convolucion[ci][cj];
					}
				}
			}
			else
			{
				for (int ci = 0; ci < convfilas; ci++)
				{
					for (int cj = 0; cj < convcolumnas; cj++)
					{
						int ii = i - 2 + ci;
						int jj = j - 2 + cj;
						if ((ii >= 0 && jj >= 0) && (ii < filas && jj < columnas))
						{
							++count;
							valor += matriz[ii][jj] * convolucion[ci][cj];
						}
					}
				}
			}
			resultado[i][j] = valor/count;
		}
	}
	cout << "CONVOLUCIÓN " << omp_get_wtime()-start << " SEGUNDOS" << endl;
	


	return 0;
	
}