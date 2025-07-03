#ifndef PROFESSOR_H
#define PROFESSOR_H
#include "pessoas.h"

class Professor : public Pessoa {
private:
    std::string titulacao;
public:
    static int qtdProfessores; // Contador estático público
    Professor();
    virtual ~Professor();

    void setTitulacao(std::string t);
    std::string getTitulacao() const;

    virtual void leiaPessoa() override;
    virtual void escrevePessoa() const override;
};

#endif