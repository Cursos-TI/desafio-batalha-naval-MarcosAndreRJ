#include <stdio.h>
#include <stdlib.h> // para rand() e srand() -- inicializar o gerador de números aleatórios
#include <time.h> // pegar o empo atual do sistema
#include <ctype.h>
// toupper() → converter letra minúscula em maiúscula.
// tolower() → converter maiúscula em minúscula.
// isdigit(), isalpha(), etc., para testar tipos de caracteres.

#define N 10

// Imprime o tabuleiro
void imprimirTabuleiro(int matriz[N][N], int shotRow, int shotCol) {
    printf("\n    ");
    for (char letra = 'A'; letra < 'A' + N; letra++) {
        printf(" %c ", letra);
    }
    printf("\n");

    for (int i = 0; i < N; i++) {
        printf("%2d |", i + 1);
        for (int j = 0; j < N; j++) {
            if (i == shotRow && j == shotCol) {
                printf(" \033[31mX\033[0m ");
            } else {
                int val = matriz[i][j];
                if (val == 0) {
                    printf(" . "); // com "." fica mais bonito que "0"
                // se clipper já tinha cor, pq fazer em preto e branco?
                } else if (val == 1) {
                    printf("\033[32m 1 \033[0m");
                } else if (val == 2) {
                    printf("\033[33m 2 \033[0m");
                } else if (val == 3) {
                    printf("\033[31m 3 \033[0m");
                } else {
                    printf(" ? ");
                }
            }
        }
        printf("|\n");
    }
    printf("    ");
    for (char letra = 'A'; letra < 'A' + N; letra++) {
        printf("---");
    }
    printf("\n");
}

// Zerando a matriz
void zerarMatriz(int matriz[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matriz[i][j] = 0;
}

// Verifica se espaço está livre para figura
int podeColocar(int matriz[N][N], int fig[5][5], int rows, int cols, int rowOffset, int colOffset) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fig[i][j]) {
                int r = rowOffset + i;
                int c = colOffset + j;
                if (r < 0 || r >= N || c < 0 || c >= N)
                    return 0;
                if (matriz[r][c] != 0)
                    return 0;
            }
        }
    }
    return 1;
}

// Coloca a figura
void colocaFigura(int matriz[N][N], int fig[5][5], int rows, int cols, int rowOffset, int colOffset, int val) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (fig[i][j]) {
                matriz[rowOffset + i][colOffset + j] = val;
            }
        }
    }
}

// Funções para gerar padrões
void geraCone(int fig[5][5]) {
    int pattern[3][5] = {
        {0,0,1,0,0},
        {0,1,1,1,0},
        {1,1,1,1,1}
    };
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
            fig[i][j] = (i<3 && j<5) ? pattern[i][j] : 0;
}

void geraOctaedro(int fig[5][5]) {
    int pattern[5][5] = {
        {0,0,1,0,0},
        {0,1,1,1,0},
        {1,1,1,1,1},
        {0,1,1,1,0},
        {0,0,1,0,0}
    };
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
            fig[i][j] = pattern[i][j];
}

void geraCruz(int fig[5][5]) {
    int pattern[5][5] = {
        {0,0,1,0,0},
        {0,0,1,0,0},
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0}
    };
    for (int i=0; i<5; i++)
        for (int j=0; j<5; j++)
            fig[i][j] = pattern[i][j];
}

int main() {
    int tabuleiro[N][N];
    int coneFig[5][5], octFig[5][5], cruzFig[5][5];

    zerarMatriz(tabuleiro);

    geraCone(coneFig);
    geraOctaedro(octFig);
    geraCruz(cruzFig);

    srand(time(NULL));

    int row, col;
    // Cone
    do {
        row = rand() % (N - 3);
        col = rand() % (N - 5);
    } while (!podeColocar(tabuleiro, coneFig, 3, 5, row, col));
    colocaFigura(tabuleiro, coneFig, 3, 5, row, col, 1);

    // Octaedro
    do {
        row = rand() % (N - 5);
        col = rand() % (N - 5);
    } while (!podeColocar(tabuleiro, octFig, 5, 5, row, col));
    colocaFigura(tabuleiro, octFig, 5, 5, row, col, 2);

    // Cruz
    do {
        row = rand() % (N - 5);
        col = rand() % (N - 5);
    } while (!podeColocar(tabuleiro, cruzFig, 5, 5, row, col));
    colocaFigura(tabuleiro, cruzFig, 5, 5, row, col, 3);

    printf("=== BATALHA NAVAL ===\n");
    printf("Legenda:\n");
    printf(" . = água\n");
    printf("\033[32m1\033[0m = cone\n");
    printf("\033[33m2\033[0m = octaedro\n");
    printf("\033[31m3\033[0m = cruz\n");
    printf("X = seu tiro\n");

    int acertou = 0;
    int shotRow, shotCol;
    do {
        char letra;
        int num;
        printf("\nDigite a letra da coluna (A-J): ");
        scanf(" %c", &letra);
        letra = toupper(letra);

        printf("Digite o número da linha (1-10): ");
        scanf("%d", &num);

        if (letra < 'A' || letra > 'J' || num < 1 || num > 10) {
            printf("Jogada inválida!\n");
            continue;
        }

        shotCol = letra - 'A';
        shotRow = num - 1;

        if (tabuleiro[shotRow][shotCol] != 0) {
            printf("\n\033[32mACERTOU!\033[0m\n");
            acertou = 1;
        } else {
            printf("\n\033[36mÁGUA!\033[0m\n");
        }

        imprimirTabuleiro(tabuleiro, shotRow, shotCol);

    } while (!acertou);

    printf("Parabéns! Você afundou um pedaço do navio!\n");

    return 0;
}
