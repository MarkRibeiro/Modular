/***************************************************************************
*  $MCI M�dulo de implementa��o: M�dulo �rvore
*
*  Arquivo gerado:              ARVORE.C
*  Letras identificadoras:      ARV
*
*  Nome da base de software:    Exemplo de teste automatizado
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\SIMPLES.BSW
*
*  Projeto: Disciplinas INF 1628 / 1301
*  Gestor:  DI/PUC-Rio
*  Autores: avs - Arndt von Staa
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*       3.00   avs   28/02/2003 Uniformiza��o da interface das fun��es e
*                               de todas as condi��es de retorno.
*       2.00   avs   03/08/2002 Elimina��o de c�digo duplicado, reestrutura��o
*       1.00   avs   15/08/2001 In�cio do desenvolvimento
*
***************************************************************************/

#include   <malloc.h>
#include   <stdio.h>
#include <stdlib.h>

#define ARVORE_OWN
#include "ARVORE.H"
#undef ARVORE_OWN

#include "LISTA.h"

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do n� da �rvore
*
*
*  $ED Descri��o do tipo
*     Descreve a organiza��o do n�
*
***********************************************************************/

   typedef struct tgNoArvore {

         struct tgNoArvore * pNoPai ;
               /* Ponteiro para pai
               *
               *$EED Assertivas estruturais
               *   � NULL sse o n� � raiz
               *   Se n�o for raiz, um de pNoEsq ou pNoDir de pNoPai do n�
               *   corrente apontam para o n� corrente */

         struct tgNoArvore * pNoEsq ;
               /* Ponteiro para filho � esquerda
               *
               *$EED Assertivas estruturais
               *   se pNoEsq do n� X != NULL ent�o pNoPai de pNoEsq aponta para o n� X */

         struct tgNoArvore * pNoDir ;
               /* Ponteiro para filho � direita
               *
               *$EED Assertivas estruturais
               *   se pNoDir do n� X != NULL ent�o pNoPai de pNoDir aponta para o n� X */

		 struct tgNoArvore * pNoCostura ;
               /* Ponteiro para o proximo a ser costurado*/

		 NoLista* pNoLista;
			//	/*Ponteiro para a lista encadeada de inteiros*/
               
               

         char Valor ;
               /* Valor do n� */

   } tpNoArvore ;

/***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor da cabe�a de uma �rvore
*
*
*  $ED Descri��o do tipo
*     A cabe�a da �rvore � o ponto de acesso para uma determinada �rvore.
*     Por interm�dio da refer�ncia para o n� corrente e do ponteiro
*     pai pode-se navegar a �rvore sem necessitar de uma pilha.
*     Pode-se, inclusive, operar com a �rvore em forma de co-rotina.
*
***********************************************************************/

   typedef struct tgArvore {

         tpNoArvore * pNoRaiz ;
               /* Ponteiro para a raiz da �rvore */

         tpNoArvore * pNoCorr ;
               /* Ponteiro para o n� corrente da �rvore */

   } tpArvore ;


   /***********************************************************************
*
*  $TC Tipo de dados: ARV Descritor do primeiro n� da costura
*
*
*  $ED Descri��o do tipo
*     O primeiro no da costura � a primeira folha que vai iniciar o processo de costura.
*
***********************************************************************/

   typedef struct primeiroCostura {
	   tpNoArvore * pNoIni; //Ponteiro para o primeiro n� da costura
	   tpNoArvore * pNoCorr; //Ponteiro para o n� corrente da costura
   } PrimCost;

/*****  Dados encapsulados no m�dulo  *****/

      static tpArvore * pArvore = NULL ;
            /* Ponteiro para a cabe�a da �rvore */

	  static PrimCost * costura = NULL;
			//Ponteiro para o primeiro n� da costura das folhas

/***** Prot�tipos das fun��es encapuladas no m�dulo *****/

   static tpNoArvore * CriarNo( char ValorParm ) ;

   static ARV_tpCondRet CriarNoRaiz( char ValorParm ) ;

   static void DestroiArvore( tpNoArvore * pNo ) ;

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Fun��o: ARV Criar �rvore
*  ****/

   ARV_tpCondRet ARV_CriarArvore( void )
   {
      if ( pArvore != NULL )
      {
         ARV_DestruirArvore( ) ;
      } /* if */

      pArvore = ( tpArvore * ) malloc( sizeof( tpArvore )) ;
      if ( pArvore == NULL )
      {
         return ARV_CondRetFaltouMemoria ;
      } /* if */

      pArvore->pNoRaiz = NULL ;
      pArvore->pNoCorr = NULL ;

      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Criar �rvore */

   /***************************************************************************/

   //In�cio da fun��o: ARV Faz Lista
   ARV_tpCondRet ARV_Faz_Lista( int n1, int n2, int n3 )
   {
      if ( pArvore != NULL )
      {
         if ( pArvore->pNoCorr != NULL )
         {
			 NoLista* aux = NULL;
			 aux=LIS_Cria( n1, n2, n3 ) ;
			 if(aux==NULL)
			 {
				 return ARV_CondRetFaltouMemoria;
			 }//if
			 costura->pNoCorr=costura->pNoIni;
			 while(costura->pNoCorr)
			 {
				 costura->pNoCorr->pNoLista=aux;
				 costura->pNoCorr=costura->pNoCorr->pNoCostura;
			 }//while
         } //if
		 else
		 {
			 return ARV_CondRetArvoreVazia ;
		 }//else
		 return ARV_CondRetOK ;
      }
	  else
	  {
		  return ARV_CondRetArvoreNaoExiste ;
	  }

   } /* Fim fun��o: ARV Faz Lista */


   /***************************************************************************/
   /****Fun��o: Costura ****/

   /* Fun��o de troca de posi��o dos n�s 'a' e 'b'*/
   void troca(tpNoArvore *a, tpNoArvore *b) 
   { 
	   tpNoArvore * aux = ( tpNoArvore * ) malloc( sizeof( tpNoArvore )) ;
	   if(aux==NULL)
	   {
		   printf("Faltou memoria para criar o no auxiliar na fun��o de troca.\n");
	   }

	   aux->pNoCostura=a->pNoCostura;
	   aux->pNoDir=a->pNoDir;
	   aux->pNoEsq=a->pNoEsq;
	   aux->pNoPai=a->pNoPai;
	   aux->Valor=a->Valor;

	   a->pNoDir=b->pNoDir;
	   a->pNoEsq=b->pNoEsq;
	   a->pNoPai=b->pNoPai;
	   a->Valor=b->Valor;

	   b->pNoDir=aux->pNoDir;
	   b->pNoEsq=aux->pNoEsq;
	   b->pNoPai=aux->pNoPai;
	   b->Valor=aux->Valor;

	   free(aux);
   } 


   /*Fun��o que recebe a lista de costura e a ordena alfabeticamente*/
   void Ajeita_Lista(tpNoArvore *start) 
   { 
	   int i, swapped;
	   tpNoArvore *NoAux1; 
	   tpNoArvore *NoAux2 = NULL;

	   do
	   { 
		   swapped = 0; 
		   NoAux1 = start; 

		   while (NoAux1 != NoAux2) 
		   { 
			   if (NoAux1->pNoCostura!=NULL && (NoAux1->Valor > NoAux1->pNoCostura->Valor)) 
			   {  
				   troca(NoAux1, NoAux1->pNoCostura); 
				   swapped = 1; 
			   } //if
			   NoAux1 = NoAux1->pNoCostura; 
		   } //while
		   NoAux2 = NoAux1; 
	   } 
	   while (swapped);

	   free(NoAux1);
	   free(NoAux2);
   } 

   //Fun��o que cria uma lista encadeada de costura desordenada a partir das folhas da arvore.
   void Costura (tpNoArvore * pNo)
   {
	    if(costura==NULL)
		{
			costura = ( PrimCost * ) malloc( sizeof( PrimCost )) ;
			if(costura==NULL)
			{
				printf("Faltou memoria para criar a lista encadeada de costura.");
			}//if
			costura->pNoIni=NULL;
			costura->pNoCorr=NULL;
		}//if
	   
		if(pNo==NULL)
		{
			return;
		}//if

		if(pNo->pNoDir!=NULL)
			Costura(pNo->pNoDir);

		if(pNo->pNoEsq!=NULL)
			Costura(pNo->pNoEsq);

		if(pNo->pNoDir==NULL && pNo->pNoEsq==NULL)
		{
			if(costura->pNoIni==NULL)
			{
				costura->pNoIni= pNo;
				costura->pNoCorr= pNo;
			}//if
			else
			{
				costura->pNoCorr->pNoCostura=pNo;
				costura->pNoCorr=pNo;
			}//else
		}//if
   }

   ARV_tpCondRet ARV_Costura( void )
   {
      if ( pArvore != NULL )
      {
         if ( pArvore->pNoCorr != NULL )
         {
            Costura( pArvore->pNoRaiz ) ;
         } 
		 else
		 {
			 return ARV_CondRetArvoreVazia ;
		 }
		 Ajeita_Lista(costura->pNoIni);
		 return ARV_CondRetOK ;
      }
	  else
	  {
		  return ARV_CondRetArvoreNaoExiste ;
	  }

   } /* Fim fun��o: ARV Costura */

/***************************************************************************
*
*  Fun��o: ARV Destruir �rvore
*  ****/

   void ARV_DestruirArvore( void )
   {

      if ( pArvore != NULL )
      {
         if ( pArvore->pNoRaiz != NULL )
         {
            DestroiArvore( pArvore->pNoRaiz ) ;
         } /* if */
         free( pArvore ) ;
		 free(costura);
         pArvore = NULL ;
		 costura=NULL;
      } /* if */

   } /* Fim fun��o: ARV Destruir �rvore */

/***************************************************************************/
  /* *  Fun��o: Printa Testes
*  ****/
   //Fun��o que printa tanto a costura das folhas da arvore quanto as listas encadeadas de inteiros de cada folha.
   void Print_Testes ()
   {
	   tpNoArvore *No = costura->pNoIni;
	   printf("\n\tCostura: ");
	   while(No!=NULL)
	   {
		   printf("%c -> ", No->Valor);
		   No=No->pNoCostura;
	   }//while
	   No = costura->pNoIni;
	   printf("\n\tLista encadeada de numeros:\n");
	   while (No != NULL) 
	   { 
		   printf("\t%c : ", No->Valor);
		   LIS_Printa(No->pNoLista);
		   No=No->pNoCostura;
	   } //while
   }

      ARV_tpCondRet ARV_Printa( void )
   {
      if ( pArvore != NULL )
      {
         if ( pArvore->pNoCorr != NULL )
         {
            Print_Testes() ;
         } //if
		 else
		 {
			 return ARV_CondRetArvoreVazia ;
		 }//else
		 return ARV_CondRetOK ;
      }//if
	  else
	  {
		  return ARV_CondRetArvoreNaoExiste ;
	  }//else

   } /* Fim fun��o: Printa Costura */

/***************************************************************************/

/*  Fun��o: ARV Adicionar filho � esquerda
*  ****/

   ARV_tpCondRet ARV_InserirEsquerda( char ValorParm )
   {

      ARV_tpCondRet CondRet ;

      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      /* Tratar vazio, esquerda */

         CondRet = CriarNoRaiz( ValorParm ) ;
         if ( CondRet != ARV_CondRetNaoCriouRaiz )
         {
            return CondRet ;
         } /* if */

      /* Criar n� � esquerda de folha */

         pCorr = pArvore->pNoCorr ;
         if ( pCorr == NULL )
         {
            return ARV_CondRetErroEstrutura ;
         } /* if */
               
         if ( pCorr->pNoEsq == NULL )
         {
            pNo = CriarNo( ValorParm ) ;
            if ( pNo == NULL )
            {
               return ARV_CondRetFaltouMemoria ;
            } /* if */
            pNo->pNoPai      = pCorr ;
            pCorr->pNoEsq    = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar n�o folha � esquerda */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Adicionar filho � direita
*  ****/

   ARV_tpCondRet ARV_InserirDireita( char ValorParm )
   {

      ARV_tpCondRet CondRet ;

      tpNoArvore * pCorr ;
      tpNoArvore * pNo ;

      /* Tratar vazio, direita */

         CondRet = CriarNoRaiz( ValorParm ) ;
         if ( CondRet != ARV_CondRetNaoCriouRaiz )
         {
            return CondRet ;
         } /* if */

      /* Criar n� � direita de folha */

         pCorr = pArvore->pNoCorr ;
         if ( pCorr == NULL )
         {
            return ARV_CondRetErroEstrutura ;
         } /* if */

         if ( pCorr->pNoDir == NULL )
         {
            pNo = CriarNo( ValorParm ) ;
            if ( pNo == NULL )
            {
               return ARV_CondRetFaltouMemoria ;
            } /* if */
            pNo->pNoPai      = pCorr ;
            pCorr->pNoDir    = pNo ;
            pArvore->pNoCorr = pNo ;

            return ARV_CondRetOK ;
         } /* if */

      /* Tratar n�o folha � direita */

         return ARV_CondRetNaoEhFolha ;

   } /* Fim fun��o: ARV Adicionar filho � direita */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� pai
*  ****/

   ARV_tpCondRet ARV_IrPai( void )
   {

      if ( pArvore == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */
      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoPai != NULL )
      {
         pArvore->pNoCorr = pArvore->pNoCorr->pNoPai ;
         return ARV_CondRetOK ;
      } else {
         return ARV_CondRetNohEhRaiz ;
      } /* if */

   } /* Fim fun��o: ARV Ir para n� pai */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � esquerda
*  ****/

   ARV_tpCondRet ARV_IrNoEsquerda( void )
   {

      if ( pArvore == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoEsq == NULL )
      {
         return ARV_CondRetNaoPossuiFilho ;
      } /* if */

      pArvore->pNoCorr = pArvore->pNoCorr->pNoEsq ;
      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Ir para n� � esquerda */

/***************************************************************************
*
*  Fun��o: ARV Ir para n� � direita
*  ****/

   ARV_tpCondRet ARV_IrNoDireita( void )
   {

      if ( pArvore == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */

      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */

      if ( pArvore->pNoCorr->pNoDir == NULL )
      {
         return ARV_CondRetNaoPossuiFilho ;
      } /* if */

      pArvore->pNoCorr = pArvore->pNoCorr->pNoDir ;
      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Ir para n� � direita */

/***************************************************************************
*
*  Fun��o: ARV Obter valor corrente
*  ****/

   ARV_tpCondRet ARV_ObterValorCorr( char * ValorParm )
   {

      if ( pArvore == NULL )
      {
         return ARV_CondRetArvoreNaoExiste ;
      } /* if */
      if ( pArvore->pNoCorr == NULL )
      {
         return ARV_CondRetArvoreVazia ;
      } /* if */
      * ValorParm = pArvore->pNoCorr->Valor ;

      return ARV_CondRetOK ;

   } /* Fim fun��o: ARV Obter valor corrente */


/*****  C�digo das fun��es encapsuladas no m�dulo  *****/


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� da �rvore
*
*  $FV Valor retornado
*     Ponteiro para o n� criado.
*     Ser� NULL caso a mem�ria tenha se esgotado.
*     Os ponteiros do n� criado estar�o nulos e o valor � igual ao do
*     par�metro.
*
***********************************************************************/

   tpNoArvore * CriarNo( char ValorParm )
   {

      tpNoArvore * pNo ;

      pNo = ( tpNoArvore * ) malloc( sizeof( tpNoArvore )) ;
      if ( pNo == NULL )
      {
         return NULL ;
      } /* if */

      pNo->pNoPai = NULL ;
      pNo->pNoEsq = NULL ;
      pNo->pNoDir = NULL ;
	  pNo->pNoCostura = NULL;
      pNo->Valor  = ValorParm ;
      return pNo ;

   } /* Fim fun��o: ARV Criar n� da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Criar n� raiz da �rvore
*
*  $FV Valor retornado
*     ARV_CondRetOK
*     ARV_CondRetFaltouMemoria
*     ARV_CondRetNaoCriouRaiz
*
***********************************************************************/

   ARV_tpCondRet CriarNoRaiz( char ValorParm )
   {
      ARV_tpCondRet CondRet ;
      tpNoArvore * pNo ;
      if ( pArvore == NULL )
      {
         CondRet = ARV_CriarArvore( ) ;

         if ( CondRet != ARV_CondRetOK )
         {
            return CondRet ;
         } /* if */
      } /* if */

      if ( pArvore->pNoRaiz == NULL )
      {
         pNo = CriarNo( ValorParm ) ;
         if ( pNo == NULL )
         {
            return ARV_CondRetFaltouMemoria ;
         } /* if */
         pArvore->pNoRaiz = pNo ;
         pArvore->pNoCorr = pNo ;

         return ARV_CondRetOK ;
      } /* if */

      return ARV_CondRetNaoCriouRaiz ;

   } /* Fim fun��o: ARV Criar n� raiz da �rvore */


/***********************************************************************
*
*  $FC Fun��o: ARV Destruir a estrutura da �rvore
*
*  $EAE Assertivas de entradas esperadas
*     pNoArvore != NULL
*
***********************************************************************/

   void DestroiArvore( tpNoArvore * pNo )
   {

      if ( pNo->pNoEsq != NULL )
      {
         DestroiArvore( pNo->pNoEsq ) ;
      } /* if */

      if ( pNo->pNoDir != NULL )
      {
         DestroiArvore( pNo->pNoDir ) ;
      } /* if */

      free( pNo ) ;

   } /* Fim fun��o: ARV Destruir a estrutura da �rvore */

/********** Fim do m�dulo de implementa��o: M�dulo �rvore **********/

