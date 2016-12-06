#include <stdio.h>
#include <stdlib.h>

struct nodo{
  int x;
  int y;
  struct nodo *next;
};

struct nodo *top=NULL;

void stackInit(){
}

struct nodo *stackPush(int x,int y){
  struct nodo *aux=(struct nodo *)malloc(sizeof(struct nodo));
  if(aux!=NULL){
    aux->x=x;
    aux->y=y;
    aux->next=top;
  }
  return aux;
}

void stackPop(){
  if(top!=NULL){
    struct nodo *aux=top;
    top=top->next;
    free(aux);
  }
}

void stackPrint(){
  if(top!=NULL){
    struct nodo *aux=top;
    while(aux!=NULL){
      printf("%d,%d ",aux->x,aux->y);
      aux=aux->next;
    }
  }
}

void stackEmpty(){
  if(top!=NULL){
    struct nodo *aux;
    while(top!=NULL){
      aux=top;
      top=top->next;
      free(aux);
    }
  }
}

int stackIsEmpty(){
  return top==NULL?1:0;
}

int stackElements(){
  int i=0;
  struct nodo *aux=top;
  while(aux!=NULL){
    aux=aux->next;
    i++;
  }
  return i;
}
  

/*
int main(){
  int i=0,j=10;
  for(i=0;i<10;i++){
    top=stackPush(i,j--);
  }
  stackPrint();
  printf("\n");
  stackPop();
  stackPop();
  stackPop();
  stackPrint();
  printf("\n");
  stackEmpty();
  stackPrint();
  printf("\n");
  return 0;
}
*/ 
