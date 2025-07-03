#include "data.h"
#include <iostream>
#include <ctime>

Data::Data() : dia(1), mes(1), ano(1900) {}
Data::Data(int d, int m, int a) { setData(d, m, a); }

bool Data::setData(int d, int m, int a) {
    if (m < 1 || m > 12 || d < 1 || d > 31) return false;
    // Validação simples. Uma validação real seria mais complexa.
    dia = d; mes = m; ano = a;
    return true;
}

int Data::getDia() const { return dia; }
int Data::getMes() const { return mes; }
int Data::getAno() const { return ano; }

void Data::leiaData() {
    int d, m, a;
    do {
        std::cout << "Dia: "; std::cin >> d;
        std::cout << "Mes: "; std::cin >> m;
        std::cout << "Ano: "; std::cin >> a;
        if (!setData(d, m, a)) {
            std::cout << "Data invalida! Tente novamente.\n";
        }
    } while (!setData(d,m,a));
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Data::escreveData() const { std::cout << dia << "/" << mes << "/" << ano; }
bool Data::mesmoMes(int m) const { return mes == m; }

Data obterDataAtual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return Data(now->tm_mday, 1 + now->tm_mon, 1900 + now->tm_year);
}

int calcularIdade(const Data& nascimento) {
    Data atual = obterDataAtual();
    int idade = atual.getAno() - nascimento.getAno();
    if (atual.getMes() < nascimento.getMes() ||
       (atual.getMes() == nascimento.getMes() && atual.getDia() < nascimento.getDia())) {
        idade--;
    }
    return idade;
}
