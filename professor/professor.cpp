#include "professor.h"
#include <iostream>

int Professor::qtdProfessores = 0; // Inicialização do contador

Professor::Professor() : Pessoa() { qtdProfessores++; }
Professor::~Professor() { qtdProfessores--; }

void Professor::setTitulacao(std::string t) { titulacao = t; }
std::string Professor::getTitulacao() const { return titulacao; }

void Professor::leiaPessoa() {
    Pessoa::leiaPessoa();
    std::cout << "Titulacao (Especialista, Mestre, Doutor): ";
    std::getline(std::cin, titulacao);
}

void Professor::escrevePessoa() const {
    Pessoa::escrevePessoa();
    std::cout << "\nTipo: Professor\nTitulacao: " << titulacao << std::endl;
}