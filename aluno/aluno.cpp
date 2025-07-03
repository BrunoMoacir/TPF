#include "aluno.h"
#include <iostream>

int Aluno::qtdAlunos = 0; // Inicialização do contador

Aluno::Aluno() : Pessoa() { qtdAlunos++; }
Aluno::~Aluno() { qtdAlunos--; }

void Aluno::setMatricula(std::string m) { matricula = m; }
std::string Aluno::getMatricula() const { return matricula; }

void Aluno::leiaPessoa() {
    Pessoa::leiaPessoa();
    std::cout << "Matricula: ";
    std::getline(std::cin, matricula);
}

void Aluno::escrevePessoa() const {
    Pessoa::escrevePessoa();
    std::cout << "\nTipo: Aluno\nMatricula: " << matricula << std::endl;
}