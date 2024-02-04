/**
 * cadeias.h
 * Funções que manipulam cadeias de caracteres
 *
 */

#ifndef CADEIAS_H
#define CADEIAS_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

/* Se não foi definido, defina o tipo booleano */

#ifndef bool 
typedef enum _bool {
  false, true
} bool;
#endif

/* Prototipagem das funções: permite que elas sejam encontradas
   independente da ordem em que são declaradas */

void   Erro(char *msg, char *funcao, ...); /* argumentos variáveis */
long   ComprimentoCadeia(char *cadeia);
char  *CopiaCadeia(char *cadeia, long pos_inicial, long pos_final);
bool   CadeiasIguais(char *cadeia1, char *cadeia2);
bool   BuscaPadraoProximaOcorrencia(char *cadeia, char *padrao, long *pos);
long   NumeroOcorrenciasPadrao(char *cadeia, char *padrao);
char  *CriaCadeia(long tamanho);
void   DestroiCadeia(char **cadeia);
bool   CadeiaNumerica(char *cadeia);
char  *MinusculasParaMaiusculas(char *cadeia);
void   CadeiaParaNumero(char *cadeia, char *tipo, void *num);
char  *NumeroParaCadeia(void *num, char *tipo);
char **QuebraCadeia(char *cadeia, char *delim);

#endif
