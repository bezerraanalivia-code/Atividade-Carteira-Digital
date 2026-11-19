#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CORRENTE 1
#define SALARIO 2
#define POUPANCA 3
#define MAX_CONTAS 2
#define MAX_HISTORICO 5

typedef struct {
    int numero;
    int tipo;
    float saldo;
} Conta;

typedef struct {
    char tipo;              /* D = deposito, S = saque, E = emprestimo */
    float valor;
    int numeroConta;
    time_t dataHora;
} Transacao;

typedef struct {
    char cliente[50];
    Conta contas[MAX_CONTAS];
    int quantidadeContas;
    Transacao historico[MAX_HISTORICO];
    int quantidadeTransacoes;
    int emprestimoRealizado;
} Cliente;

typedef struct {
    Cliente *clientes;
    int quantidadeClientes;
    int proximoNumeroConta;
    int aberto;
} Banco;

void inicializarBanco(Banco *banco);
int salvarBanco(Banco *banco);
void liberarBanco(Banco *banco);
void abrirBanco(Banco *banco);
void fecharBanco(Banco *banco);
void cadastrarCliente(Banco *banco);
void movimentar(Banco *banco, char operacao);
void solicitarEmprestimo(Banco *banco);
void consultarHistorico(Banco *banco);
void mostrarRelatorios(Banco *banco);
void realizarSorteio(Banco *banco);

#endif
