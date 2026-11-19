#include "meu_banco.h"

void inicializarBanco(Banco *banco) {

    banco->clientes = NULL;
    banco->quantidadeClientes = 0;
    banco->proximoNumeroConta = 1000;
    banco->aberto = 0;

}
int salvarBanco(Banco *banco) {

    FILE *arquivo;

    arquivo = fopen("banco.dat", "wb");

    if (arquivo == NULL) {
        printf("Erro ao salvar os dados.\n");
        return 0;
    }

    fwrite(&banco->quantidadeClientes, sizeof(int), 1, arquivo);
    fwrite(&banco->proximoNumeroConta, sizeof(int), 1, arquivo);

    fwrite(banco->clientes, sizeof(Cliente),
           banco->quantidadeClientes, arquivo);

    fclose(arquivo);

    printf("Dados salvos com sucesso!\n");

    return 1;
}
void liberarBanco(Banco *banco) {

    free(banco->clientes);

    banco->clientes = NULL;
    banco->quantidadeClientes = 0;

}
void abrirBanco(Banco *banco) {

    if (banco->aberto == 1) {
        printf("O banco ja esta aberto.\n");
    }
    else {
        banco->aberto = 1;
        printf("Banco aberto com sucesso!\n");
    }

}
void fecharBanco(Banco *banco) {

    if (banco->aberto == 0) {
        printf("O banco ja esta fechado.\n");
    }
    else {
        banco->aberto = 0;
        printf("Banco fechado com sucesso!\n");
    }

}
void cadastrarCliente(Banco *banco, char operacao) {

    Cliente *novo;

    if (banco->aberto == 0) {
        printf("Abra o banco primeiro.\n");
        return;
    }

    novo = realloc(banco->clientes,
                   (banco->quantidadeClientes + 1) * sizeof(Cliente));

    if (novo == NULL) {
        printf("Erro ao cadastrar cliente.\n");
        return;
    }

    banco->clientes = novo;

    printf("Digite o nome do cliente: ");
    scanf(" %49[^\n]", banco->clientes[banco->quantidadeClientes].nome);

    banco->clientes[banco->quantidadeClientes].numeroConta =
        banco->proximoNumeroConta;

    banco->clientes[banco->quantidadeClientes].saldo = 0;

    banco->clientes[banco->quantidadeClientes].quantidadeTransacoes = 0;

    banco->clientes[banco->quantidadeClientes].emprestimoRealizado = 0;

    printf("Cliente cadastrado com sucesso!\n");
    printf("Numero da conta: %d\n", banco->proximoNumeroConta);

    banco->quantidadeClientes++;
    banco->proximoNumeroConta++;

    (void)operacao;
}
void movimentar(Banco *banco, char operacao) {

    int conta, i;
    float valor;

    if (banco->aberto == 0) {
        printf("O banco esta fechado.\n");
        return;
    }

    printf("Digite o numero da conta: ");
    scanf("%d", &conta);

    for (i = 0; i < banco->quantidadeClientes; i++) {

        if (banco->clientes[i].numeroConta == conta) {

            printf("Digite o valor: ");
            scanf("%f", &valor);

            if (valor <= 0) {
                printf("Valor invalido.\n");
                return;
            }

            if (operacao == 'D') {

                banco->clientes[i].saldo += valor;

                printf("Deposito realizado!\n");
            }
            else if (operacao == 'S') {

                banco->clientes[i].saldo -= valor;

                printf("Saque realizado!\n");
            }

            if (banco->clientes[i].quantidadeTransacoes < MAX_HISTORICO) {

                int posicao = banco->clientes[i].quantidadeTransacoes;

                if (operacao == 'D') {
                    strcpy(banco->clientes[i].historico[posicao].tipo,
                           "Deposito");
                }
                else {
                    strcpy(banco->clientes[i].historico[posicao].tipo,
                           "Saque");
                }

                banco->clientes[i].historico[posicao].valor = valor;

                banco->clientes[i].quantidadeTransacoes++;
            }
            else {

                int j;

                for (j = 0; j < MAX_HISTORICO - 1; j++) {
                    banco->clientes[i].historico[j] =
                        banco->clientes[i].historico[j + 1];
                }

                if (operacao == 'D') {
                    strcpy(banco->clientes[i].historico[4].tipo,
                           "Deposito");
                }
                else {
                    strcpy(banco->clientes[i].historico[4].tipo,
                           "Saque");
                }

                banco->clientes[i].historico[4].valor = valor;
            }

            return;
        }
    }

    printf("Conta nao encontrada.\n");
}
void solicitarEmprestimo(Banco *banco) {

    int conta, i;
    float valor;

    if (banco->aberto == 0) {
        printf("O banco esta fechado.\n");
        return;
    }

    printf("Digite o numero da conta: ");
    scanf("%d", &conta);

    for (i = 0; i < banco->quantidadeClientes; i++) {

        if (banco->clientes[i].numeroConta == conta) {

            if (banco->clientes[i].emprestimoRealizado == 1) {
                printf("Este cliente ja realizou um emprestimo.\n");
                return;
            }

            printf("Digite o valor do emprestimo: ");
            scanf("%f", &valor);

            if (valor <= 0) {
                printf("Valor invalido.\n");
                return;
            }

            banco->clientes[i].saldo += valor;
            banco->clientes[i].emprestimoRealizado = 1;

            printf("Emprestimo realizado com sucesso!\n");

            return;
        }
    }

    printf("Conta nao encontrada.\n");
}
void consultarHistorico(Banco *banco) {

    int conta, i, j;

    printf("Digite o numero da conta: ");
    scanf("%d", &conta);

    for (i = 0; i < banco->quantidadeClientes; i++) {

        if (banco->clientes[i].numeroConta == conta) {

            printf("\nHistorico de %s\n",
                   banco->clientes[i].nome);

            for (j = 0; j < banco->clientes[i].quantidadeTransacoes; j++) {

                printf("%s: R$ %.2f\n",
                       banco->clientes[i].historico[j].tipo,
                       banco->clientes[i].historico[j].valor);
            }

            return;
        }
    }

    printf("Conta nao encontrada.\n");
}
void mostrarRelatorios(Banco *banco) {

    int i;
    float saldoTotal = 0;

    printf("\n========== RELATORIOS ==========\n");

    printf("Total de clientes: %d\n", banco->quantidadeClientes);

    for (i = 0; i < banco->quantidadeClientes; i++) {

        saldoTotal += banco->clientes[i].saldo;
    }

    printf("Saldo total do banco: R$ %.2f\n", saldoTotal);

    printf("\nClientes com saldo negativo:\n");

    for (i = 0; i < banco->quantidadeClientes; i++) {

        if (banco->clientes[i].saldo < 0) {

            printf("%s - R$ %.2f\n",
                   banco->clientes[i].nome,
                   banco->clientes[i].saldo);
        }
    }

    printf("\nClientes que nunca fizeram emprestimo:\n");

    for (i = 0; i < banco->quantidadeClientes; i++) {

        if (banco->clientes[i].emprestimoRealizado == 0) {

            printf("%s - Conta %d\n",
                   banco->clientes[i].nome,
                   banco->clientes[i].numeroConta);
        }
    }
}
int fibonacci(int valor) {

    int t1 = 0;
    int t2 = 1;
    int proximoTermo;

    if (valor < 0) {
        return -1;
    }

    if (valor == 0) {
        return 0;
    }

    if (valor == 1) {
        return 1;
    }

    for (int i = 2; i <= valor; i++) {

        proximoTermo = t1 + t2;
        t1 = t2;
        t2 = proximoTermo;
    }

    return t2;
}

