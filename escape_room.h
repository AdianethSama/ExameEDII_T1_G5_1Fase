#ifndef ESCAPE_ROOM_H
#define ESCAPE_ROOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PORTAS 50
#define MAX_SALAS  100
#define INF         99999
#define ARQUIVO_ESTADO "estado_jogo.txt"


//  INVENTÁRIO 

typedef struct Item {
    int id;
    char nome[50];
    char tipo[30];
    int valor;
    struct Item *proximo;
} Item;

typedef struct {
    Item *cabeca;
    int tamanho;
} Inventario;


// ÁRVORE AVL 

typedef struct NoAVL {
    int chave;
    char nome[50];
    int quantidade;
    int altura;
    struct NoAVL *esq, *dir;
} NoAVL;

//  GRAFO (MAPA)

typedef struct aresta corredor;
typedef struct vertice sala;

typedef struct aresta {
    int custoEnergia;
    int desbloqueado;
    sala *destino;
    struct aresta *prox;
} corredor;

typedef struct vertice {
    int id;
    char nome[20];
    char descricao[30];
    int acesso;
    int temItem;
    int visitada;
    corredor *adj;
    struct vertice *prox;   
} sala;

typedef struct grafos {
    int QntSala;
    int QntCorredores;
    sala *listaSalas;
} inst;

// SISTEMA DE ENERGIA E PORTAS

typedef struct {
    int atual;
    int maximo;
    int taxaConsumo;
} Energia;

typedef struct {
    char pergunta[200];
    char resposta[100];
    char dica[150];
} Enigma;

typedef struct {
    int id;
    int nivelAcesso;
    int aberta;
    int temEnigma;
    int itemNecessarioId;
    Enigma *enigma;
} Porta;

typedef struct {
    Porta *portas[MAX_PORTAS];
    int quantidade;
} ListaPortas;

typedef struct {
    int salas[MAX_SALAS];
    int tamanho;
    int custoTotal;
} Caminho;

typedef struct {
    int salaAtual;
    int nivelAcesso;
    Energia *energia;
    ListaPortas *portas;
    int portasParaVencer;
    int portasAbertas;
} EstadoJogo;





// Grafo 
inst *criarGrafo(void);
void adicionarSala(inst *g, int id, char nome[], char descricao[]);
void adicionarCorredor(inst *g, int id_origem, sala *destino, int custo);
void entrarSala(inst *g, sala **salaAtual, int id);
void mostrarMapa(inst *g);
void moverJogador(inst *g, sala **salaAtual, EstadoJogo *ej);

// Inventário
Inventario *criarInventario(void);
void adicionarItem(Inventario *inv, int id, const char *nome, const char *tipo, int valor);
int removerItem(Inventario *inv, int id);
Item *buscarItem(Inventario *inv, int id);
void listarInventario(const Inventario *inv);
void destruirInventario(Inventario *inv);

// AVL
NoAVL *inserirAVL(NoAVL *raiz, int chave, const char *nome, int quantidade);
NoAVL *removerAVL(NoAVL *raiz, int chave);
NoAVL *procurarAVL(NoAVL *raiz, int chave);
void mostrarAVL(const NoAVL *raiz);
void mostrarAVLArvore(const NoAVL *raiz, int nivel);
void destruirAVL(NoAVL *raiz);

// Energia
Energia *criarEnergia(int maximo, int taxaConsumo);
int consumirEnergia(Energia *e, int quantidade);
void recarregarEnergia(Energia *e, int quantidade);
void mostrarEnergia(const Energia *e);
void salvarEnergia(const Energia *e, const char *arquivo);
void carregarEnergia(Energia *e, const char *arquivo);
void destruirEnergia(Energia *e);

#endif
