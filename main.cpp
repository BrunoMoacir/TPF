#include <iostream>
#include "data.h"
#include "pessoas.h"
#include "aluno.h"
#include "professor.h"

// A variável TAM agora é declarada em pessoas.cpp e informada aqui como externa.
// Isso é uma prática melhor do que ter múltiplas definições.
extern int TAM;

int main()
{
    // Para simplificar, o array 'tipos' também foi movido para dentro de pessoas.cpp
    // e é acessado pelas funções que precisam dele.
    Pessoa *pessoas[MAX];
    
    // Carrega os dados salvos anteriormente ao iniciar o programa
    abertura(pessoas); 

    int opcao;
    do
    {
        std::cout << "\n========== MENU PRINCIPAL ==========\n";
        std::cout << "0 - Sair do programa\n";
        std::cout << "1 - Cadastrar pessoa\n";
        std::cout << "2 - Listar pessoas\n";
        std::cout << "3 - Pesquisar por nome\n";
        std::cout << "4 - Pesquisar por CPF\n";
        std::cout << "5 - Excluir pessoa\n";
        std::cout << "6 - Apagar todos os registros\n";
        std::cout << "7 - Aniversariantes do mes\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;
        std::cin.ignore(); // Limpa o buffer do teclado

        switch (opcao)
        {
        case 0:
            despedida(pessoas); // Salva tudo antes de sair
            break;
        case 1:
            submenuCadastro(pessoas);
            break;
        case 2:
            submenuListagem(pessoas);
            break;
        case 3:
            submenuPesquisaNome(pessoas);
            break;
        case 4:
            submenuPesquisaCPF(pessoas);
            break;
        case 5:
            submenuExcluir(pessoas);
            break;
        case 6:
            submenuApagarTodos(pessoas);
            break;
        case 7:
            aniversariantesMes(pessoas);
            break;
        default:
            std::cout << "Opcao invalida! Tente novamente.\n";
        }
    } while (opcao != 0);

    std::cout << "\nPrograma finalizado.\n";
    return 0;
}