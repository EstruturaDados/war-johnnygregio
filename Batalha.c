#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

// --- Constantes ---
#define MAX_STRING 50
#define MAX_COR 10

// ============================================================================
// --- Estrutura de Dados ---
// ============================================================================

/**
 * @brief Estrutura que representa um território no mapa.
 */
typedef struct {
    char nome[MAX_STRING];
    char cor[MAX_COR]; // Cor do exército dominante
    int tropas;
} Territorio;

// ============================================================================
// --- Protótipos de Funções ---
// ============================================================================

// Gerenciamento de Memória
Territorio* alocarMapa(int tamanho);
void liberarMemoria(Territorio* mapa);

// Setup e Exibição
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void exibirMapa(const Territorio* mapa, int tamanho);

// Lógica do Jogo
void faseDeAtaque(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);

// Utilitárias
void limparBufferEntrada(void);
int rolarDado(void);

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================

int main() {
    // Inicializa a semente para garantir aleatoriedade nas rolagens de dado
    srand((unsigned int)time(NULL)); 
    
    int num_territorios = 0;
    Territorio* mapa = NULL; 

    printf("=======================================================\n");
    printf("         WAR ESTRUTURADO - SIMULACAO DE BATALHA\n");
    printf("=======================================================\n");

    // Solicita o número de territórios
    printf("Informe o numero total de territorios para o mapa: ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios <= 0) {
        printf("ERRO: Numero de territorios invalido. Encerrando.\n");
        return 1;
    }
    limparBufferEntrada();
    
    // Alocação Dinâmica
    mapa = alocarMapa(num_territorios);
    if (mapa == NULL) {
        printf("ERRO: Falha ao alocar memoria. Encerrando o programa.\n");
        return 1;
    }

    // Cadastro
    cadastrarTerritorios(mapa, num_territorios);

    // Loop Principal
    int escolha = -1;
    do {
        exibirMapa(mapa, num_territorios);
        printf("\n--- Menu de Acoes ---\n");
        printf("1. Iniciar Ataque\n");
        printf("0. Sair do Jogo\n");
        printf("Sua escolha: ");
        
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1; 
        } else {
            limparBufferEntrada();
        }

        if (escolha == 1) {
            faseDeAtaque(mapa, num_territorios);
        } else if (escolha != 0) {
            printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (escolha != 0);

    // Liberação de Memória
    liberarMemoria(mapa);
    printf("\nMemoria liberada. Programa finalizado.\n");

    return 0;
}

// ============================================================================
// --- Implementação das Funções de Gerenciamento de Memória ---
// ============================================================================

/**
 * @brief Aloca dinamicamente a memoria para o vetor de territorios usando calloc.
 * @param tamanho O numero de elementos a alocar.
 * @return Um ponteiro para o bloco de memoria alocado.
 */
Territorio* alocarMapa(int tamanho) {
    Territorio* mapa = (Territorio*)calloc(tamanho, sizeof(Territorio));
    return mapa;
}

/**
 * @brief Libera a memoria alocada para o mapa.
 * @param mapa Ponteiro para o bloco de memoria a ser liberado.
 */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

// ============================================================================
// --- Implementação das Funções de Setup e Exibição ---
// ============================================================================

/**
 * @brief Solicita e armazena os dados de cada territorio.
 * @param mapa O ponteiro para o vetor de territorios.
 * @param tamanho O numero de territorios a cadastrar.
 */
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\n--- Cadastro do Territorio %d de %d ---\n", i + 1, tamanho);

        // Entrada do Nome
        printf("Nome do Territorio: ");
        if (fgets(mapa[i].nome, MAX_STRING, stdin) == NULL) return;
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Entrada da Cor do Exercito
        printf("Cor do Exercito Dominante: ");
        if (fgets(mapa[i].cor, MAX_COR, stdin) == NULL) return;
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        
        // Entrada do Numero de Tropas
        printf("Numero de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas <= 0) {
             printf("Tropas invalidas. Definindo para 1.\n");
             mapa[i].tropas = 1;
        }
        limparBufferEntrada(); 
    }
}

/**
 * @brief Exibe o estado atual de todos os territorios em formato de tabela.
 * @param mapa O ponteiro constante para o vetor de territorios.
 * @param tamanho O numero de territorios a exibir.
 */
void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n\n=======================================================\n");
    printf("               ESTADO ATUAL DO MAPA\n");
    printf("=======================================================\n");

    printf("| %-3s | %-20s | %-10s | %-10s |\n", "ID", "Territorio", "Exercito", "Tropas");
    printf("|-----|----------------------|------------|------------|\n");

    for (int i = 0; i < tamanho; i++) {
        printf("| %-3d | %-20s | %-10s | %-10d |\n", 
               i + 1, // ID baseado em 1 para o usuario
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("|-----|----------------------|------------|------------|\n");
}

// ============================================================================
// --- Implementação das Funções de Lógica do Jogo ---
// ============================================================================

/**
 * @brief Rola um dado de 6 faces (1 a 6).
 * @return O resultado da rolagem.
 */
int rolarDado(void) {
    return (rand() % 6) + 1;
}

/**
 * @brief Gerencia a interface de ataque e valida as escolhas do jogador.
 * @param mapa O ponteiro para o vetor de territorios.
 * @param tamanho O numero total de territorios.
 */
void faseDeAtaque(Territorio* mapa, int tamanho) {
    int id_atacante, id_defensor;
    
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do Territorio Atacante (1 a %d): ", tamanho);
    if (scanf("%d", &id_atacante) != 1 || id_atacante < 1 || id_atacante > tamanho) {
        printf("Erro: ID de atacante invalido.\n");
        limparBufferEntrada();
        return;
    }

    printf("Digite o ID do Territorio Defensor (1 a %d): ", tamanho);
    if (scanf("%d", &id_defensor) != 1 || id_defensor < 1 || id_defensor > tamanho) {
        printf("Erro: ID de defensor invalido.\n");
        limparBufferEntrada();
        return;
    }
    limparBufferEntrada();

    // Converte ID (base 1) para indice (base 0)
    int i_atacante = id_atacante - 1;
    int i_defensor = id_defensor - 1;

    // --- Validacoes ---
    if (i_atacante == i_defensor) {
        printf("Ataque cancelado: Nao e possivel atacar o proprio territorio.\n");
        return;
    }
    if (mapa[i_atacante].tropas <= 1) {
        printf("Ataque cancelado: O atacante precisa de no minimo 2 tropas.\n");
        return;
    }
    if (strcmp(mapa[i_atacante].cor, mapa[i_defensor].cor) == 0) {
        printf("Ataque cancelado: Nao e possivel atacar um territorio do mesmo exercito.\n");
        return;
    }
    
    // Chama a simulacao de batalha, passando os ponteiros
    atacar(&mapa[i_atacante], &mapa[i_defensor]);
}

/**
 * @brief Simula um ataque entre dois territorios e atualiza os dados.
 * @param atacante Ponteiro para o territorio atacante.
 * @param defensor Ponteiro para o territorio defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_a = rolarDado();
    int dado_d = rolarDado();
    
    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Batalha: %s (%s) vs %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dados Sorteados: Atacante (%d) contra Defensor (%d)\n", dado_a, dado_d);
           
    if (dado_a > dado_d) {
        // Atacante vence
        printf("O Atacante %s VENCEU a rodada.\n", atacante->nome);
        
        // Atualizacao: defensor perde metade das tropas
        if (defensor->tropas > 0) {
            int tropas_perdidas = (defensor->tropas + 1) / 2; // Arredonda para cima
            defensor->tropas -= tropas_perdidas;
            printf("Defensor perde %d tropas. Tropas restantes: %d\n", tropas_perdidas, defensor->tropas);
        }

        if (defensor->tropas <= 0) {
            printf("\nTERRITORIO CONQUISTADO! %s agora pertence ao exercito %s.\n",
                   defensor->nome, atacante->cor);
                   
            // Altera o dono e move tropas
            strcpy(defensor->cor, atacante->cor);
            atacante->tropas -= 1;
            defensor->tropas = 1; // Tropa conquistadora
            
            printf("Uma tropa de %s move-se para %s.\n", atacante->nome, defensor->nome);
        }
        
    } else { // Defensor vence ou ha empate
        printf("O Defensor %s RESISTIU. Atacante perde 1 tropa.\n", defensor->nome);
        
        // Atacante perde 1 tropa
        atacante->tropas -= 1;
        
        printf("Atacante perde 1 tropa. Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar(); 
}

// ============================================================================
// --- Implementação da Função Utilitária ---
// ============================================================================

/**
 * @brief Limpa o buffer de entrada do teclado (stdin).
 */
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Consome os caracteres restantes
    }
}