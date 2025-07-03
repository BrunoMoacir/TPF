#ifndef PESSOAS_H
#define PESSOAS_H

#include "data.h"
#include <iostream>
#include <string>

#define MAX 100 

class Pessoa {
protected:
    std::string nome;
    std::string cpf;
    Data nascimento;

public:
    Pessoa(); // Construtor padrão
    Pessoa(std::string n, std::string c, const Data& d); // Construtor com parâmetros
    virtual ~Pessoa(); 

    void setNome(std::string n);
    std::string getNome() const;
    bool setCPF(std::string c);
    std::string getCPF() const;
    Data getNascimento() const;

    virtual void leiaPessoa();
    virtual void escrevePessoa() const;
};

// Funções de gerenciamento do sistema
extern int TAM;
extern char tipos[MAX];
void abertura(Pessoa *pessoas[]);
void despedida(Pessoa *pessoas[]);

// Funções específicas chamadas pelos submenus
void cadastrarProfessor(Pessoa *pessoas[]);
void cadastrarAluno(Pessoa *pessoas[]);
void listar(Pessoa *pessoas[], char tipoFiltro);
void pesquisarPorNome(Pessoa *pessoas[], char tipoFiltro);
void pesquisarPorCPF(Pessoa *pessoas[], char tipoFiltro);
void excluirPorCPF(Pessoa *pessoas[], char tipoFiltro);
void apagarTodos(Pessoa *pessoas[], char tipoFiltro);
void listarAniversariantes(Pessoa *pessoas[]);


#endif