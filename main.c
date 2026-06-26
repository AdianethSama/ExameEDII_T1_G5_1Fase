#include <stdio.h>
#include <stdlib.h>
#include "escape_room.h"

inst       *mapa;
sala       *salaAtual;
Inventario *inv;
NoAVL      *avl;
EstadoJogo  ej;

void inicializar(void) {
    sala *s1, *s2, *s3, *s4;

    mapa = criarGrafo();
    adicionarSala(mapa, 1, "Entrada",     "Portao principal");
    adicionarSala(mapa, 2, "Corredor A",  "Corredor norte");
    adicionarSala(mapa, 3, "Laboratorio", "Sala de pesquisa");
    adicionarSala(mapa, 4, "Arquivo",     "Sala de dados");

    s1 = mapa->listaSalas;
    s2 = s1->prox;
    s3 = s2->prox;
    s4 = s3->prox;

    adicionarCorredor(mapa, 1, s2, 5);
    adicionarCorredor(mapa, 2, s1, 5);
    adicionarCorredor(mapa, 2, s3, 8);
    adicionarCorredor(mapa, 3, s2, 8);
    adicionarCorredor(mapa, 3, s4, 3);
    adicionarCorredor(mapa, 4, s3, 3);

    salaAtual = mapa->listaSalas;

    inv = criarInventario();
    adicionarItem(inv, 1, "Cartao de Acesso",  "chave",   10);
    adicionarItem(inv, 2, "Bateria Extra",      "energia", 50);
    adicionarItem(inv, 3, "Mapa da Instalacao", "mapa",     5);

    avl = NULL;
    avl = inserirAVL(avl, 10, "Combustivel", 5);
    avl = inserirAVL(avl, 20, "Medicamento", 3);
    avl = inserirAVL(avl, 5,  "Agua",        12);
    avl = inserirAVL(avl, 15, "Ferramenta",  7);
    avl = inserirAVL(avl, 25, "Municao",     20);

    ej.energia          = criarEnergia(100, 2);
    ej.salaAtual        = 1;
    ej.nivelAcesso      = 1;
    ej.portasAbertas    = 0;
    ej.portasParaVencer = 0;
    ej.portas           = NULL;
}

void menuGrafo(void) {
    int op;
    do {
        printf("\n--- GRAFO ---\n");
        printf("1. Mostrar mapa\n");
        printf("2. Mover jogador\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        if (op == 1) mostrarMapa(mapa);
        else if (op == 2) moverJogador(mapa, &salaAtual, &ej);
    } while (op != 0);
}

void menuInventario(void) {
    int op, id, valor;
    char nome[50], tipo[30];
    do {
        printf("\n--- INVENTARIO ---\n");
        printf("1. Listar itens\n");
        printf("2. Adicionar item\n");
        printf("3. Remover item\n");
        printf("4. Buscar item\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        if (op == 1) {
            listarInventario(inv);
        } else if (op == 2) {
            printf("ID: ");     scanf("%d", &id);
            printf("Nome: ");   scanf("%s", nome);
            printf("Tipo: ");   scanf("%s", tipo);
            printf("Valor: ");  scanf("%d", &valor);
            adicionarItem(inv, id, nome, tipo, valor);
        } else if (op == 3) {
            printf("ID a remover: "); scanf("%d", &id);
            removerItem(inv, id);
        } else if (op == 4) {
            printf("ID a buscar: "); scanf("%d", &id);
            Item *it = buscarItem(inv, id);
            if (it) printf("  [%d] %s | tipo: %s | valor: %d\n", it->id, it->nome, it->tipo, it->valor);
            else    printf("  Item nao encontrado.\n");
        }
    } while (op != 0);
}

void menuAVL(void) {
    int op, chave, qtd;
    char nome[50];
    do {
        printf("\n--- ARVORE AVL ---\n");
        printf("1. Listar recursos (em ordem)\n");
        printf("2. Mostrar arvore visual\n");
        printf("3. Inserir recurso\n");
        printf("4. Remover recurso\n");
        printf("5. Buscar recurso\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        if (op == 1) {
            mostrarAVL(avl);
        } else if (op == 2) {
            mostrarAVLArvore(avl, 0);
        } else if (op == 3) {
            printf("Chave: ");      scanf("%d", &chave);
            printf("Nome: ");       scanf("%s", nome);
            printf("Quantidade: "); scanf("%d", &qtd);
            avl = inserirAVL(avl, chave, nome, qtd);
        } else if (op == 4) {
            printf("Chave a remover: "); scanf("%d", &chave);
            avl = removerAVL(avl, chave);
        } else if (op == 5) {
            printf("Chave a buscar: "); scanf("%d", &chave);
            NoAVL *r = procurarAVL(avl, chave);
            if (r) printf("  [%d] %s | qtd: %d\n", r->chave, r->nome, r->quantidade);
            else   printf("  Recurso nao encontrado.\n");
        }
        system("pause");
        system("cls");
    } while (op != 0);
}

void menuEnergia(void) {
    int op, qtd;
    do {
        printf("\n--- ENERGIA ---\n");
        printf("1. Ver energia\n");
        printf("2. Consumir energia\n");
        printf("3. Recarregar energia\n");
        printf("4. Guardar estado\n");
        printf("5. Carregar estado\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
        scanf("%d", &op);
        if (op == 1) {
            mostrarEnergia(ej.energia);
        } else if (op == 2) {
            printf("Quantidade a consumir: "); scanf("%d", &qtd);
            consumirEnergia(ej.energia, qtd);
        } else if (op == 3) {
            printf("Quantidade a recarregar: "); scanf("%d", &qtd);
            recarregarEnergia(ej.energia, qtd);
        } else if (op == 4) {
            salvarEnergia(ej.energia, "energia_save.txt");
        } else if (op == 5) {
            carregarEnergia(ej.energia, "energia_save.txt");
        }
    } while (op != 0);
}

int main(void) {
    int op;
    inicializar();

    do {
        printf("\n========================================\n");
        printf("         PROJETO ORION - MENU           \n");
        printf("========================================\n");
        printf("  Sala actual: %d | Energia: %d/%d\n",
               ej.salaAtual, ej.energia->atual, ej.energia->maximo);
        printf("----------------------------------------\n");
        printf("1. Grafo e Navegacao\n");
        printf("2. Inventario\n");
        printf("3. Arvore AVL\n");
        printf("4. Energia\n");
        printf("0. Sair\n");
        printf("========================================\n");
        printf("Opcao: ");
        scanf("%d", &op);

        if      (op == 1) menuGrafo();
        else if (op == 2) menuInventario();
        else if (op == 3) menuAVL();
        else if (op == 4) menuEnergia();

    } while (op != 0);

    destruirAVL(avl);
    destruirInventario(inv);
    destruirEnergia(ej.energia);

    sala *s = mapa->listaSalas;
    while (s != NULL) {
        corredor *c = s->adj;
        while (c != NULL) { corredor *prox = c->prox; free(c); c = prox; }
        sala *prox = s->prox; free(s); s = prox;
    }
    free(mapa);

    printf("\nAte logo!\n");
    return 0;
}
