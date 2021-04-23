# `autocomplete`
`autocomplete` é um programa em C++ que lê um arquivo de texto contendo pares query-peso e calcula e apresenta sugestões baseadas num prefixo inserido pelo usuário.

## Compilação
`autocomplete` precisa de um compilador com suporte C++11. O seguinte comando constrói o binário `autocomplete` no diretório atual:

    clang++ -std=c++11 -Wall -Wextra -pedantic -a autocomplete main.cpp

## Invocação
`autocomplete` recebe um único argumento de linha de comando, que deve ser o endereço do arquivo de texto que contém os termos. 

## Itens pedidos
- Read, validate, and process command line arguments: lê mas não testa validade ou existência do arquivo de entrada.
- Read the input data file and store the information in memory: carrega adequadamente um arquivo de termos bem formatado.
- Display a welcome message explaining what the system does and how it operates: o programa confirma o arquivo carregado e a quantidade de termos.
- The project contains at least one class: o projeto contém pelo menos uma classe.
- Reads the user query string: a entrada do usuário é feita como pedido, exceto pela forma como o programa é terminado. O comando do trabalho pede que o programa acabe quando receber `EOF`; esta implementação sai do fluxo quando recebe como prefixo um único ponto ("`.`"). O programa avisa isto para o usuário.
- Correctly returns a list of matching terms, sorted by decreasing relevance: o programa escolhe corretamente quais sugestões devem ser apresentadas e ordena pelo peso de forma decrescente.
- The implementation is efficient, using binary search algorithms: a implementação usa as funções da STL sempre que possível, e a busca é feita usando `std::upper_bound` e `std::lower_bound`. De acordo com a documentaçao disponível sobre [`std::upper_bound`](https://en.cppreference.com/w/cpp/algorithm/upper_bound) e [std::lower_bound](https://en.cppreference.com/w/cpp/algorithm/lower_bound), ambas têm complexidade da ordem de O(log n), compatível com a da busca binária.
- Allows the user to enter new queries: o usuário decide quando quer parar de pedir sugestões.
- Compiling and/or runtime errors: a compilação lança erros ou avisos.
- Missing code documentation in Doxygen style: todos métodos e classes públicas estão documentados.
- Memory leak: Valgrind não acusa vazamentos ou erros de acesso.
- Missing README file: o arquivo readme contempla os pontos pedidos.

## Autoria
- Professor: SELAN RODRIGUES DOS SANTOS 
- Componente: DIM0120 - LINGUAGEM DE PROGRAMAÇÃO I T01 (2020.2)
- Aluno: Matheus Andrade Monteiro

