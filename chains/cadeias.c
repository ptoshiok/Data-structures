#include "cadeias.h"

/*----- Declaração das funções ------------------------*/
/* Imprime mensagem de erro e a função onde ela ocorreu */


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

/* Calcula o comprimento útil de uma cadeia (não conta a posição do
   '\0'). Equivale a strlen() */

long ComprimentoCadeia(char *cadeia)
{
  if (cadeia != NULL){
    long i=0; 
    while(cadeia[i]!='\0') i++;
    return(i);
  } 
  return(0);
}

/* Cria nova cadeia com parte de outra. Funções relacionadas são
   strcpy e strncpy */

char *CopiaCadeia(char *cadeia, long pos_inicial, long pos_final)
{
  long comp_cadeia = ComprimentoCadeia(cadeia);

  /* Ajusta parâmetros fora do intervalo */
  
  if (pos_inicial < 0)
    pos_inicial = 0;

  if ((pos_final < 0)||(pos_final>=comp_cadeia))
    pos_final=comp_cadeia-1;

  /* Verifica erro para parâmetros no intervalo correto */
  
  if (pos_inicial > pos_final){
    Erro("Posição inicial %ld não pode ser maior do que posição final %ld\n","CopiaCadeia",pos_inicial,pos_final);
  }

  /* Cria a cópia */
  
  char *copia_cadeia = CriaCadeia(pos_final-pos_inicial+2);
  for (long pos=pos_inicial, i=0; pos <= pos_final; pos++, i++)
    copia_cadeia[i] = cadeia[pos];
  copia_cadeia[pos_final-pos_inicial+1] = '\0';

  return(copia_cadeia);
}


/* Verifica se duas cadeias são iguais, retornando 1 (verdadeiro) ou 0
   (falso). Relacionada com strcmp */

bool CadeiasIguais(char *cadeia1, char *cadeia2)
{
  long comp_cadeia = ComprimentoCadeia(cadeia1);
  if (comp_cadeia!=ComprimentoCadeia(cadeia2))
    return(false);
  else {
    for(long i=0; i < comp_cadeia; i++) 
      if (cadeia1[i]!=cadeia2[i])
	return(false);
  }

  return(true);
}

/* Busca a próxima ocorrência de um padrão na cadeia. Se existir,
   retorna verdadeiro e sua posição inicial na variável pos. Caso
   contrário, retorna falso. */


bool BuscaPadraoProximaOcorrencia(char *cadeia, char *padrao, long *pos)
{
  long comp_cadeia = ComprimentoCadeia(cadeia);
  long comp_padrao = ComprimentoCadeia(padrao);

  do {
    if (cadeia[*pos] == padrao[0]){ /* detecta o primeiro caracter do
				       delimitador e verifica se
				       trata-se de uma ocorrência. */
      long i = 1;
      while ((i < comp_padrao)&&(cadeia[(*pos)+i]==padrao[i]))
	i++;
      if (i == comp_padrao)
	return(true); /* encontrou na posição *pos */
    }    
    (*pos)++;    
  } while ((*pos) < (comp_cadeia - comp_padrao));

  return(false); /* não encontrou o padrão */
}

/* Calcula o número de ocorrências de um dado padrão em uma cadeia */

long NumeroOcorrenciasPadrao(char *cadeia, char *padrao)
{
  long num_ocorr = 0, pos = 0, comp_padrao = ComprimentoCadeia(padrao);

  while(BuscaPadraoProximaOcorrencia(cadeia, padrao,&pos)){
    num_ocorr++;
    pos = pos + comp_padrao;
  }

  return(num_ocorr);
}

/* Cria uma cadeia vazia */

char *CriaCadeia(long tamanho)
{
  if (tamanho <= 0)
    Erro("Tamanho %d inválido","CriaCadeia",tamanho);
    
  char *cadeia = (char *)calloc(tamanho+1,sizeof(char));
  cadeia[tamanho]='\0';
  return(cadeia);
}

/* Libera a área da cadeia na memória heap */

void DestroiCadeia(char **cadeia)
{
  if ((*cadeia) != NULL){
    free(*cadeia);
    (*cadeia)=NULL; 
  }
}

/* Verifica se os caracteres de uma cadeia são dígitos de 0 a 9, ou um
   ponto decimal. */

bool CadeiaNumerica(char *cadeia)
{
  for (long i=0; i < ComprimentoCadeia(cadeia); i++)
    if (((cadeia[i]!=46)&&(cadeia[i]<48))||(cadeia[i]>57))
      return(false);
  return(true);
}

/* Converte letras minúsculas em maiúsculas */

char *MinusculasParaMaiusculas(char *cadeia)
{
  long comp_cadeia = ComprimentoCadeia(cadeia);
  char *maiusculas = CriaCadeia(comp_cadeia);
  
  for (long i=0; i < comp_cadeia; i++){
    if ((cadeia[i]>=97)&&(cadeia[i]<=122)) /* se for letra minúscula */
      maiusculas[i] = cadeia[i]-32; /* converte para maiúscula */
    else
      maiusculas[i] = cadeia[i]; /* apenas copia o caracter */
  }

  return(maiusculas);
}

/* Converte uma cadeia numérica para número */

void CadeiaParaNumero(char *cadeia, char *tipo, void *num)
{
  if (!CadeiaNumerica(cadeia))
    Erro("Cadeia não numérica","CadeiaParaNumero");
	 
  if (CadeiasIguais(tipo,"long")){
    long numero;
    sscanf(cadeia,"%ld",&numero);
    *((long *)num) = numero;
  }else{
    if (CadeiasIguais(tipo,"unsigned long")){
      unsigned long numero;
      sscanf(cadeia,"%lu",&numero);
      *((unsigned long *)num) = numero;
    }else{
      if (CadeiasIguais(tipo,"int")){
	int numero;
	sscanf(cadeia,"%d",&numero);
	*((int *)num) = numero;
      }else{
	if (CadeiasIguais(tipo,"unsigned int")){
	  unsigned int numero;
	  sscanf(cadeia,"%u",&numero);
	  *((unsigned int *)num) = numero;
	} else {
	  if (CadeiasIguais(tipo,"float")){
	    float numero;
	    sscanf(cadeia,"%f",&numero);
	    *((float *)num) = numero;
	  } else {
	    if (CadeiasIguais(tipo,"double")){
	      double numero;
	      sscanf(cadeia,"%lf",&numero);
	      *((double *)num) = numero;
	    } else {
	      if (CadeiasIguais(tipo,"long double")){
		long double numero;
		sscanf(cadeia,"%Lf",&numero);
		*((long double *)num) = numero;
	      } else {
		Erro("tipo %s inválido","CadeiaParaNumero",tipo);
	      }
	    }
	  }
	}
      }
    }
  }
}

/* Converte número em cadeia numérica */

char *NumeroParaCadeia(void *num, char *tipo)
{
  char *cadeia = CriaCadeia(30);
  if (CadeiasIguais(tipo,"long")){
    sprintf(cadeia,"%ld",*((long *)num));
  }else{
    if (CadeiasIguais(tipo,"unsigned long")){
      sprintf(cadeia,"%lu",*((unsigned long *)num));
    }else{
      if (CadeiasIguais(tipo,"int")){
	sprintf(cadeia,"%d",*((int *)num));
      }else{
	if (CadeiasIguais(tipo,"unsigned int")){
	  sprintf(cadeia,"%u",*((unsigned int *)num));
	} else {
	  if (CadeiasIguais(tipo,"float")){
	    sprintf(cadeia,"%f",*((float *)num));
	  } else {
	    if (CadeiasIguais(tipo,"double")){
	      sprintf(cadeia,"%lf",*((double *)num));
	    } else {
	      if (CadeiasIguais(tipo,"long double")){
		sprintf(cadeia,"%Lf",*((long double *)num));
	      } else {
		Erro("tipo %s inválido","NumeroParaCadeia",tipo);
	      }
	    }
	  }
	}
      }
    }
  }
  return(cadeia);
}

/* Quebra a cadeia nos delimitadores, gerando as partes em um vetor de
   cadeias de caracteres. A primeira posição do vetor contém o número
   de partes. */

char **QuebraCadeia(char *cadeia, char *delim)
{
  long ndelim = 0, comp_delim = ComprimentoCadeia(delim);
  long pos = 0, pos_ant = 0;
  char **partes;

  ndelim = NumeroOcorrenciasPadrao(cadeia,delim);
  
  if (ndelim > 0){
    partes = (char **)calloc(ndelim+2,sizeof(char *));
    long tamanho = ndelim+1;
    partes[0]    = NumeroParaCadeia(&tamanho,"long");
    long i       = 1;
    pos          = pos_ant = 0; 
    while(BuscaPadraoProximaOcorrencia(cadeia, delim,&pos)){
      partes[i] = CopiaCadeia(cadeia,pos_ant,pos-1);
      i         = i + 1;
      pos       = pos_ant = pos + comp_delim;    
    }
    if (cadeia[pos]!=delim[0]){ /* o usuário esqueceu delimitador após
				   o último campo */
      pos = pos + comp_delim;
    }
    partes[i] = CopiaCadeia(cadeia,pos_ant,pos-1);
  } else {
    partes    = (char **)calloc(1,sizeof(char *));
    long tamanho = 0;
    partes[0]    = NumeroParaCadeia(&tamanho,"long");
  }

  return(partes);
}
