=============================================================================================================================================================

Questao 1:

Iteracao 1
Tempo sequencial: 1.433240

Tempo paralelo: 0.606362

Speedup  = 2.363672

--------------------------------------
Iteracao 2
Tempo sequencial: 1.406820

Tempo paralelo: 0.609193

Speedup  = 2.309319

--------------------------------------
Iteracao 3
Tempo sequencial: 1.406622

Tempo paralelo: 0.632066

Speedup  = 2.225436

--------------------------------------
Iteracao 4
Tempo sequencial: 1.400980

Tempo paralelo: 0.662982

Speedup  = 2.113151

--------------------------------------
Iteracao 5
Tempo sequencial: 1.392953

Tempo paralelo: 0.605469

Speedup  = 2.300620

--------------------------------------
Iteracao 6
Tempo sequencial: 1.390758

Tempo paralelo: 0.607510

Speedup  = 2.289277

--------------------------------------
Iteracao 7
Tempo sequencial: 1.391909

Tempo paralelo: 0.605005

Speedup  = 2.300656

--------------------------------------
Iteracao 8
Tempo sequencial: 1.393184

Tempo paralelo: 0.610324

Speedup  = 2.282695

--------------------------------------
Iteracao 9
Tempo sequencial: 1.391557

Tempo paralelo: 0.600961

Speedup  = 2.315554

--------------------------------------
Iteracao 10
Tempo sequencial: 1.392955

Tempo paralelo: 0.602669

Speedup  = 2.311312

--------------------------------------

Codigo questao 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int ehPrimo(long num) {
	long divisor;                                                                
	if( num <= 1 )
		return 0;
	
	else
		if(num > 3)
		{
			if(num % 2 == 0)
				return 0;
      		long max_divisor = sqrt(num);
      	
			for(divisor = 3; divisor <= max_divisor; divisor += 2)
	    		if (num % divisor == 0)
	      			return 0;
		}
	return 1;
}

int main() {
	long LIMITE_MAX = 2000000;
	long contaPrimo;
	long soma;
	int n;
	double inicio, fim, speedup;
	double tempo_sequencial, tempo_paralelo;

	for(int i = 0;  i < 10;i++)
	{
		/*
		 *
		 * Calculando tempo sequencial
		 *
		 */

		printf("Iteracao %d\n", i+1);		

		inicio = omp_get_wtime();
		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo  =  ehPrimo(n);
				soma        =  soma + contaPrimo;
			}
  		}	
		fim               =  omp_get_wtime();
		tempo_sequencial  =  fim-inicio;
		
		printf("Tempo sequencial: %lf\n\n", tempo_sequencial);
		
		/*
		 *
		 * Calculando tempo paralelo
		 *
		 */
		
		inicio = omp_get_wtime();

		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			#pragma omp parallel for private (contaPrimo) reduction(+:soma)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo = ehPrimo(n);
				soma = soma + contaPrimo;
			}
  		}	

		fim             =  omp_get_wtime();
		tempo_paralelo  =  fim-inicio;

		printf("Tempo paralelo: %lf\n\nSpeedup  = %lf\n\n--------------------------------------\n", tempo_paralelo, tempo_sequencial/tempo_paralelo);
  	}
  	
	return 0;
}

=============================================================================================================================================================

Questao 2:

Iteracao 1
Tempo reduction: 0.672754

Tempo sessao critica: 0.605507

--------------------------------------
Iteracao 2
Tempo reduction: 0.602287

Tempo sessao critica: 0.629466

--------------------------------------
Iteracao 3
Tempo reduction: 0.599605

Tempo sessao critica: 0.652632

--------------------------------------
Iteracao 4
Tempo reduction: 0.644793

Tempo sessao critica: 0.651751

--------------------------------------
Iteracao 5
Tempo reduction: 0.644119

Tempo sessao critica: 0.652363

--------------------------------------
Iteracao 6
Tempo reduction: 0.640389

Tempo sessao critica: 0.652646

--------------------------------------
Iteracao 7
Tempo reduction: 0.659726

Tempo sessao critica: 0.659600

--------------------------------------
Iteracao 8
Tempo reduction: 0.634384

Tempo sessao critica: 0.611248

--------------------------------------
Iteracao 9
Tempo reduction: 0.597815

Tempo sessao critica: 0.612323

--------------------------------------
Iteracao 10
Tempo reduction: 0.599154

Tempo sessao critica: 0.636043

--------------------------------------


Codigo questao 2:

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int ehPrimo(long num) {
	long divisor;                                                                
	if( num <= 1 )
		return 0;
	
	else
		if(num > 3)
		{
			if(num % 2 == 0)
				return 0;
      		long max_divisor = sqrt(num);
      	
			for(divisor = 3; divisor <= max_divisor; divisor += 2)
	    		if (num % divisor == 0)
	      			return 0;
		}
	return 1;
}

int main() {
	long LIMITE_MAX = 2000000;
	long contaPrimo;
	long soma;
	int n;
	double inicio, fim, speedup;
	double tempo_reduction, tempo_sessao_critica;

	for(int i = 0;  i < 10;i++)
	{
		/*
		 *
		 * Calculando tempo com reduction
		 *
		 */

		printf("Iteracao %d\n", i+1);		

		inicio = omp_get_wtime();
		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			#pragma omp parallel for private (contaPrimo) reduction(+:soma)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo  =  ehPrimo(n);
				soma        =  soma + contaPrimo;
			}
  		}	
		fim               =  omp_get_wtime();
		tempo_reduction  =  fim-inicio;
		
		printf("Tempo reduction: %lf\n\n", tempo_reduction);
		
		/*
		 *
		 * Calculando tempo com sessao critica
		 *
		 */
		
		inicio = omp_get_wtime();

		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
            #pragma omp parallel for private (contaPrimo)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo = ehPrimo(n);
				#pragma omp critical
				soma = soma + contaPrimo;
			}
  		}	

		fim                   =  omp_get_wtime();
		tempo_sessao_critica  =  fim-inicio;

		printf("Tempo sessao critica: %lf\n\n--------------------------------------\n", tempo_sessao_critica);
  	}
  	
	return 0;
}


=============================================================================================================================================================

Questao 3:

Por causa da natureza do problema, muitas iteracoes da sessao critica terao uma carga de trabalho menor do que outras, o que faz com que a execucao seja quase instantanea. Apesar disso, algumas threads ainda terao uma carga de trabalho muito grande e terao que esperar outras threads por causa da sessao critica, o que faz com que o tempo de execucao do codigo com a sessao critica ainda seja maior do que usando a reducao.

==============================================================================================================================================

Questao 4:

Numero de execucoes:        100
Tempo medio nas execucoes:  0.8908609407 
LIMITE_MAX:                 2.000.000

Politica           |      static         |    dynamic       |    guided         |
Tempo medio        |      0.9315357204   |    0.8668543339  |    0.8741927677   |



Numero de execucoes:        100
Tempo medio nas execucoes:  2.2869358910
LIMITE_MAX:                 4.000.000 

Politica           |      static         |    dynamic       |    guided         |
Tempo medio        |     2.3859206051    |   2.2319110892   |    2.2429759786   |

Codigo:

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int ehPrimo(long num) {
	long divisor;                                                                
	if( num <= 1 )
		return 0;
	
	else
		if(num > 3)
		{
			if(num % 2 == 0)
				return 0;
      		long max_divisor = sqrt(num);
      	
			for(divisor = 3; divisor <= max_divisor; divisor += 2)
	    		if (num % divisor == 0)
	      			return 0;
		}
	return 1;
}

int main() {
	long LIMITE_MAX = 4000000;
	long contaPrimo;
	long soma;
	int n;
	double inicio, fim, speedup;
	double tempo_static, tempo_dynamic, tempo_guided;
	double t1 = 0, t2 = 0, t3 = 0;


	for(int i = 0; i < 100; i++)
	{
		/*
		 *
		 * Calculando tempo dynamic
		 *
		 */

		printf("Iteracao %d\n", i+1);

		inicio = omp_get_wtime();
		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (dynamic,100)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo  =  ehPrimo(n);
				soma        =  soma + contaPrimo;
			}
  		}	

		fim               =  omp_get_wtime();
		tempo_dynamic     =  fim-inicio;
		t1 += tempo_dynamic;
		printf("Tempo dynamic: %lf\n\n", tempo_dynamic);
			
		/*
		 *
		 * Calculando tempo static
		 *
		 */
		
		inicio = omp_get_wtime();

		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (static)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo = ehPrimo(n);
				soma = soma + contaPrimo;
			}
  		}	

		fim             =  omp_get_wtime();
		tempo_static    =  fim-inicio;
		t2 += tempo_static;
		printf("Tempo static: %lf\n\n", tempo_static);
		
		/*
		 *
		 * Calculando tempo guided
		 *
		 */
		
		inicio = omp_get_wtime();

		if (LIMITE_MAX <= 1)
			soma = 0;

		else
		{
			soma = 1;
			#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (guided, 100)
			for (n = 3; n < LIMITE_MAX; n += 2)
			{
				contaPrimo = ehPrimo(n);
				soma = soma + contaPrimo;
			}
  		}	

		fim             =  omp_get_wtime();
		tempo_guided    =  fim-inicio;
		t3 += tempo_guided;
		printf("Tempo guided: %lf\n\n", tempo_guided);
		
  	}
  	
	printf("Media dynamic = %.10lf\nMedia static = %.10lf\nMedia guided = %.10lf\n\n", t1/100, t2/100, t3/100);
	printf("Media entre execucoes = %.10lf\n", (t1/100 + t2/100 + t3/100)/3);	

	return 0;
}

================================================================================================================
Questao 5:

CHUNK_MAX = 1000

Menor tempo guided e chunk usado: 1.451105   550
Menor tempo dynamic e chunk usado: 1.449571   300
Menor media de tempo guided: 1.453689
Menor media de tempo dynamic: 1.452483

Codigo:

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int ehPrimo(long num) {
	long divisor;                                                                
	if( num <= 1 )
		return 0;
	
	else
		if(num > 3)
		{
			if(num % 2 == 0)
				return 0;
      		long max_divisor = sqrt(num);
      	
			for(divisor = 3; divisor <= max_divisor; divisor += 2)
	    		if (num % divisor == 0)
	      			return 0;
		}
	return 1;
}

int main() {
	long LIMITE_MAX = 4000000;
	long CHUNK_MAX  = 1000;
	long contaPrimo;
	long soma;
	int n;
	double inicio, fim, speedup;
	double tempo_dynamic, tempo_guided;
	
	double t1, t2;
	double menor_tempo_guided = 100, menor_tempo_dynamic = 100;
	double menor_media_guided = 100, menor_media_dynamic = 100;
	int melhor_chunk_guided, melhor_chunk_dynamic;
	
	for(int chunk = 50; chunk < CHUNK_MAX; chunk += 50)
	{
		t1 = 0; t2 = 0;
		printf("%lf%% feito\n", (double)chunk/CHUNK_MAX*100);
		
		for(int i = 0; i < 20; i++)
		{
			/*
			 *
			 * Calculando tempo dynamic
			 *
			 */

			printf("Iteracao %d\n", i+1);

			inicio = omp_get_wtime();
			if (LIMITE_MAX <= 1)
				soma = 0;

			else
			{
				soma = 1;
				#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (dynamic, chunk)
				for (n = 3; n < LIMITE_MAX; n += 2)
				{
					contaPrimo  =  ehPrimo(n);
					soma        =  soma + contaPrimo;
				}
	  		}	

			fim               =  omp_get_wtime();
			tempo_dynamic     =  fim-inicio;
			t1                += tempo_dynamic;
			printf("Tempo dynamic: %lf\n\n", tempo_dynamic);
		
			/*
			 *
			 * Calculando tempo guided
			 *
			 */
		
			inicio = omp_get_wtime();

			if (LIMITE_MAX <= 1)
				soma = 0;

			else
			{
				soma = 1;
				#pragma omp parallel for private (contaPrimo) reduction(+:soma) schedule (guided, chunk)
				for (n = 3; n < LIMITE_MAX; n += 2)
				{
					contaPrimo = ehPrimo(n);
					soma = soma + contaPrimo;
				}
  			}	

			fim             =  omp_get_wtime();
			tempo_guided    =  fim-inicio;
			t2              += tempo_guided;
			printf("Tempo guided: %lf\n\n", tempo_guided);
			
  			
			if(tempo_guided < menor_tempo_guided)
			{
				menor_tempo_guided   =  tempo_guided;
				printf("Melhor chunk guided agora eh %d\n", chunk);
				melhor_chunk_guided  =  chunk;
			}
		
			if(tempo_dynamic < menor_tempo_dynamic)
			{
				menor_tempo_dynamic   =  tempo_dynamic;
				melhor_chunk_dynamic  =  chunk;
				printf("Melhor chunk dynamic agora eh %d\n", chunk);
			}
  		}
		

		t1 /= 20; t2 /= 20;	
		if(t1 < menor_media_dynamic)
			menor_media_dynamic        =  t1;
		
		if(t2 < menor_media_guided)
			menor_media_guided        =  t2;

	}
	printf("\n===============\nMenor tempo guided e chunk usado: %lf   %d\nMenor tempo dynamic e chunk usado: %lf   %d\n", menor_tempo_guided, melhor_chunk_guided, menor_tempo_dynamic, melhor_chunk_dynamic);
	
	printf("Menor media de tempo guided: %lf\nMenor media de tempo dynamic: %lf\n", menor_media_guided, menor_media_dynamic);	

	return 0;
}
======================================================================================================================

