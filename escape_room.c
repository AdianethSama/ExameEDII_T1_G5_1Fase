#include <stdio.h>
#include <stdlib.h>
#include "escape_room.h"
#include <strings.h>


//  GRAFO & MOVIMENTAÇÃO
inst *criarGrafo() {
    inst *escape_room = malloc(sizeof(inst));
    if (escape_room == NULL) {
        printf("Erro de alocacao de memoria\n");
        return NULL;
    }
    escape_room->listaSalas = NULL;
    escape_room->QntCorredores = 0;
    escape_room->QntSala = 0;
    return escape_room;
}

void adicionarSala(inst *g, int id, char nome[], char descricao[]) {
    if (g == NULL) return;
    sala *aux = g->listaSalas;
    while (aux != NULL) {
        if (aux->id == id) {
            printf("Ja existe uma sala com esse ID.\n");
            return;
        }
        aux = aux->prox;  
    }
    sala *novaSala = malloc(sizeof(sala));
    if (novaSala == NULL) {
        printf("Erro de memoria\n");
        return;
    }
    novaSala->id = id;
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->descricao, descricao);
    novaSala->temItem = 0;
    novaSala->acesso = 0;
    novaSala->adj = NULL;
    novaSala->prox = NULL;
    novaSala->visitada = 0;

    if (g->listaSalas == NULL) {
        g->listaSalas = novaSala;
    } else {
        sala *aux2 = g->listaSalas;
        while (aux2->prox != NULL) {
            aux2 = aux2->prox;  
        }
        aux2->prox = novaSala;
    }
    g->QntSala++;
}

void adicionarCorredor(inst *g, int id_origem, sala *destino, int custo) {
    if (g == NULL || destino == NULL) return;
    sala *aux = g->listaSalas;
    while (aux != NULL && aux->id != id_origem) {
        aux = aux->prox;
    }
    if (aux == NULL) {
        printf("Sala de origem nao encontrada.\n");
        return;
    }
    corredor *novoCorredor = malloc(sizeof(corredor));
    if (novoCorredor == NULL) {
        printf("Erro de memoria.\n");
        return;
    }
    novoCorredor->desbloqueado = 1;
    novoCorredor->custoEnergia = custo;
    novoCorredor->destino = destino;
    novoCorredor->prox = aux->adj;
    aux->adj = novoCorredor;
    g->QntCorredores++;
}

void entrarSala(inst *g, sala **salaAtual, int id) {
    if (g == NULL || salaAtual == NULL) {
        printf("Erro no sistema.\n");
        return;
    }
    sala *aux = g->listaSalas;
    while (aux != NULL) {
        if (aux->id == id) {
            *salaAtual = aux;
            if (!aux->visitada) aux->visitada = 1;
            printf("Jogador entrou na sala %d - %s\n", aux->id, aux->nome);
            return;
        }
        aux = aux->prox;
    }
    printf("Sala com ID %d nao existe.\n", id);
}

void mostrarMapa(inst *g) {
    if (g == NULL || g->listaSalas == NULL) {
        printf("Mapa vazio.\n");
        return;
    }
    sala *aux = g->listaSalas;
    while (aux != NULL) {
        printf("\n====================\n");
        printf("Sala %d - %s ", aux->id, aux->nome);
        if (aux->visitada) printf("[VISITADA]\n");
        else printf("[NAO VISITADA]\n");
        printf("Descricao: %s\n", aux->descricao);
        printf("Ligacoes:\n");
        corredor *c = aux->adj;
        if (c == NULL) printf("   (sem ligacoes)\n");
        while (c != NULL) {
            printf("   -> Sala %d (custo %d) [%s]\n", c->destino->id, c->custoEnergia, c->desbloqueado ? "livre" : "bloqueado");
            c = c->prox;
        }
        aux = aux->prox;
    }
}


void moverJogador(inst *g, sala **salaAtual, EstadoJogo *ej) {
    if (g == NULL || salaAtual == NULL || *salaAtual == NULL || ej == NULL) {
        printf("Erro: parâmetros inválidos no movimento.\n");
        return;
    }

    printf("\nVoce esta na sala %d - %s\n", (*salaAtual)->id, (*salaAtual)->nome);
    if ((*salaAtual)->adj == NULL) {
        printf("Esta sala nao possui corredores.\n");
        return;
    }

    corredor *c = (*salaAtual)->adj;
    printf("Voce pode ir para:\n");
    while (c != NULL) {
        printf("%d -> %s (custo %d de energia)\n", c->destino->id, c->destino->nome, c->custoEnergia);
        c = c->prox;
    }

    int salaEscolhida;
    printf("Digite o ID da sala: ");
    if (scanf("%d", &salaEscolhida) != 1) {
        printf("Entrada invalida.\n");
        return;
    }

    c = (*salaAtual)->adj;
    while (c != NULL) {
        if (c->destino->id == salaEscolhida) {
            if (c->desbloqueado == 0) {
                printf("Corredor bloqueado!\n");
                return;
            }

           
            if (!consumirEnergia(ej->energia, c->custoEnergia)) {
                printf("Movimento cancelado por falta de energia!\n");
                return;
            }

            *salaAtual = c->destino;
            ej->salaAtual = c->destino->id; 

            if (!(*salaAtual)->visitada) {
                (*salaAtual)->visitada = 1;
            }

            printf("Movimento realizado com sucesso!\n");
            return;
        }
        c = c->prox;
    }
    printf("Nao existe um corredor para essa sala.\n");
}



//  INVENTÁRIO


Inventario *criarInventario(void) {
    Inventario *inv = (Inventario *)malloc(sizeof(Inventario));
    if (!inv) { printf("Erro: sem memoria.\n"); exit(1); }
    inv->cabeca  = NULL;
    inv->tamanho = 0;
    return inv;
}

void adicionarItem(Inventario *inv, int id, const char *nome, const char *tipo, int valor) {
    if (buscarItem(inv, id)) {
        printf("[inventario] Item com id %d ja existe.\n", id);
        return;
    }
    Item *novo = (Item *)malloc(sizeof(Item));
    if (!novo) { printf("Erro: sem memoria.\n"); exit(1); }
    novo->id = id;
    strncpy(novo->nome, nome, 49); novo->nome[49] = '\0';
    strncpy(novo->tipo, tipo, 29); novo->tipo[29] = '\0';
    novo->valor   = valor;
    novo->proximo = inv->cabeca;
    inv->cabeca   = novo;
    inv->tamanho++;
    printf("[inventario] '%s' adicionado.\n", nome);
}

int removerItem(Inventario *inv, int id) {
    Item *atual    = inv->cabeca;
    Item *anterior = NULL;
    while (atual) {
        if (atual->id == id) {
            if (anterior) anterior->proximo = atual->proximo;
            else inv->cabeca = atual->proximo;
            printf("[inventario] '%s' removido.\n", atual->nome);
            free(atual);
            inv->tamanho--;
            return 1;
        }
        anterior = atual;
        atual    = atual->proximo;
    }
    printf("[inventario] Item %d nao encontrado.\n", id);
    return 0;
}

Item *buscarItem(Inventario *inv, int id) {
    Item *atual = inv->cabeca;
    while (atual) {
        if (atual->id == id) return atual;
        atual = atual->proximo;
    }
    return NULL;
}

void listarInventario(const Inventario *inv) {
    if (!inv->cabeca) { printf("[inventario] Vazio.\n"); return; }
    printf("\n=== Inventario (%d item(s)) ===\n", inv->tamanho);
    Item *atual = inv->cabeca;
    while (atual) {
        printf("  [%d] %-20s | tipo: %-12s | valor: %d\n", atual->id, atual->nome, atual->tipo, atual->valor);
        atual = atual->proximo;
    }
    printf("================================\n\n");
}

void destruirInventario(Inventario *inv) {
    Item *atual = inv->cabeca;
    while (atual) {
        Item *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    free(inv);
}



//ÁRVORE AVL


static int altura(NoAVL *n) { return n == NULL ? 0 : n->altura; }
static int max2(int a, int b) { return a > b ? a : b; }
static int fatorBalanco(NoAVL *n) { return n == NULL ? 0 : altura(n->esq) - altura(n->dir); }
static void atualizarAltura(NoAVL *n) { n->altura = 1 + max2(altura(n->esq), altura(n->dir)); }

static NoAVL *novoNo(int chave, const char *nome, int qtd) {
    NoAVL *n = (NoAVL *)malloc(sizeof(NoAVL));
    if (!n) { printf("Erro: sem memoria.\n"); exit(1); }
    n->chave      = chave;
    n->quantidade = qtd;
    n->altura     = 1;
    n->esq = n->dir = NULL;
    strncpy(n->nome, nome, 49); n->nome[49] = '\0';
    return n;
}

static NoAVL *rotDir(NoAVL *y) {
    NoAVL *x  = y->esq; NoAVL *T2 = x->dir;
    x->dir = y; y->esq = T2;
    atualizarAltura(y); atualizarAltura(x);
    return x;
}

static NoAVL *rotEsq(NoAVL *x) {
    NoAVL *y  = x->dir; NoAVL *T2 = y->esq;
    y->esq = x; x->dir = T2;
    atualizarAltura(x); atualizarAltura(y);
    return y;
}

static NoAVL *balancear(NoAVL *n) {
    atualizarAltura(n);
    int fb = fatorBalanco(n);
    if (fb > 1 && fatorBalanco(n->esq) >= 0) return rotDir(n);
    if (fb > 1 && fatorBalanco(n->esq) < 0) { n->esq = rotEsq(n->esq); return rotDir(n); }
    if (fb < -1 && fatorBalanco(n->dir) <= 0) return rotEsq(n);
    if (fb < -1 && fatorBalanco(n->dir) > 0) { n->dir = rotDir(n->dir); return rotEsq(n); }
    return n;
}

NoAVL *inserirAVL(NoAVL *raiz, int chave, const char *nome, int quantidade) {
    if (raiz == NULL) return novoNo(chave, nome, quantidade);
    if (chave < raiz->chave) raiz->esq = inserirAVL(raiz->esq, chave, nome, quantidade);
    else if (chave > raiz->chave) raiz->dir = inserirAVL(raiz->dir, chave, nome, quantidade);
    else {
        raiz->quantidade += quantidade;
        printf("[AVL] '%s' atualizado - qtd total: %d\n", nome, raiz->quantidade);
        return raiz;
    }
    return balancear(raiz);
}

NoAVL *removerAVL(NoAVL *raiz, int chave) {
    if (raiz == NULL) { printf("[AVL] Chave %d nao encontrada.\n", chave); return NULL; }
    if (chave < raiz->chave) raiz->esq = removerAVL(raiz->esq, chave);
    else if (chave > raiz->chave) raiz->dir = removerAVL(raiz->dir, chave);
    else {
        printf("[AVL] '%s' removido.\n", raiz->nome);
        if (raiz->esq == NULL || raiz->dir == NULL) {
            NoAVL *tmp = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return tmp;
        }
        NoAVL *suc = raiz->dir;
        while (suc->esq) suc = suc->esq;
        raiz->chave      = suc->chave;
        raiz->quantidade = suc->quantidade;
        strncpy(raiz->nome, suc->nome, 49);
        raiz->dir = removerAVL(raiz->dir, suc->chave);
    }
    return balancear(raiz);
}

NoAVL *procurarAVL(NoAVL *raiz, int chave) {
    if (raiz == NULL || raiz->chave == chave) return raiz;
    if (chave < raiz->chave) return procurarAVL(raiz->esq, chave);
    return procurarAVL(raiz->dir, chave);
}

void mostrarAVL(const NoAVL *raiz) {
    if (raiz == NULL) return;
    mostrarAVL(raiz->esq);
    printf("  [%d] %-20s qtd: %d\n", raiz->chave, raiz->nome, raiz->quantidade);
    mostrarAVL(raiz->dir);
}

void mostrarAVLArvore(const NoAVL *raiz, int nivel) {
    if (raiz == NULL) return;
    mostrarAVLArvore(raiz->dir, nivel + 1);
    int i;
    for (i = 0; i < nivel; i++) printf("    ");
    printf("[%d] %s (h=%d)\n", raiz->chave, raiz->nome, raiz->altura);
    mostrarAVLArvore(raiz->esq, nivel + 1);
}

void destruirAVL(NoAVL *raiz) {
    if (raiz == NULL) return;
    destruirAVL(raiz->esq);
    destruirAVL(raiz->dir);
    free(raiz);
}



// ENERGIA


Energia *criarEnergia(int maximo, int taxaConsumo) {
    Energia *e = (Energia *)malloc(sizeof(Energia));
    if (!e) { printf("Erro: sem memoria.\n"); exit(1); }
    e->maximo      = maximo;
    e->atual       = maximo;
    e->taxaConsumo = taxaConsumo;
    printf("[energia] Sistema de energia inicializado (max: %d, consumo: %d/passo).\n", maximo, taxaConsumo);
    return e;
}

int consumirEnergia(Energia *e, int quantidade) {
    if (e->atual < quantidade) {
        printf("[energia] Energia insuficiente! Necessario: %d | Disponivel: %d\n", quantidade, e->atual);
        return 0;
    }
    e->atual -= quantidade;
    printf("[energia] Consumidos %d pontos de energia. Restante: %d/%d\n", quantidade, e->atual, e->maximo);
    return 1;
}

void recarregarEnergia(Energia *e, int quantidade) {
    int anterior = e->atual;
    e->atual += quantidade;
    if (e->atual > e->maximo) e->atual = e->maximo;
    printf("[energia] Energia recarregada: %d -> %d (max: %d)\n", anterior, e->atual, e->maximo);
}

void mostrarEnergia(const Energia *e) {
    printf("\n=== Energia ===\n");
    printf("  Atual   : %d\n", e->atual);
    printf("  Maximo  : %d\n", e->maximo);
    printf("  Consumo : %d/passo\n", e->taxaConsumo);
    printf("===============\n\n");
}

void salvarEnergia(const Energia *e, const char *arquivo) {
    FILE *f = fopen(arquivo, "w");
    if (!f) return;
    fprintf(f, "%d %d %d\n", e->atual, e->maximo, e->taxaConsumo);
    fclose(f);
}

void carregarEnergia(Energia *e, const char *arquivo) {
    FILE *f = fopen(arquivo, "r");
    if (!f) return;
    if (fscanf(f, "%d %d %d", &e->atual, &e->maximo, &e->taxaConsumo) != 3) {
         printf("[energia] Erro ao ler arquivo de energia.\n");
    }
    fclose(f);
}

void destruirEnergia(Energia *e) { free(e); }




