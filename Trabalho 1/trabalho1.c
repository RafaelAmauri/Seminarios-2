#include <stdio.h>
#include <omp.h>

/* 
 *   Autor      =  Rafael Amauri Diniz Augusto
 *   Matricula  =  651047
 */


double calcular_sequencial(double precisao);
double calcular_paralelo(double precisao);

int main(){
	double precisao = 0.000000001;
	double inicio, fim, tempo_sequencial, tempo_paralelo;
	int resposta, quant_iteracoes = 15; // Trocar aqui para quantidade de iteracoes que deseja
	
	// Tirar o bloco abaixo dos comentarios caso queira trocar a precisao
	
	/*	
	 *
		printf("A precisao atual esta em '0.000000001'. Deseja mudar?\n[1] = Sim\n[2] = Nao\n");
		scanf("%d", &resposta);
	
		resposta == 1
		? printf("Digite a precisao (valores menores = mais preciso)\n"), scanf("%lf", &precisao)
		: 0;
	*
	*/
	
	printf("Precisao escolhida: %.9lf\n\n", precisao);
	printf("Comecando a rodar o calculo de pi %d vezes...\n", quant_iteracoes);

	double speedup_medio = 0;
	double pi;

	// Calculando tempo de execucao medio entre iteracoes
	for(int i = 0; i < quant_iteracoes; i++)
	{
		printf("-------------------\n|Execucao numero %d|\n-------------------\n\n", i+1);
		
		
		printf("Calculando sequencial...\n------\n");
		inicio            =  omp_get_wtime();
		pi                =  calcular_sequencial(precisao);
		fim               =  omp_get_wtime();
		tempo_sequencial  =  fim-inicio;
		printf("Tempo sequencial = %lf\n\n------\n", tempo_sequencial);
		

		printf("Calculando paralelo...\n------\n");
		inicio          =  omp_get_wtime();
		pi              =  calcular_paralelo(precisao);
		fim             =  omp_get_wtime();
		tempo_paralelo  =  fim-inicio;
		printf("Tempo paralelo = %lf\n\n", tempo_paralelo);

		
		printf("O speedup da %d execucao foi %lf\n", i+1, tempo_sequencial/tempo_paralelo);
		speedup_medio += tempo_sequencial/tempo_paralelo;
	}
	
	printf("\n\nO speedup medio entre %d iteracoes foi de %lf\n", quant_iteracoes, speedup_medio/quant_iteracoes);
	
	return 0;
}

// Funcao da questao 1
double calcular_sequencial(double precisao)
{
	double pi =  4;	
	long int quantPassos = 1.0/precisao;

	for(int i = 3; i < quantPassos; i += 4)
	{
		pi -=  4.0 / (i);
		pi +=  4.0 / (i+2);
	}
	return pi;
}

// Funcao da questao 2
double calcular_paralelo(double precisao)
{
	double pi =  4;	
	long int quantPassos = 1.0/precisao;
	
	#pragma omp parallel for reduction(+:pi)	
		for(int i = 3; i < quantPassos; i += 4)
		{
			pi -=  4.0 / (i);
			pi +=  4.0 / (i+2);
		}
	return pi;
}
