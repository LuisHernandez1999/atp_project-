#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ALUNOS 100
#define MAX_MATERIAS 5
#define ARQUIVO "alunos.txt"

typedef struct {
    char nome[50];
    int id_matricula;
    char materias[MAX_MATERIAS][50];
    int carga_horaria[MAX_MATERIAS];
    char professores[MAX_MATERIAS][50];
    float taxa_presenca[MAX_MATERIAS];
} Aluno;

Aluno alunos[MAX_ALUNOS];
int total_alunos = 0;

// Função para verificar se uma string é um número
int eh_numero(const char* str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// Função para carregar dados do arquivo
void carregar_dados() {
    FILE *file = fopen(ARQUIVO, "r");
    if (file == NULL) {
        printf("Nenhum arquivo de alunos encontrado, começando um novo cadastro.\n");
        return;
    }

    while (fscanf(file, " Nome: %[^\n]\n ID: %d\n", alunos[total_alunos].nome, &alunos[total_alunos].id_matricula) != EOF) {
        for (int i = 0; i < MAX_MATERIAS; i++) {
            fscanf(file, " Matéria %d: %[^,], Carga Horária: %d, Professor: %[^,], Presença: %f%%\n",
                   &i, alunos[total_alunos].materias[i], &alunos[total_alunos].carga_horaria[i],
                   alunos[total_alunos].professores[i], &alunos[total_alunos].taxa_presenca[i]);
        }
        fscanf(file, "--------------------\n");
        total_alunos++;
    }
    fclose(file);
}

// Função para salvar dados no arquivo
void salvar_dados() {
    FILE *file = fopen(ARQUIVO, "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar dados!\n");
        return;
    }

    for (int i = 0; i < total_alunos; i++) {
        fprintf(file, "Nome: %s\n", alunos[i].nome);
        fprintf(file, "ID: %d\n", alunos[i].id_matricula);
        for (int j = 0; j < MAX_MATERIAS; j++) {
            fprintf(file, "Matéria %d: %s, Carga Horária: %d, Professor: %s, Presença: %.2f%%\n",
                    j + 1, alunos[i].materias[j], alunos[i].carga_horaria[j],
                    alunos[i].professores[j], alunos[i].taxa_presenca[j]);
        }
        fprintf(file, "--------------------\n");
    }
    fclose(file);
}

// Função para cadastrar aluno
void cadastrar_aluno() {
    if (total_alunos >= MAX_ALUNOS) {
        printf("Limite de alunos atingido!\n");
        return;
    }

    Aluno novo_aluno;

    printf("Digite o nome do aluno: ");
    scanf(" %[^\n]", novo_aluno.nome);

    printf("Digite o ID de matrícula (somente números): ");
    char id_input[10];
    scanf("%s", id_input);
    if (!eh_numero(id_input)) {
        printf("ID de matrícula inválido! Deve ser numérico.\n");
        return;
    }
    novo_aluno.id_matricula = atoi(id_input);

    for (int i = 0; i < MAX_MATERIAS; i++) {
        printf("Digite o nome da matéria %d: ", i + 1);
        scanf(" %[^\n]", novo_aluno.materias[i]);

        printf("Digite a carga horária da matéria %d (número positivo): ", i + 1);
        int carga;
        scanf("%d", &carga);
        if (carga <= 0) {
            printf("Carga horária inválida! Deve ser um número positivo.\n");
            return;
        }
        novo_aluno.carga_horaria[i] = carga;

        printf("Digite o nome do professor da matéria %d: ", i + 1);
        scanf(" %[^\n]", novo_aluno.professores[i]);

        printf("Digite a taxa de presença da matéria %d (0 a 100): ", i + 1);
        float presenca;
        scanf("%f", &presenca);
        if (presenca < 0 || presenca > 100) {
            printf("Taxa de presença inválida! Deve estar entre 0 e 100.\n");
            return;
        }
        novo_aluno.taxa_presenca[i] = presenca;
    }

    alunos[total_alunos++] = novo_aluno;
    salvar_dados(); // Salva imediatamente após o cadastro do aluno

    printf("Aluno cadastrado com sucesso!\n");
}

// Função para buscar aluno por ID
int buscar_aluno_por_id(int id_matricula) {
    for (int i = 0; i < total_alunos; i++) {
        if (alunos[i].id_matricula == id_matricula) {
            return i; // Retorna o índice do aluno encontrado
        }
    }
    return -1; // Retorna -1 se não encontrar
}

// Função para listar detalhes de um aluno
void listar_detalhes_aluno(int indice) {
    Aluno *aluno = &alunos[indice];
    printf("Detalhes do Aluno:\n");
    printf("Nome: %s\n", aluno->nome);
    printf("ID de Matrícula: %d\n", aluno->id_matricula);
    for (int i = 0; i < MAX_MATERIAS; i++) {
        printf("Matéria: %s, Carga Horária: %d, Professor: %s, Presença: %.2f%%\n",
               aluno->materias[i], aluno->carga_horaria[i],
               aluno->professores[i], aluno->taxa_presenca[i]);
    }
}

// Função para editar aluno
void editar_aluno() {
    int id_matricula;
    printf("Digite o ID de matrícula do aluno que deseja editar: ");
    scanf("%d", &id_matricula);

    int indice = buscar_aluno_por_id(id_matricula);
    if (indice == -1) {
        printf("Aluno com ID %d não encontrado.\n", id_matricula);
        return;
    }

    Aluno *aluno = &alunos[indice];
    printf("Editando informações do aluno: %s\n", aluno->nome);

    printf("Digite o novo nome do aluno (ou pressione Enter para manter o mesmo): ");
    char novo_nome[50];
    getchar(); // Limpa o buffer
    fgets(novo_nome, 50, stdin);
    if (strlen(novo_nome) > 1) {
        novo_nome[strcspn(novo_nome, "\n")] = 0; // Remove nova linha
        strcpy(aluno->nome, novo_nome);
    }

    for (int j = 0; j < MAX_MATERIAS; j++) {
        printf("Editando matéria %d:\n", j + 1);
        
        printf("Nome da matéria (ou pressione Enter para manter o mesmo): ");
        char nova_materia[50];
        fgets(nova_materia, 50, stdin);
        if (strlen(nova_materia) > 1) {
            nova_materia[strcspn(nova_materia, "\n")] = 0;
            strcpy(aluno->materias[j], nova_materia);
        }

        printf("Carga horária (ou 0 para manter o mesmo): ");
        int nova_carga_horaria;
        scanf("%d", &nova_carga_horaria);
        if (nova_carga_horaria > 0) {
            aluno->carga_horaria[j] = nova_carga_horaria;
        }

        printf("Professor (ou pressione Enter para manter o mesmo): ");
        char novo_professor[50];
        getchar(); // Limpa o buffer
        fgets(novo_professor, 50, stdin);
        if (strlen(novo_professor) > 1) {
            novo_professor[strcspn(novo_professor, "\n")] = 0;
            strcpy(aluno->professores[j], novo_professor);
        }

        printf("Taxa de presença (ou -1 para manter o mesmo): ");
        float nova_presenca;
        scanf("%f", &nova_presenca);
        if (nova_presenca >= 0 && nova_presenca <= 100) {
            aluno->taxa_presenca[j] = nova_presenca;
        }
    }

    salvar_dados(); // Salva as alterações imediatamente
    printf("Informações do aluno atualizadas com sucesso!\n");
}

// Função para excluir aluno
void excluir_aluno() {
    int id_matricula;
    printf("Digite o ID de matrícula do aluno que deseja excluir: ");
    scanf("%d", &id_matricula);

    int indice = buscar_aluno_por_id(id_matricula);
    if (indice == -1) {
        printf("Aluno com ID %d não encontrado.\n", id_matricula);
        return;
    }

    // Mover alunos para cima para preencher o espaço
    for (int i = indice; i < total_alunos - 1; i++) {
        alunos[i] = alunos[i + 1];
    }
    total_alunos--;
    salvar_dados(); // Salva após a exclusão
    printf("Aluno excluído com sucesso!\n");
}

// Função para listar alunos
void listar_alunos() {
    printf("Lista de Alunos:\n");
    for (int i = 0; i < total_alunos; i++) {
        printf("Nome: %s | ID: %d\n", alunos[i].nome, alunos[i].id_matricula);
    }
}

// Função principal
int main() {
    int opcao;
    carregar_dados();

    do {
        printf("\nMenu:\n");
        printf("1. Cadastrar Aluno\n");
        printf("2. Editar Aluno\n");
        printf("3. Excluir Aluno\n");
        printf("4. Listar Alunos\n");
        printf("5. Ver Detalhes do Aluno\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                cadastrar_aluno();
                break;
            case 2:
                editar_aluno();
                break;
            case 3:
                excluir_aluno();
                break;
            case 4:
                listar_alunos();
                break;
            case 5: {
                int id_matricula;
                printf("Digite o ID de matrícula do aluno que deseja visualizar: ");
                scanf("%d", &id_matricula);
                int indice = buscar_aluno_por_id(id_matricula);
                if (indice != -1) {
                    listar_detalhes_aluno(indice);
                } else {
                    printf("Aluno com ID %d não encontrado.\n", id_matricula);
                }
                break;
            }
            case 6:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 6);

    return 0;
}
