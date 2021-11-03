#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

void mergesort(int arr[],int p,int f);
void mergeparallel(int arr[],int p,int f,int stop);
void merge(int vet[],int p,int m,int f);

const int max_threads = 256;
const int stopmax = 32;


/*
 * A ideia do codigo eh criar 2 threads a cada recursao que eh chamada,
 * e jogar cada metade para uma das threads. Quando o numero de recursoes
 * chamadas for igual ao numero maximo de threads, eh porque o codigo
 * comecou a ficar sequencial
 */

void swap(int *a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}


int partition(int *a, int left,int right,int pivo)
{
    int pos, i;
    swap(a, pivo, right);
    pos = left;
    for(i = left; i < right; i++)
    {
        if (a[i] < a[right])
        {
            swap(a, i, pos);
            pos++;
        }
    }
    swap(a, right, pos);
    return pos;
}

void quickParallel(int *a, int left, int right, int stop)
{
    if (left < right)
    {
        int pivo = (left + right) / 2;
        int pos = partition(a,left,right,pivo);
        if (stop > 1) {     // Se stop > 1, eh pq ainda estamos lidando com uma chamada paralela
          #pragma omp parallel sections
          {
            #pragma omp section
            quickParallel(a, left, pos - 1, stop/2);
            #pragma omp section
            quickParallel(a, pos + 1, right, stop/2);
          }
        }
        else {              // A partir daqui, o numero de chamadas recursivas eh igual ao de threads, o que faz o codigo ser sequencial
          quickParallel(a, left, pos - 1, stop);
          quickParallel(a, pos + 1, right, stop);
        }
    }
}


void quickSequential(int *a, int left, int right)
{
    if (left < right)
    {
        int pivo = (left + right) / 2;
        int pos = partition(a,left,right,pivo);

        quickSequential(a, left, pos - 1);
        quickSequential(a, pos + 1, right);
    }
}


void mergeparallel(int arr[],int p,int f,int stop)
{
    if(p<f)
    {
        int m = (f+p)/2;
        if(stop > 1)
        {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeparallel(arr,p,m,stop/2);
                #pragma omp section
                mergeparallel(arr,m+1,f,stop/2);
            }
        }
        else
        {
            mergeparallel(arr,p,m,stop);
            mergeparallel(arr,m+1,f,stop);
        }
        merge(arr,p,m,f);
    }
}

void mergesort(int arr[],int p,int f)
{
    if(p<f)
    {
        int m = (f+p)/2;
        mergesort(arr,p,m);
        mergesort(arr,m+1,f);
        merge(arr,p,m,f);
    }
}

void merge(int vet[],int p,int m,int f)
{
    int com1 = p, com2 = m+1, comAux = 0, tam = f-p+1;
    int *vetAux;
    vetAux= (int*)malloc(tam*sizeof(int));

    while(com1 <= m && com2 <= f)
    {
        if(vet[com1] < vet[com2])
        {
            vetAux[comAux] = vet[com1];
            com1++;
        }
        else
        {
            vetAux[comAux] = vet[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= m)
    {
        vetAux[comAux] = vet[com1];
        comAux++;
        com1++;
    }

    while(com2 <= f)
    {
        vetAux[comAux] = vet[com2];
        comAux++;
        com2++;
    }

    for(comAux = p;comAux <= f; comAux++)
    {
        vet[comAux] = vetAux[comAux-p];
    }

    free(vetAux);
}

///Mergesort////////////////////////////////////////////////////////

int IsSort(int *array, int size) {
	int i;
	for(i = 1; i < size; i++)
		if(array[i-1] > array[i])
			return 0;
	return 1;
}

void shuff(int arr[],int size)
{
    srand(time(NULL));
    for (int i = 0; i < size; i++)
		arr[i] = rand()%size;
}


int main(int argc, char** argv) {
	int size = 1000000, *array;

	array = malloc(size* sizeof(int));
	double start, end;
	int MAX_THREADS = max_threads;     // Caution!

	for (int threads =  1; threads <= MAX_THREADS; threads *= 2) {
        omp_set_num_threads(threads);   // threads equals to 1 should be sequential...
        for (int nested = 0; nested <= 1; nested++) {  // false or true
            omp_set_nested( nested );
            for (int stop = 2; stop <= stopmax; stop *= 2) {
                shuff(array,size);
                start = omp_get_wtime();
                quickParallel(array, 0, size - 1, stop);
                end = omp_get_wtime();

                printf("Quicksort\n");
                printf("Tempo: %.3f threads: %d  nested: %d stop: %d\n", end - start, threads, nested, stop);
                if(IsSort(array, size) == 1)
                    printf("Result: Sorted\n");
                else
                    printf("Result: Not Sorted\n");
                
                shuff(array,size);
                start = omp_get_wtime();
                quickSequential(array, 0, size - 1);
                end = omp_get_wtime();

                printf("Quicksort sequencial\n");
                printf("Tempo: %.3f threads: %d  nested: %d stop: %d\n", end - start, threads, nested, stop);
                if(IsSort(array, size) == 1)
                    printf("Result: Sorted\n");
                else
                    printf("Result: Not Sorted\n");
            }
        }
	}

    printf("===========================================================\n");
    printf("===========================================================\n");
    printf("===========================================================\n");

    MAX_THREADS = max_threads;

	for(int threads = 1; threads <= MAX_THREADS; threads *= 2)
	{
		omp_set_num_threads(threads);
		for(int nested = 0; nested <= 1;nested++)
		{
			omp_set_nested(nested);
			for(int stop = 2; stop <= stopmax; stop *= 2)
			{
                shuff(array,size);
				start = omp_get_wtime();
				mergeparallel(array,0,size-1,stop);
				end = omp_get_wtime();

                printf("Mergesort\n");
				printf("Tempo: %.3f threads: %d nested: %d stop: %d\n",end-start,threads,nested,stop);
				if(IsSort(array,size)==1)
					printf("Result: Sorted\n");
				else
					printf("Result: Not Sorted\n");

                shuff(array,size);
				start = omp_get_wtime();
				mergesort(array,0,size-1);
				end = omp_get_wtime();

                printf("Mergesort sequencial\n");
				printf("Tempo: %.3f threads: %d nested: %d stop: %d\n",end-start,threads,nested,stop);
				if(IsSort(array,size)==1)
					printf("Result: Sorted\n");
				else
					printf("Result: Not Sorted\n");
			}
		}
	}
	return 0;
}

/*Área de respostas

Se rodar o arquivo como >>saida vai montar a tabela

Tamanho do vetor: 1000000 unidades

resultados:

Os melhores resultados para ambos foi entre 0.040 e 0.050 segundos
quando as threads estavam como 2, o atributo nested nÃ£o influenciou
significativamente o resultado.

*/

