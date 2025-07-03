#include "pessoas.h"
#include "aluno.h"
#include "professor.h"
#include <fstream>
#include <string>
#include <new> // Para std::bad_alloc

int TAM = 0;
char tipos[MAX]; 

// --- Implementação da classe Pessoa ---
Pessoa::Pessoa() : nome(""), cpf(""), nascimento() {}

Pessoa::Pessoa(std::string n, std::string c, const Data& d) : nome(n), cpf(c), nascimento(d) {}

Pessoa::~Pessoa() {}

void Pessoa::setNome(std::string n) { nome = n; }
std::string Pessoa::getNome() const { return nome; }
bool Pessoa::setCPF(std::string c) {
    if (c.length() == 14 && c[3] == '.' && c[7] == '.' && c[11] == '-') {
        cpf = c;
        return true;
    }
    return false;
}
std::string Pessoa::getCPF() const { return cpf; }
Data Pessoa::getNascimento() const { return nascimento; }

void Pessoa::leiaPessoa() {
    std::cout << "Nome: ";
    std::getline(std::cin, nome);
    
    std::string tempCpf;
    do {
        std::cout << "CPF (formato 000.000.000-00): ";
        std::getline(std::cin, tempCpf);
    } while (!setCPF(tempCpf));

    std::cout << "Data de nascimento:\n";
    nascimento.leiaData();
}

void Pessoa::escrevePessoa() const {
    std::cout << "\nNome: " << nome << "\nCPF: " << cpf << "\nNascimento: ";
    nascimento.escreveData();
    std::cout << "\nIdade: " << calcularIdade(nascimento);
}

// --- FUNÇÕES DE PERSISTÊNCIA (ATUALIZADAS) ---
void writeString(std::ofstream& arq, const std::string& str) {
    size_t len = str.size();
    arq.write(reinterpret_cast<const char*>(&len), sizeof(len));
    arq.write(str.c_str(), len);
}

std::string readString(std::ifstream& arq) {
    size_t len;
    arq.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (arq.fail() || len > 1024) return ""; // Proteção
    char* buffer = new char[len + 1];
    arq.read(buffer, len);
    buffer[len] = '\0';
    std::string str(buffer);
    delete[] buffer;
    return str;
}

void gravaPessoas(Pessoa *pessoas[]) {
    std::ofstream arq("pessoas.dat", std::ios::binary);
    if (!arq) return;
    arq.write(reinterpret_cast<const char*>(&TAM), sizeof(TAM));
    for (int i = 0; i < TAM; i++) {
        arq.write(&tipos[i], sizeof(char));
        writeString(arq, pessoas[i]->getNome());
        writeString(arq, pessoas[i]->getCPF());
        Data nasc = pessoas[i]->getNascimento();
        arq.write(reinterpret_cast<const char*>(&nasc), sizeof(Data));
        if (tipos[i] == 'A') {
            writeString(arq, dynamic_cast<Aluno*>(pessoas[i])->getMatricula());
        } else {
            writeString(arq, dynamic_cast<Professor*>(pessoas[i])->getTitulacao());
        }
    }
    arq.close();
}

void carregaPessoas(Pessoa *pessoas[]) {
    std::ifstream arq("pessoas.dat", std::ios::binary);
    if (!arq) { TAM = 0; return; }
    arq.read(reinterpret_cast<char*>(&TAM), sizeof(TAM));
    if (arq.fail() || TAM < 0 || TAM > MAX) { TAM = 0; arq.close(); return; }

    for (int i = 0; i < TAM; i++) {
        arq.read(&tipos[i], sizeof(char));
        if(arq.eof()) { TAM = i; break; }

        if (tipos[i] == 'A') pessoas[i] = new Aluno();
        else if (tipos[i] == 'P') pessoas[i] = new Professor();
        else continue; 

        pessoas[i]->setNome(readString(arq));
        pessoas[i]->setCPF(readString(arq));
        Data nasc;
        arq.read(reinterpret_cast<char*>(&nasc), sizeof(Data));
        pessoas[i]->Pessoa::Pessoa(pessoas[i]->getNome(), pessoas[i]->getCPF(), nasc);

        if (tipos[i] == 'A') {
            dynamic_cast<Aluno*>(pessoas[i])->setMatricula(readString(arq));
        } else {
            dynamic_cast<Professor*>(pessoas[i])->setTitulacao(readString(arq));
        }
    }
    arq.close();
}

void abertura(Pessoa *pessoas[]) {
    carregaPessoas(pessoas);
    std::cout << "Sistema iniciado. " << TAM << " registros carregados.\n";
}

void despedida(Pessoa *pessoas[]) {
    gravaPessoas(pessoas);
    for (int i = 0; i < TAM; i++) delete pessoas[i];
}

// --- FUNÇÕES ESPECÍFICAS PARA OS SUBMENUS ---

void cadastrar(Pessoa* pessoas[], char tipo) {
    if (TAM >= MAX) {
        std::cout << "Erro: Limite maximo de cadastros atingido!\n";
        return;
    }
    try {
        if (tipo == 'A') {
            pessoas[TAM] = new Aluno();
            std::cout << "\n--- Cadastro de Aluno ---\n";
        } else {
            pessoas[TAM] = new Professor();
            std::cout << "\n--- Cadastro de Professor ---\n";
        }
        pessoas[TAM]->leiaPessoa();
        tipos[TAM] = tipo;
        TAM++;
        std::cout << "Cadastro realizado com sucesso!\n";
    } catch (const std::bad_alloc& e) {
        std::cerr << "ERRO GRAVE: Falha ao alocar memoria! Nao foi possivel cadastrar. " << e.what() << '\n';
    }
}
void cadastrarProfessor(Pessoa *pessoas[]) { cadastrar(pessoas, 'P'); }
void cadastrarAluno(Pessoa *pessoas[]) { cadastrar(pessoas, 'A'); }

void listar(Pessoa *pessoas[], char tipoFiltro) {
    if (TAM == 0) { std::cout << "Nenhum registro cadastrado.\n"; return; }
    
    int count = 0;
    for(int i = 0; i < TAM; i++) {
        if(tipoFiltro == 'T' || tipos[i] == tipoFiltro) {
            pessoas[i]->escrevePessoa();
            std::cout << "\n-------------------------------------\n";
            count++;
        }
    }
    if(count == 0) {
        std::cout << "Nenhum registro do tipo especificado encontrado.\n";
    }
}

void pesquisarPorNome(Pessoa *pessoas[], char tipoFiltro) {
    std::string nomeBusca;
    std::cout << "Digite o nome para pesquisar: ";
    std::getline(std::cin, nomeBusca);
    
    int count = 0;
    for (int i = 0; i < TAM; i++) {
        if ((tipoFiltro == 'T' || tipos[i] == tipoFiltro) && 
            (pessoas[i]->getNome().find(nomeBusca) != std::string::npos)) {
            pessoas[i]->escrevePessoa();
            std::cout << "\n---------------------------\n";
            count++;
        }
    }
    if (count == 0) std::cout << "Nenhum registro encontrado.\n";
}

void pesquisarPorCPF(Pessoa *pessoas[], char tipoFiltro) {
    std::string cpfBusca;
    std::cout << "Digite o CPF para pesquisar (000.000.000-00): ";
    std::getline(std::cin, cpfBusca);

    int count = 0;
    for (int i = 0; i < TAM; i++) {
        if ((tipoFiltro == 'T' || tipos[i] == tipoFiltro) && (pessoas[i]->getCPF() == cpfBusca)) {
            pessoas[i]->escrevePessoa();
            std::cout << "\n---------------------------\n";
            count++;
        }
    }
    if (count == 0) std::cout << "Nenhum registro encontrado.\n";
}

void excluirPorCPF(Pessoa *pessoas[], char tipoFiltro) {
    std::string cpfBusca;
    std::cout << "Digite o CPF do registro a ser excluido: ";
    std::getline(std::cin, cpfBusca);

    int idx = -1;
    for (int i = 0; i < TAM; i++) {
        if ((tipoFiltro == 'T' || tipos[i] == tipoFiltro) && (pessoas[i]->getCPF() == cpfBusca)) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        delete pessoas[idx];
        for (int i = idx; i < TAM - 1; i++) {
            pessoas[i] = pessoas[i + 1];
            tipos[i] = tipos[i + 1];
        }
        TAM--;
        std::cout << "Registro excluido com sucesso!\n";
    } else {
        std::cout << "Nenhum registro encontrado com este CPF para o tipo selecionado.\n";
    }
}

void apagarTodos(Pessoa *pessoas[], char tipoFiltro) {
    char confirm;
    std::cout << "TEM CERTEZA? (S/N): ";
    std::cin >> confirm;
    std::cin.ignore();
    
    if (toupper(confirm) != 'S') { std::cout << "Operacao cancelada.\n"; return; }

    int i = 0;
    while(i < TAM) {
        if(tipoFiltro == 'T' || tipos[i] == tipoFiltro) {
            delete pessoas[i];
            for(int j = i; j < TAM - 1; j++) {
                pessoas[j] = pessoas[j+1];
                tipos[j] = tipos[j+1];
            }
            TAM--;
        } else {
            i++;
        }
    }
    std::cout << "Registros excluidos.\n";
}

void listarAniversariantes(Pessoa *pessoas[]) {
    int mes;
    std::cout << "Digite o mes para pesquisar (1-12): ";
    std::cin >> mes;
    std::cin.ignore();
    
    if(mes < 1 || mes > 12) { std::cout << "Mes invalido.\n"; return; }

    int count = 0;
    std::cout << "\n--- Aniversariantes do Mes " << mes << " ---\n";
    for(int i = 0; i < TAM; i++) {
        if(pessoas[i]->getNascimento().getMes() == mes) {
            std::cout << (tipos[i] == 'A' ? "Aluno: " : "Professor: ") 
                      << pessoas[i]->getNome() << " (Dia " << pessoas[i]->getNascimento().getDia() << ")\n";
            count++;
        }
    }
    if(count == 0) std::cout << "Nenhum aniversariante encontrado.\n";
}

// --- Funções de Submenu (Handlers) ---
void handleCadastro(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n1.0 - Voltar\n1.1 - Cadastrar Professor\n1.2 - Cadastrar Aluno\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "1.1") cadastrarProfessor(pessoas);
    else if(op == "1.2") cadastrarAluno(pessoas);
}

void handleListagem(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n2.0 - Voltar\n2.1 - Listar Professores\n2.2 - Listar Alunos\n2.3 - Listar Todos\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "2.1") listar(pessoas, 'P');
    else if(op == "2.2") listar(pessoas, 'A');
    else if(op == "2.3") listar(pessoas, 'T');
}

void handlePesquisaNome(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n3.0 - Voltar\n3.1 - Pesquisar Professor\n3.2 - Pesquisar Aluno\n3.3 - Pesquisar em Todos\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "3.1") pesquisarPorNome(pessoas, 'P');
    else if(op == "3.2") pesquisarPorNome(pessoas, 'A');
    else if(op == "3.3") pesquisarPorNome(pessoas, 'T');
}

void handlePesquisaCPF(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n4.0 - Voltar\n4.1 - Pesquisar Professor\n4.2 - Pesquisar Aluno\n4.3 - Pesquisar em Todos\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "4.1") pesquisarPorCPF(pessoas, 'P');
    else if(op == "4.2") pesquisarPorCPF(pessoas, 'A');
    else if(op == "4.3") pesquisarPorCPF(pessoas, 'T');
}

void handleExcluir(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n5.0 - Voltar\n5.1 - Excluir Professor (pelo CPF)\n5.2 - Excluir Aluno (pelo CPF)\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "5.1") excluirPorCPF(pessoas, 'P');
    else if(op == "5.2") excluirPorCPF(pessoas, 'A');
}

void handleApagarTodos(Pessoa *pessoas[]) {
    std::string op;
    std::cout << "\n6.0 - Voltar\n6.1 - Apagar TODOS os Professores\n6.2 - Apagar TODOS os Alunos\n6.3 - Apagar TUDO\nEscolha: ";
    std::getline(std::cin, op);
    if(op == "6.1") apagarTodos(pessoas, 'P');
    else if(op == "6.2") apagarTodos(pessoas, 'A');
    else if(op == "6.3") apagarTodos(pessoas, 'T');
}

void handleAniversariantes(Pessoa *pessoas[]) {
    listarAniversariantes(pessoas);
}