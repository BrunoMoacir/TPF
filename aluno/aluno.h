#ifndef ALUNO_H
#define ALUNO_H
#include "pessoas.h"

class Aluno : public Pessoa {
private:
    std::string matricula;
public:
    static int qtdAlunos; // Contador estático público
    Aluno();
    virtual ~Aluno();

    void setMatricula(std::string m);
    std::string getMatricula() const;

    virtual void leiaPessoa() override;
    virtual void escrevePessoa() const override;
};
#endif