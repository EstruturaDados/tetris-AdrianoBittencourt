#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5      // Tamanho da fila de peças
#define TAM_PILHA 3     // Capacidade da pilha de reserva

// Estrutura de cada peça
typedef struct {
    char tipo; // 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

/*============================================================
    FILA CIRCULAR DE PEÇAS
============================================================*/
typedef struct {
    Peca pecas[TAM_FILA];
    int inicio;
    int fim;
    int count;
} Fila;

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->count = 0;
}

// Adiciona peça ao final da fila (enqueue)
void enqueue(Fila *f, Peca p) {
    if(f->count == TAM_FILA) {
        printf("Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    f->pecas[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->count++;
}

// Remove peça da frente da fila (dequeue)
Peca dequeue(Fila *f) {
    Peca vazio = {' ', -1};
    if(f->count == 0) return vazio;

    Peca p = f->pecas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->count--;
    return p;
}

// Exibe a fila de peças
void mostrarFila(Fila *f) {
    printf("Fila de peças:\t");
    for(int i = 0; i < f->count; i++) {
        int idx = (f->inicio + i) % TAM_FILA;
        printf("[%c %d] ", f->pecas[idx].tipo, f->pecas[idx].id);
    }
    printf("\n");
}

/*============================================================
    PILHA DE RESERVA
============================================================*/
typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Adiciona peça no topo da pilha
int push(Pilha *p, Peca peça) {
    if(p->topo == TAM_PILHA - 1) return 0; // pilha cheia
    p->topo++;
    p->pecas[p->topo] = peça;
    return 1;
}

// Remove peça do topo da pilha
Peca pop(Pilha *p) {
    Peca vazio = {' ', -1};
    if(p->topo == -1) return vazio;
    return p->pecas[p->topo--];
}

// Exibe a pilha de reserva
void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\t");
    for(int i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].tipo, p->pecas[i].id);
    }
    printf("\n");
}

/*============================================================
    FUNÇÃO PARA GERAR PEÇAS ALEATÓRIAS
============================================================*/
char tipos[] = {'I', 'O', 'T', 'L'};
int idGlobal = 0;

Peca gerarPeca() {
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = idGlobal++;
    return p;
}

/*============================================================
    FUNÇÃO PRINCIPAL
============================================================*/
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial
    for(int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n=== Estado Atual ===\n");
        mostrarFila(&fila);
        mostrarPilha(&pilha);

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        if(opcao == 1) {
            Peca jogada = dequeue(&fila);
            if(jogada.id != -1) {
                printf("Jogou a peça [%c %d]\n", jogada.tipo, jogada.id);
                enqueue(&fila, gerarPeca());
            } else {
                printf("Fila vazia! Nenhuma peça para jogar.\n");
            }
        } else if(opcao == 2) {
            Peca frente = dequeue(&fila);
            if(frente.id != -1) {
                if(push(&pilha, frente)) {
                    printf("Reservou a peça [%c %d]\n", frente.tipo, frente.id);
                    enqueue(&fila, gerarPeca());
                } else {
                    printf("Pilha de reserva cheia! Não foi possível reservar.\n");
                    // Se não couber, devolve à fila
                    enqueue(&fila, frente);
                }
            } else {
                printf("Fila vazia! Nenhuma peça para reservar.\n");
            }
        } else if(opcao == 3) {
            Peca usada = pop(&pilha);
            if(usada.id != -1) {
                printf("Usou a peça reservada [%c %d]\n", usada.tipo, usada.id);
            } else {
                printf("Pilha de reserva vazia! Nenhuma peça para usar.\n");
            }
        } else if(opcao != 0) {
            printf("Opção inválida! Tente novamente.\n");
        }

    } while(opcao != 0);

    printf("Encerrando o jogo...\n");

    return 0;
}

