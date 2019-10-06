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

int horario_para_int(horario tmp);
void swap(celula *c1, celula *c2);

void ordenar_prioridade(celula *vetor, int esquerda, int direita);
void ordenar_tempo(celula *vetor, int esquerda, int direita);

int main(){
	char comando[7];//Ve qual comando o usuario deseja
	celula vetor[100];//Vetor com a quantidade maxima de elementos possivel
	int tamanho = 0;//Guarda o valor atual de elementos no vetor
	
	do{
		fflush(stdin);
		scanf("%s", comando);//Leitura do comando

		if(!strcmp(comando, "add")){
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

			tamanho++;
		}
		else if(!strcmp(comando, "exec")){
			//Executa um processo de acordo com o criterio
			char select[3];
			scanf("%s", select);
			int i;

			if(strcmp(select, "-p")==0){
				//Executa processo com maior prioridade
				i = busca_maior_prior(vetor, tamanho);
			}
			else if(strcmp(select, "-t")==0){
				//Executa processo com menor tempo
				i = busca_menor_tempo(vetor, tamanho);
			}
			
			desloca(vetor, tamanho, i);//Evita um espaco vago dentro do vetor
			tamanho--;
		}
		else if(!strcmp(comando, "next")){

			//Mostra um processo de acordo com o criterio
			char select[3];
			int i;
			scanf("%s", select);
			
			if(strcmp(select, "-p")==0){
				//Informacoes do processo com mais prioridade
				i = busca_maior_prior(vetor, tamanho);	
			}
			else if(strcmp(select, "-t")==0){
				//Informacoes do processo com menor tempo
				i = busca_menor_tempo(vetor, tamanho);
			}

			printf("%02d %02d:%02d:%02d %s\n\n", vetor[i].prior, vetor[i].chegada.hh, vetor[i].chegada.mm, vetor[i].chegada.ss, vetor[i].descricao);

		}
		else if(!strcmp(comando, "change")){

			//Modifica alguma informacao de um processo
			char select[3];
			scanf("%s", select);
			
			if(!strcmp(select, "-p")){
				//Altera a prioridade do processo
				int anterior, novo;
				scanf("%d|%d", &anterior, &novo);
				troca_prior(vetor, tamanho, anterior, novo);
			}
			else if(!strcmp(select, "-t")){
				//Altera o tempo do processo
				char anterior[9], novo[9], gamb_anterior[3], gamb_novo[3];
				horario temp_anterior, temp_novo;
				
				scanf("%s|%s", anterior, novo);
				
				//Definicao do horario antigo
				gamb_anterior[0]=anterior[0];
				gamb_anterior[1]=anterior[1];
				gamb_anterior[2]='\0';
				temp_anterior.hh = atoi(gamb_anterior);
				
				gamb_anterior[0]=anterior[3];
				gamb_anterior[1]=anterior[4];
				gamb_anterior[2]='\0';
				temp_anterior.mm = atoi(gamb_anterior);
				
				gamb_anterior[0]=anterior[6];
				gamb_anterior[1]=anterior[7];
				gamb_anterior[2]='\0';
				temp_anterior.ss = atoi(gamb_anterior);
				
				//Definicao do horario novo
				gamb_novo[0]=novo[0];
				gamb_novo[1]=novo[1];
				gamb_novo[2]='\0';
				temp_novo.hh = atoi(gamb_novo);
				
				gamb_novo[0]=novo[3];
				gamb_novo[1]=novo[4];
				gamb_novo[2]='\0';
				temp_novo.mm = atoi(gamb_novo);
				
				gamb_novo[0]=novo[6];
				gamb_novo[1]=novo[7];
				gamb_novo[2]='\0';
				temp_novo.ss = atoi(gamb_novo);
				
				troca_tempo(vetor, tamanho, temp_anterior, temp_novo);
			}

		}
		else if(!strcmp(comando, "print")){

			//Modifica alguma informacao de um processo
			char select[3];
			scanf("%s", select);
			
			if(!strcmp(select, "-p")){
				
				ordenar_prioridade(vetor, 0, tamanho-1);

			}
			else if(!strcmp(select, "-t")){

				ordenar_tempo(vetor, 0, tamanho-1);
				
			}

			//Imprime todos os processos
			int i;
			for(i = 0;i < tamanho;i++)
				printf("%02d %02d:%02d:%02d %s\n", vetor[i].prior, vetor[i].chegada.hh, vetor[i].chegada.mm, vetor[i].chegada.ss, vetor[i].descricao);

			printf("\n");

		}

	}while(strcmp(comando, "quit") != 0);

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

	int i, casa = -1, atual, menor = 99999999;

	//Busca o endereco do elemento com o menor tempo
	for(i=0; i<tamanho; i++){

		//Converte o horario para um inteiro
		atual = horario_para_int(vetor[i].chegada);

		if(atual < menor){
			menor = atual;
			casa = i;
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
		if(vetor[i].chegada.hh == temp_anterior.hh && vetor[i].chegada.mm == temp_anterior.mm && vetor[i].chegada.ss == temp_anterior.ss){
			//Encontra o horario especifico e troca
			vetor[i].chegada.hh = temp_novo.hh;
			vetor[i].chegada.mm = temp_novo.mm;
			vetor[i].chegada.ss = temp_novo.ss;
			break;
		}
	}
	return;
}

int horario_para_int(horario tmp){

	return ((tmp.hh*3600)+(tmp.mm*60)+tmp.ss);
	
}

void swap(celula *c1, celula *c2){

	int tmp_prior, tmp_hh, tmp_mm, tmp_ss;
	char *tmp_descricao = (char *) malloc((strlen((*c1).descricao) + 1) * sizeof(char));

	tmp_prior 		= (*c1).prior;
	(*c1).prior 	= (*c2).prior;
	(*c2).prior 	= tmp_prior;

	tmp_hh = (*c1).chegada.hh;
	tmp_mm = (*c1).chegada.mm;
	tmp_ss = (*c1).chegada.ss;

	(*c1).chegada.hh = (*c2).chegada.hh;
	(*c1).chegada.mm = (*c2).chegada.mm;
	(*c1).chegada.ss = (*c2).chegada.ss;

	(*c2).chegada.hh = tmp_hh;
	(*c2).chegada.mm = tmp_mm;
	(*c2).chegada.ss = tmp_ss;

	strcpy(tmp_descricao, (*c1).descricao);
	strcpy((*c1).descricao, (*c2).descricao);
	strcpy((*c2).descricao, tmp_descricao);

}

void ordenar_prioridade(celula *vetor, int esquerda, int direita){
  
    int i, j, x, y;
     
    i = esquerda;
    j = direita;
    x = vetor[(esquerda + direita) / 2].prior;

    while(i <= j) {

        while(vetor[i].prior > x && i < direita)
            i++;

        while(vetor[j].prior < x && j > esquerda)
            j--;

        if(i <= j) {

			swap(&vetor[i], &vetor[j]);

            i++;
            j--;
        
		}
    }
     
    if(j > esquerda)
        ordenar_prioridade(vetor, esquerda, j);

    if(i < direita)
        ordenar_prioridade(vetor, i, direita);

}

void ordenar_tempo(celula *vetor, int esquerda, int direita){
  
    int i, j, x;
     
    i = esquerda;
    j = direita;
    x = horario_para_int(vetor[(esquerda + direita) / 2].chegada);

    while(i <= j) {

        while(horario_para_int(vetor[i].chegada) < x && i < direita)
            i++;

        while(horario_para_int(vetor[j].chegada) > x && j > esquerda)
            j--;

        if(i <= j) {
			
			swap(&vetor[i], &vetor[j]);

            i++;
            j--;
        
		}
    }
     
    if(j > esquerda)
        ordenar_tempo(vetor, esquerda, j);

    if(i < direita)
        ordenar_tempo(vetor, i, direita);

}
