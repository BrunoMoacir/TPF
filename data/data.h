#ifndef DATA_H
#define DATA_H

#include <string>

class Data {
private:
    int dia, mes, ano;

public:
    Data(); 
    Data(int d, int m, int a);

    bool setData(int d, int m, int a);
    int getDia() const;
    int getMes() const;
    int getAno() const;

    void leiaData();
    void escreveData() const;
    bool mesmoMes(int m) const;
};

Data obterDataAtual();
int calcularIdade(const Data& nascimento);

#endif