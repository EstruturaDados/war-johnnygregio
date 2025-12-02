#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#include <ctype.h> // Necessária para a função toupper()

// --- Constantes ---
#define MAX_STRING 50
#define MAX_COR 10
#define MAX_MISSAO_LEN 100
#define TOTAL_MISSOES 5

// ============================================================================
// --- Estrutura de Dados ---
// ============================================================================

/**
 * @brief Estrutura que representa um território no mapa.
 */
typedef struct {
    char nome[MAX_STRING];
    char cor[MAX_COR]; // Cor do exército dominante (sempre armazenada em MAIÚSCULAS)
    int tropas;
} Territorio;

// ============================================================================
// --- Protótipos de Funções ---
// ============================================================================

// Gerenciamento de Memória
Territorio* alocarMapa(int tamanho);
char* alocarMissao(void);
void liberarMemoria(Territorio* mapa, char* missao);

// Setup e Exibição
void cadastrarTerritorios(Territorio* mapa, int tamanho);
void exibirMapa(const Territorio* mapa, int tamanho);
void exibirMissao(const char* missao);

// Lógica do Jogo (Missões e Batalha)
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* cor_jogador);
void faseDeAtaque(Territorio* mapa, int tamanho, const char* cor_jogador);
void atacar(Territorio* atacante, Territorio* defensor);

// Utilitárias
void limparBufferEntrada(void);
int rolarDado(void);
void toUpperString(char* str); // Nova função para conversão

// ============================================================================
// --- Função Principal (main) ---
// ============================================================================

int main() {
    srand((unsigned int)time(NULL)); 
    
    // Vetor de Strings para Missões
    const char* missoes[] = {
        "Conquistar 4 territorios seguidos.",
        "Eliminar todas as tropas da cor VERDE.",
        "Conquistar um total de 3 territorios no mapa.",
        "Garantir que pelo menos 3 de seus territorios tenham mais de 5 tropas.",
        "Dominar o mapa inteiro (todos os territorios)."
    };
    // Cor do jogador definida em MAIÚSCULAS para comparação consistente
    const char* cor_jogador = "AZUL"; 

    int num_territorios = 0;
    Territorio* mapa = NULL; 
    char* missao_jogador = NULL; 

    printf("=======================================================\n");
    printf("         WAR ESTRUTURADO - DESAFIO FINAL\n");
    printf("=======================================================\n");

    printf("Informe o numero total de territorios (Min. 5): ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios < 5) {
        printf("Numero de territorios ajustado para 5.\n");
        num_territorios = 5;
    }
    limparBufferEntrada();
    
    mapa = alocarMapa(num_territorios);
    missao_jogador = alocarMissao(); 

    if (mapa == NULL || missao_jogador == NULL) {
        printf("ERRO: Falha ao alocar memoria. Encerrando o programa.\n");
        liberarMemoria(mapa, missao_jogador);
        return 1;
    }
    
    atribuirMissao(missao_jogador, missoes, TOTAL_MISSOES);
    exibirMissao(missao_jogador);
    
    printf("\n[ATENCAO] Seu exercito e a cor: %s\n", cor_jogador);
    
    // O cadastro agora garante que as cores sejam armazenadas em MAIÚSCULAS
    cadastrarTerritorios(mapa, num_territorios);

    int escolha = -1;
    int vitoria = 0;
    do {
        exibirMapa(mapa, num_territorios);
        printf("\n--- Menu de Acoes ---\n");
        printf("1. Iniciar Ataque\n");
        printf("2. Verificar Missao (Condicao de Vitoria)\n");
        printf("0. Sair do Jogo\n");
        printf("Sua escolha: ");
        
        if (scanf("%d", &escolha) != 1) {
            limparBufferEntrada();
            escolha = -1; 
        } else {
            limparBufferEntrada();
        }

        if (escolha == 1) {
            faseDeAtaque(mapa, num_territorios, cor_jogador);
            vitoria = verificarMissao(missao_jogador, mapa, num_territorios, cor_jogador);
        } else if (escolha == 2) {
            vitoria = verificarMissao(missao_jogador, mapa, num_territorios, cor_jogador);
            if (vitoria) {
                printf("\n=======================================================\n");
                printf("!!! MISSAO CUMPRIDA: VOCE VENCEU O JOGO !!!\n");
                printf("=======================================================\n");
            } else {
                printf("\nA missao '%s' ainda nao foi cumprida. Continue lutando.\n", missao_jogador);
            }
        } else if (escolha != 0) {
            printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (escolha != 0 && !vitoria);

    liberarMemoria(mapa, missao_jogador);
    printf("\nMemoria e recursos liberados. Programa finalizado.\n");

    return 0;
}

// ============================================================================
// --- Implementação das Funções de Gerenciamento de Memória ---
// ============================================================================

Territorio* alocarMapa(int tamanho) {
    Territorio* mapa = (Territorio*)calloc(tamanho, sizeof(Territorio));
    return mapa;
}

char* alocarMissao(void) {
    char* missao = (char*)malloc(MAX_MISSAO_LEN * sizeof(char)); 
    return missao;
}

void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (missao != NULL) {
        free(missao);
    }
}

// ============================================================================
// --- Implementação das Funções de Missão ---
// ============================================================================

void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    int indice_sorteado = rand() % totalMissoes; 
    
    strncpy(destino, missoes[indice_sorteado], MAX_MISSAO_LEN - 1);
    destino[MAX_MISSAO_LEN - 1] = '\0';
}

void exibirMissao(const char* missao) {
    printf("\n[MISSAO SECRETA]: %s\n", missao);
}

/**
 * @brief Verifica se a missão do jogador foi cumprida. 
 * @note As cores nos territorios estão garantidamente em MAIÚSCULAS.
 */
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* cor_jogador) {
    int count_tropas_altas = 0;
    int count_territorios_jogador = 0;
    int inimigo_verde_existe = 0;

    // Conta os territórios do jogador
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, cor_jogador) == 0) {
            count_territorios_jogador++;
        }
    }

    if (strstr(missao, "Eliminar todas as tropas da cor VERDE")) {
        for (int i = 0; i < tamanho; i++) {
            // A comparação agora é segura, pois mapa[i].cor foi convertido para MAIÚSCULAS no cadastro
            if (strcmp(mapa[i].cor, "VERDE") == 0) {
                inimigo_verde_existe = 1; 
                break;
            }
        }
        return !inimigo_verde_existe;
    }
    
    if (strstr(missao, "Dominar o mapa inteiro")) {
        return (count_territorios_jogador == tamanho);
    }
    
    if (strstr(missao, "Conquistar um total de 3 territorios")) {
        return (count_territorios_jogador >= 3);
    }

    if (strstr(missao, "territorios tenham mais de 5 tropas")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, cor_jogador) == 0 && mapa[i].tropas > 5) {
                count_tropas_altas++;
            }
        }
        return (count_tropas_altas >= 3);
    }
    
    if (strstr(missao, "Conquistar 4 territorios seguidos")) {
        return (count_territorios_jogador >= 4);
    }

    return 0;
}


// ============================================================================
// --- Implementação das Funções de Setup e Exibição ---
// ============================================================================

/**
 * @brief Converte todos os caracteres de uma string para maiúsculas.
 * @param str Ponteiro para a string a ser modificada.
 */
void toUpperString(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)toupper((unsigned char)str[i]);
    }
}

/**
 * @brief Solicita e armazena os dados de cada território.
 */
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\n--- Cadastro do Territorio %d de %d ---\n", i + 1, tamanho);

        printf("Nome do Territorio: ");
        if (fgets(mapa[i].nome, MAX_STRING, stdin) == NULL) return;
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do Exercito Dominante (Ex: Vermelho, Azul, Verde): ");
        if (fgets(mapa[i].cor, MAX_COR, stdin) == NULL) return;
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        
        // 🚨 NOVO: Garante que a cor seja armazenada em MAIÚSCULAS
        toUpperString(mapa[i].cor);
        
        printf("Numero de Tropas: ");
        if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas <= 0) {
             printf("Tropas invalidas. Definindo para 1.\n");
             mapa[i].tropas = 1;
        }
        limparBufferEntrada(); 
    }
}

void exibirMapa(const Territorio* mapa, int tamanho) {
    printf("\n\n=======================================================\n");
    printf("               ESTADO ATUAL DO MAPA\n");
    printf("=======================================================\n");

    printf("| %-3s | %-20s | %-10s | %-10s |\n", "ID", "Territorio", "Exercito", "Tropas");
    printf("|-----|----------------------|------------|------------|\n");

    for (int i = 0; i < tamanho; i++) {
        printf("| %-3d | %-20s | %-10s | %-10d |\n", 
               i + 1,
               mapa[i].nome, 
               mapa[i].cor, 
               mapa[i].tropas);
    }
    printf("|-----|----------------------|------------|------------|\n");
}

// ============================================================================
// --- Implementação das Funções de Lógica do Jogo (Ataque) ---
// ============================================================================

int rolarDado(void) {
    return (rand() % 6) + 1;
}

/**
 * @brief Gerencia a interface de ataque e valida as escolhas do jogador.
 * @note A cor do territorio atacante foi convertida para MAIÚSCULAS no cadastro.
 */
void faseDeAtaque(Territorio* mapa, int tamanho, const char* cor_jogador) {
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

    int i_atacante = id_atacante - 1;
    int i_defensor = id_defensor - 1;

    // --- Validacoes ---
    if (i_atacante == i_defensor) {
        printf("Ataque cancelado: Nao e possivel atacar o proprio territorio.\n");
        return;
    }
    // A comparação é segura, pois mapa[i_atacante].cor é MAIÚSCULA
    if (strcmp(mapa[i_atacante].cor, cor_jogador) != 0) {
        printf("Ataque cancelado: Voce so pode atacar de seus proprios territorios (%s).\n", cor_jogador);
        return;
    }
    if (mapa[i_atacante].tropas <= 1) {
        printf("Ataque cancelado: O atacante precisa de no minimo 2 tropas.\n");
        return;
    }
    // A comparação é segura
    if (strcmp(mapa[i_atacante].cor, mapa[i_defensor].cor) == 0) {
        printf("Ataque cancelado: Nao e possivel atacar um territorio do mesmo exercito.\n");
        return;
    }
    
    atacar(&mapa[i_atacante], &mapa[i_defensor]);
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dado_a = rolarDado();
    int dado_d = rolarDado();
    
    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Batalha: %s (%s) vs %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dados Sorteados: Atacante (%d) contra Defensor (%d)\n", dado_a, dado_d);
           
    if (dado_a > dado_d) {
        printf("O Atacante %s VENCEU a rodada.\n", atacante->nome);
        
        if (defensor->tropas > 0) {
            int tropas_perdidas = (defensor->tropas + 1) / 2;
            defensor->tropas -= tropas_perdidas;
            printf("Defensor perde %d tropas. Tropas restantes: %d\n", tropas_perdidas, defensor->tropas);
        }

        if (defensor->tropas <= 0) {
            printf("\nTERRITORIO CONQUISTADO! %s agora pertence ao exercito %s.\n",
                   defensor->nome, atacante->cor);
                   
            // Altera o dono. Como atacante->cor está em MAIÚSCULAS, o novo dono também estará.
            strcpy(defensor->cor, atacante->cor);
            atacante->tropas -= 1;
            defensor->tropas = 1; 
            
            printf("Uma tropa de %s move-se para %s.\n", atacante->nome, defensor->nome);
        }
        
    } else {
        printf("O Defensor %s RESISTIU. Atacante perde 1 tropa.\n", defensor->nome);
        atacante->tropas -= 1;
        printf("Atacante perde 1 tropa. Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
    }
    
    printf("\nPressione ENTER para continuar...");
    getchar(); 
}

// ============================================================================
// --- Implementação da Função Utilitária ---
// ============================================================================

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}