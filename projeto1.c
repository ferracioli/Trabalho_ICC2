#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCR 50

typedef struct a_{
	//struct que guarda o horario
	int hh;
	int mm;
	int ss;
} horario;

typedef struct b_{
	//struct para cada processo
	int prior;
	horario chegada;
	char descricao[MAX_DESCR];
} celula;

int busca_maior_prior(celula *vetor, int tamanho);
int busca_menor_tempo(celula *vetor, int tamanho);
void troca_prior(celula *vetor, int tamanho, int anterior, int novo);
void troca_tempo(celula *vetor, int tamanho, horario temp_anterior, horario temp_novo);
void desloca(celula *vetor, int tamanho, int inicio);

int main(){
	char comando[7];//Ve qual comando o usuario deseja
	celula vetor[100];//Vetor com a quantidade maxima de elementos possivel
	int tamanho = 0;//Guarda o valor atual de elementos no vetor
	
	do{
		fflush(stdin);
		scanf("%s", comando);//Leitura do comando
		if(strcmp(comando, "add") == 0){
		    //Adiciona um processo na lista de espera apos o ultimo elemento
		    char tempo[9];
		    char string[3];
		    
		    scanf("%d", &vetor[tamanho].prior);
		    
		    //Leitura do horario
		    scanf("%s", tempo);
		    string[0]=tempo[0];
		    string[1]=tempo[1];
		    string[2]='\0';
		    vetor[tamanho].chegada.hh = atoi(string);
		    
		    string[0]=tempo[3];
		    string[1]=tempo[4];
		    string[2]='\0';
		    vetor[tamanho].chegada.mm = atoi(string);
		    
		    string[0]=tempo[6];
		    string[1]=tempo[7];
		    string[2]='\0';
		    vetor[tamanho].chegada.ss = atoi(string);
		    
			scanf("%s", vetor[tamanho].descricao);
			
			/*printf("%2d\n", vetor[tamanho].prior);
			printf("%d:%d:%d\n", vetor[tamanho].chegada.hh, vetor[tamanho].chegada.mm, vetor[tamanho].chegada.ss);
			printf("%s\n", vetor[tamanho].descricao);*/
			tamanho++;
		}
		else{
			if(!strcmp(comando, "exec")){
				//Executa um processo de acordo com o criterio
				char select[3];
				scanf("%s", select);
				int i;
				if(strcmp(select, "-p")==0){
					//Executa processo com maior prioridade
					i = busca_maior_prior(vetor, tamanho);
				}
				else{
					if(strcmp(select, "-t")==0){
						//Executa processo com menor tempo
						i = busca_menor_tempo(vetor, tamanho);
					}
				}
				desloca(vetor, tamanho, i);//Evita um espaco vago dentro do vetor
				tamanho--;
			}
			else{
				if(!strcmp(comando, "next")){
					//Mostra um processo de acordo com o criterio
					char select[3];
					int i;
					scanf("%s", select);
					
					if(strcmp(select, "-p")==0){
						//Informacoes do processo com mais prioridade
						i = busca_maior_prior(vetor, tamanho);	
					}
					else{
						if(strcmp(select, "-t")==0){
							//Informacoes do processo com menor tempo
							i = busca_menor_tempo(vetor, tamanho);
						}
					}
					if(vetor[i].prior < 10)
						printf("0%d ", vetor[i].prior);
					else
						printf("%d ", vetor[i].prior);
					printf("%d:%d:%d %s\n\n", vetor[i].chegada.hh, vetor[i].chegada.mm, vetor[i].chegada.ss, vetor[i].descricao);
				}
				else{
					if(!strcmp(comando, "change")){
						//Modifica alguma informacao de um processo
						char select[3];
						scanf("%s", select);
						
						if(strcmp(select, "-p")==0){
							//Altera a prioridade do processo
							int anterior, novo;
							scanf("%d|%d", &anterior, &novo);
							troca_prior(vetor, tamanho, anterior, novo);
						}
						else{
							if(strcmp(select, "-t")==0){
								//Altera o tempo do processo
								char anterior[9], novo[9], gamb_anterior[3], gamb_novo[3];
								horario temp_anterior, temp_novo;
								
								scanf("%s|%s", anterior, novo);
								
								//Definicao do horario antigo
								gamb_anterior[0]=anterior[0];
							    gamb_anterior[1]=anterior[1];
							    gamb_anterior[2]='\0';
							    temp_anterior.hh = atoi(anterior);
							    
							    gamb_anterior[0]=anterior[3];
							    gamb_anterior[1]=anterior[4];
							    gamb_anterior[2]='\0';
							    temp_anterior.mm = atoi(anterior);
							    
							    gamb_anterior[0]=anterior[6];
							    gamb_anterior[1]=anterior[7];
							    gamb_anterior[2]='\0';
							    temp_anterior.ss = atoi(anterior);
							    
							    //Definicao do horario novo
							    gamb_novo[0]=novo[0];
							    gamb_novo[1]=novo[1];
							    gamb_novo[2]='\0';
							    temp_novo.hh = atoi(novo);
							    
							    gamb_novo[0]=novo[3];
							    gamb_novo[1]=novo[4];
							    gamb_novo[2]='\0';
							    temp_novo.mm = atoi(novo);
							    
							    gamb_novo[0]=novo[6];
							    gamb_novo[1]=novo[7];
							    gamb_novo[2]='\0';
							    temp_novo.ss = atoi(novo);
							    
							    troca_tempo(vetor, tamanho, temp_anterior, temp_novo);
							}
						}
					}
					else{
						if(!strcmp(comando, "print")){
							//Imprime todos os processos
							int i;
							for(i = 0;i < tamanho;i++){
								if(vetor[i].prior < 10)
									printf("0%d ", vetor[i].prior);
								else
									printf("%d ", vetor[i].prior);
								printf("%d:%d:%d %s\n", vetor[i].chegada.hh, vetor[i].chegada.mm, vetor[i].chegada.ss, vetor[i].descricao);
							}
							//OBS:DEVEMOS IMPRIMIR A LISTA COM UM SORT
						}
					}
				}
			}
		}
	}while(strcmp(comando, "quit") != 0);
	
	printf("Saiu\n");
	return 0;
}

int busca_maior_prior(celula *vetor, int tamanho){
	//Busca o endereco do elemento com o maior indice de prioridade
	int i, maior = 0, casa = 0;
	for(i = 0;i < tamanho;i++){
		if(vetor[i].prior > maior){
			maior = vetor[i].prior;
			casa = i;
		}
	}
	return casa;
}

int busca_menor_tempo(celula *vetor, int tamanho){
	//Busca o endereco do elemento com o menor tempo
	int i, horas = 25, minutos = 61, segundos = 61, casa;
	for(i = 0;i < tamanho;i++){
		if(vetor[i].chegada.hh < horas){
			//Se o numero de horas for menor ja atribui
			horas = vetor[i].chegada.hh;
			casa = i;
		}
		else{
			if(vetor[i].chegada.hh == horas){
				//Se o numero de horas e igual,desempata nos minutos
				if(vetor[i].chegada.mm < minutos){
					//Se o numero de minutos for menor ja recebe
					minutos = vetor[i].chegada.mm;
					casa = i;
				}
				else{
					if(vetor[i].chegada.mm == minutos){
						//Em caso de minutos iguais os segundos decidem
						if(vetor[i].chegada.ss < segundos){
							segundos = vetor[i].chegada.ss;
							casa = i;
					}
					}
				}
			}
		}
	}
	return casa;
}

void desloca(celula *vetor, int tamanho, int inicio){
	//A partir de uma casa definida,desloca todos os elementos uma casa para a esquerda
	int i;
	for(i= inicio;i < tamanho;i++){
		vetor[i].chegada.hh = vetor[i+1].chegada.hh;
		vetor[i].chegada.mm = vetor[i+1].chegada.mm;
		vetor[i].chegada.ss = vetor[i+1].chegada.ss;
		strcpy(vetor[i].descricao, vetor[i+1].descricao);
		vetor[i].prior = vetor[i+1].prior;
	}
	return;
}

void troca_prior(celula *vetor, int tamanho, int anterior, int novo){
	int i;
	for(i = 0;i < tamanho;i++){
		if(vetor[i].prior == anterior){
			//Cada prior e unico,entao encontrando e so trocar
			vetor[i].prior = novo;
			break;
		}
	}
	return;
}

void troca_tempo(celula *vetor, int tamanho, horario temp_anterior, horario temp_novo){
	int i;
	for(i = 0;i < tamanho;i++){
		if(vetor[i].chegada.hh == temp_anterior.hh && vetor[i].chegada.mm == temp_anterior.mm && vetor[i].chegada.hh == temp_anterior.ss){
			//Encontra o horario especifico e troca
			vetor[i].chegada.hh = temp_novo.hh;
			vetor[i].chegada.mm = temp_novo.mm;
			vetor[i].chegada.ss = temp_novo.ss;
			break;
		}
	}
	return;
}
