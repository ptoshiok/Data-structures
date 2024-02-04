#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

void Erro(char *msg, char *funcao, ...) 
{
  va_list args; /* lista de argumentos */
  char    msg_final[4096];

  va_start(args, funcao); /* inicializa a lista args para recuperar
			     argumentos após o argumento funcao */
  vsprintf(msg_final, msg, args); /* envia a lista inteira de
				     argumentos em args para
				     formatação em msg_final */
  va_end(args); /* finaliza o processo de recuperação */

  printf("Erro em %s: %s\n",funcao,msg_final);
  fflush(stdout);
  exit(1);
}

typedef struct _ponto {
  int x, y;
} Ponto;

typedef struct _labirinto {
  char **Posicao;
  int nx, ny;
  int nportais;
  Ponto entrada, saida;
  Ponto *portais;
} Labirinto;

Labirinto *LeLabirinto(char *nomearq)
{
  FILE *fp = fopen(nomearq,"r");
  Labirinto *L = (Labirinto *)calloc(1,sizeof(Labirinto));
  
  fscanf(fp,"%d %d %d\n",&L->nx,&L->ny, &L->nportais);
  L->Posicao = (char **)calloc(L->ny,sizeof(char *));
  for (int y=0; y < L->ny; y++) 
    L->Posicao[y] = (char *)calloc(L->nx,sizeof(char ));

  L->portais = (Ponto *)calloc(L->nportais, sizeof(Ponto));
  int k = 0;
  for (int y=0; y < L->ny; y++){ 
    for (int x=0; x < L->nx; x++) {
      fscanf(fp,"%c",&L->Posicao[y][x]);
      if (L->Posicao[y][x]=='E'){
	      L->entrada.x= x; L->entrada.y= y;
	      L->Posicao[y][x]='P';
      } else {
	      if (L->Posicao[y][x]=='S'){
	        L->saida.x= x; L->saida.y= y;
	        L->Posicao[y][x]='P';
	      }
        if (L->Posicao[y][x]=='G'){
          L->portais[k].x = x;
          L->portais[k].y = y;
          k += 1;
        }
      }
    }
    fscanf(fp,"\n");
  }

  fclose(fp);
  return(L);
}

void DestroiLabirinto(Labirinto **L)
{
  if ((*L)!=NULL){
    for (int y=0; y < (*L)->ny; y++)
      free((*L)->Posicao[y]);
    free((*L)->Posicao);
    free((*L)->portais);
    free((*L));
    (*L) = NULL;
  }
}

bool Backtrack(Labirinto *L, Ponto P)
{
  bool res = false;
  
  if ((P.y == L->saida.y)&&(P.x == L->saida.x)) { 
    res = true; /* solução encontrada */
  } else { /* gera candidatos */
    if((L->Posicao[P.y][P.x] == 'X') ||( L->Posicao[P.y][P.x] == 'P')){
    Ponto *C = (Ponto *)calloc(4,sizeof(Ponto));
    C[0].x = P.x - 1; C[0].y = P.y;
    C[1].x = P.x + 1; C[1].y = P.y;
    C[2].x = P.x;     C[2].y = P.y - 1;
    C[3].x = P.x;     C[3].y = P.y + 1;
    /* processa os candidatos e a condição (res != true) evita visitar
       todas as posições */
    for (int i=0; (i < 4) && (res != true); i++) {
      /* verifica as restrições */
      if ((C[i].x >= 0)&&(C[i].x < L->nx)&&(C[i].y >= 0)&&(C[i].y < L->ny)){
	    if ((L->Posicao[C[i].y][C[i].x] != 'X') && (L->Posicao[C[i].y][C[i].x] != 'K')){ /* restrição */
	    //printf("Processa (%d,%d)=%c a partir de (%d,%d)\n",C[i].x,C[i].y,L->Posicao[C[i].y][C[i].x],P.x,P.y);
	    char valor_correto = L->Posicao[C[i].y][C[i].x];
	    //L->Posicao[C[i].y][C[i].x] = 'X';  
        if(L->Posicao[C[i].y][C[i].x] == 'P'){
        L->Posicao[C[i].y][C[i].x] = 'X';
        } if(L->Posicao[C[i].y][C[i].x] == 'G'){
        L->Posicao[C[i].y][C[i].x] = 'K';
        }
        res = Backtrack(L,C[i]);
	    L->Posicao[C[i].y][C[i].x] = valor_correto; 
	    }
      }
    }
    free(C);
    } else if ((L->Posicao[P.y][P.x] == 'K'))
    {
      Ponto *C = (Ponto *)calloc((4+L->nportais),sizeof(Ponto));
      C[0].x = P.x - 1; C[0].y = P.y;
      C[1].x = P.x + 1; C[1].y = P.y;
      C[2].x = P.x;     C[2].y = P.y - 1;
      C[3].x = P.x;     C[3].y = P.y + 1;
      for(int i = 4; i < 4 + L->nportais; i++){
        C[i].x = L->portais[i-4].x; C[i].y = L->portais[i-4].y;
      }
      /* processa os candidatos e a condição (res != true) evita visitar
        todas as posições */
      for (int i=0; (i < 4+L->nportais) && (res != true); i++) {
        /* verifica as restrições */

        if ((C[i].x >= 0)&&(C[i].x < L->nx)&&(C[i].y >= 0)&&(C[i].y < L->ny)){
          if ((L->Posicao[C[i].y][C[i].x] != 'X')&&(L->Posicao[C[i].y][C[i].x] != 'K')){ /* restrição */
          //printf("Processa (%d,%d)=%c a partir de (%d,%d)\n",C[i].x,C[i].y,L->Posicao[C[i].y][C[i].x],P.x,P.y);
          char valor_correto = L->Posicao[C[i].y][C[i].x];
          if(L->Posicao[C[i].y][C[i].x] == 'P'){
          L->Posicao[C[i].y][C[i].x] = 'X';
          } if(L->Posicao[C[i].y][C[i].x] == 'G'){
          L->Posicao[C[i].y][C[i].x] = 'K';
          }
          res = Backtrack(L,C[i]);
          L->Posicao[C[i].y][C[i].x] = valor_correto; 
          }
        }
      }
      free(C);
    }
    
    //free(C);
  }
  return(res);
}

int main(int argc, char **argv)
{
  Labirinto *L = NULL;
  
  if (argc != 2){
    Erro("%s <arquivo texto do labirinto>","main",argv[0]); 
  }
  
  L = LeLabirinto(argv[1]);

  /*for (int y=0; y < L->ny; y++){  
     for (int x=0; x < L->nx; x++) { 
       printf("%c",L->Posicao[y][x]); 
     }
     printf("\n");
   }*/

  if (Backtrack(L,L->entrada))
    printf("Encontrou a saida em (%d,%d)\n",L->saida.x,L->saida.y);
  else
    printf("Labirinto sem saida\n");
	
  DestroiLabirinto(&L);
  return(0);
}
