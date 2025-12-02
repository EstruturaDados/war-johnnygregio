#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Constantes ---
#define MAX_TERRITORIOS 5
#define MAX_STRING 50

// --- Estrutura de Dados ---

/**
 * @brief Estrutura de territórios no mapa.
 */
typedef struct {
    char nome[MAX_STRING];
    char cor_exercito[MAX_STRING];
    int tropas;
} Territorio;

// --- Funções ---
void cadastrarTerritorios(Territorio mapa[], int tamanho);
void exibirMapa(const Territorio mapa[], int tamanho);
void limparBufferEntrada(void);

// --- Principal (main) ---

int main() {
    // 1. Vetor Estático de Territórios
    // O vetor 'mapa' pode armazenar MAX_TERRITORIOS (5) elementos do tipo Territorio.
    Territorio mapa[MAX_TERRITORIOS];

    printf("=======================================================\n");
    printf("         CADASTRO INICIAL DO MAPA DE TERRITÓRIOS\n");
    printf("=======================================================\n");

    // 2. Leitura dos Dados
    cadastrarTerritorios(mapa, MAX_TERRITORIOS);

    // 3. Exibição do Mapa
    exibirMapa(mapa, MAX_TERRITORIOS);

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Consome os caracteres restantes
    }
}

/**
 * @brief Solicita e armazena os território.
 * @param mapa O vetor de territórios (passagem por referência).
 * @param tamanho O número de territórios a cadastrar (5).
 */
void cadastrarTerritorios(Territorio mapa[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\n--- Território %d de %d ---\n", i + 1, tamanho);

        // 1. Entrada do Nome (Usando fgets para capturar strings com espaços)
        printf("Nome do Território: ");
        // fgets lê até MAX_STRING ou até encontrar '\n', guardando o '\n'
        if (fgets(mapa[i].nome, MAX_STRING, stdin) == NULL) {
            printf("Erro na leitura.\n");
            return;
        }
        // Remove o '\n' que o fgets adiciona ao final da string
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';


        // 2. Entrada da Cor do Exército (Usando fgets)
        printf("Cor do Exército Dominante: ");
        if (fgets(mapa[i].cor_exercito, MAX_STRING, stdin) == NULL) {
            printf("Erro na leitura.\n");
            return;
        }
        mapa[i].cor_exercito[strcspn(mapa[i].cor_exercito, "\n")] = '\0';


        // 3. Entrada do Número de Tropas (Usando scanf)
        printf("Número de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
             printf("Entrada inválida. Definindo Tropas para 0.\n");
             mapa[i].tropas = 0;
        }
        limparBufferEntrada(); // Limpa o buffer após o scanf
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios
 * @param mapa O vetor de territórios (passagem por referência constante).
 * @param tamanho O número de territórios a exibir.
 */
void exibirMapa(const Territorio mapa[], int tamanho) {
    printf("\n\n=======================================================\n");
    printf("               ESTADO ATUAL DO MAPA\n");
    printf("=======================================================\n");

    // Cabeçalho da Tabela
    printf("| %-20s | %-20s | %-10s |\n", "Território", "Exército", "Tropas");
    printf("|----------------------|----------------------|------------|\n");

    // Conteúdo da Tabela
    for (int i = 0; i < tamanho; i++) {
        printf("| %-20s | %-20s | %-10d |\n", 
               mapa[i].nome, 
               mapa[i].cor_exercito, 
               mapa[i].tropas);
    }
    printf("|----------------------|----------------------|------------|\n");
}