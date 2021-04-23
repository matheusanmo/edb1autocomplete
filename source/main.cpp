/* main.cpp */
#include <string>     // getline
#include <ios>        // boolalpha
#include <vector>
#include <istream>
#include <sstream>    // stringstream
#include <fstream>    // ifstream
#include <ostream>    // flush, endl
#include <iostream>   // cout, cin
#include <functional> // reference_wrapper, cref
#include <algorithm>  // sort, upper_bound, lower_bound, reverse

///////////////////
// `using` globais
///////////////////
using std::string;

////////////////////////////
// Macros do preprocessador
////////////////////////////
#define DBG_PRINTLINE std::cout << __FILE__ << ":" <<  __LINE__ << std::endl

//////////////////////////////////////////
// Forward declaration de funcoes globais
//////////////////////////////////////////
void test_routine();

/**
 * @brief Um termo composto por um peso `m_weight` e uma query `m_query`.
 */
struct Term {
    size_t    m_weight;
    string    m_query;
    /** @brief Construtor regular a partir de um peso (unsigned) e uma string.
     */
    Term(size_t weight=0, const string& query="") 
        : m_weight(weight)
        , m_query(query)
    { /* empty */ }
    /** @brief Constrói a partir de uma linha do arquivo de dados.
     *
     * Este método assume que as linhas seguem um padrão `\s*(\d*)\s*(.*)`, onde o 
     * primeiro grupo captura o peso do termo e o segundo a query.
     * @param   line    linha formatada como nos arquivos de dados.
     */
    explicit Term(const string& line) {
        std::stringstream ss{line};
        ss >> m_weight;
        ss >> m_query;
    }
    /** @brief Formata o objeto como string e envia para a stream. 
     */
    friend std::ostream& operator<<(std::ostream& os, const Term& term) {
        return os << "<Term weight=" << term.m_weight << ", query=\"" << term.m_query << "\">";
    }
}; // struct Term

/** @brief Functor para comparar Term pelo peso.
 */
struct CompTermWeight {
    /** lhs < rhs ? */
    bool operator()(const Term& lhs, const Term& rhs) {
        return (lhs.m_weight < rhs.m_weight);
    }
}; // struct CompTermWeight

/** @brief Functor para comparar Term pela query como um dicionário. Usado na ordenação inicial dos termos.
 */
struct CompTermLexicographic {
    bool operator()(const Term& lhs, const Term& rhs) {
        const string& str_lhs = lhs.m_query;
        const string& str_rhs = rhs.m_query;
        return str_lhs < str_rhs;
    }
}; // struct CompTermLexicographic

/** @brief Functor para comparar Term usado por `lower_bound`. 
 */
struct CompTermLB {
    bool operator()(const Term& lhs, const Term& rhs) {
        const string& str_rhs = rhs.m_query;
        const string& str_lhs = lhs.m_query.substr(0, str_rhs.size());
        return str_lhs < str_rhs;
    }
}; // struct CompTermLB

/** @brief Functor para comparar Term usado por `upper_bound`. 
 */
struct CompTermUB {
    /** lhs < rhs ? */
    bool operator()(const Term& lhs, const Term& rhs) {
        const string& str_lhs = lhs.m_query;
        const string& str_rhs = rhs.m_query.substr(0, str_lhs.size());
        return str_lhs < str_rhs;
    }
}; // struct CompTermUB

/**
 * Lida com tarefas que envolvem IO.
 */
class IOHandler {
    ////////////////////
    // Membros privados
    ////////////////////
    private:
        /** Stream que sera usada para printar queries. */
        std::istream& m_istream;
        /** Stream que sera usada para receber prefixos. */
        std::ostream& m_ostream;
        /** Nome do arquivo de texto contendo a database. */
        string        m_database_filename;
        friend void test_routine(void);

    ////////////////////
    // Metodos publicos
    ////////////////////
    public:
        /** @brief Construtor regular.
         *
         * @param   is          `istream` para receber string prefixo para busca de autosugestão.
         * @param   os          `ostream` para apresentação das sugestões e exibição de mensagens.
         * @param   dbfilename  string contendo localização do arquivo de texto de dados.
         */
        IOHandler(std::istream& is, std::ostream& os, string dbfilename) 
            : m_istream(is)
            , m_ostream(os)
            , m_database_filename(dbfilename)
        { /* empty */ }

        /** @brief Exibe mensagem pedindo input, recebe (pelo `m_istream`) e retorna string. 
         * */
        string request_line(void) const {
            m_ostream << " >>> Type a word and hit ENTER or a single dot (\'.\') to quit: " << std::flush;
            string ret_str{};
            m_istream.clear();
            std::getline(m_istream, ret_str);
            return ret_str;
        }

        /** @brief Printa os termos recebidos usando a stream de saída.
         * */
        void present_terms(const std::vector<std::reference_wrapper<const Term>>& terms) const {
            std::cout << " >>> The matches are:\n";
            for (const std::reference_wrapper<const Term>& term : terms) {
                m_ostream << term.get().m_query << "\n";
            }
            m_ostream << std::flush;
            return;
        }

        /** @brief Abre o arquivo de texto `m_database_filename` e carrega as linhas num vector. 
         * */
        std::vector<string> read_lines(void) const {
            std::vector<string> lines_ret{};
            std::ifstream ifs{m_database_filename};
            for (string line; std::getline(ifs, line); ) {
                lines_ret.push_back(line);
            }
            return lines_ret;
        }
}; //class IOHandler

/** 
 * Constrói e armazena a base de dados de termos de sugestão a partir de um vetor de linhas
 * e oferece métodos para pedir sugestões a partir de prefixos.
 */
class DBHandler {
    ////////////////////
    // Membros e metodos privados
    ////////////////////
    private:
        /** Database de termos (pesos e queries). */
        std::vector<Term> m_database;

        friend void test_routine(void);

    ////////////////////
    // Metodos publicos
    ////////////////////
    public:
        /** @brief Construtor regular a partir de um vector de strings como no arquivos de dados.
         */
        explicit DBHandler(const std::vector<string>& lines) {
            m_database.clear();
            bool firstline = true;
            for (const string& line : lines) {
                if (firstline) {
                    // Pular a primeira que contem apenas contagem de termos
                    firstline = false;
                    continue;
                }
                m_database.push_back(Term(line));
            }
            std::sort(m_database.begin(), m_database.end(), CompTermLexicographic{});
        }

        /** @brief Cria vetor de referências a termos. */
        std::vector<std::reference_wrapper<const Term>> get_terms_refs(const string& prefix) {
            // Encaixar prefix num Term para facilitar comparacao
            Term dummy_prefix{0u, prefix};
            // Inicializar vetor de referencias
            std::vector<std::reference_wrapper<const Term>> vec_ret{};
            // Apanhar lower/upper bounds
            auto lb = std::lower_bound(m_database.begin(), m_database.end(), dummy_prefix, CompTermLB{});
            auto ub = std::upper_bound(m_database.begin(), m_database.end(), dummy_prefix, CompTermUB{});
            // Colocar referencias no vetor de retorno
            while (lb != ub) 
                vec_ret.push_back(std::cref(*lb++));
            // Ordenar referencias por peso
            std::sort(vec_ret.begin(), vec_ret.end(), CompTermWeight{});
            std::reverse(vec_ret.begin(), vec_ret.end());
            return vec_ret;
        }

}; // class DBHandler

/**
 * Printa instruções de invocação do programa.
 */
void print_help() {
    using std::cout;
    using std::endl;
    cout << "Usage: autocomplete <database_file>\n";
    cout << "  Where <database_file> is the ascii file that contains the query terms and weights." << endl;
    return;
}

/**
 * "Testa" de forma manual as classes.
 */
void test_routine() {
    using std::cout;
    using std::endl;

    // requestline
    IOHandler ioh{std::cin, std::cout, "data_wiktionary.txt"};
    string str{ioh.request_line()};
    cout << str << endl;

    // present terms
    std::vector<Term> test_terms = { Term(1, "numero 1"), Term(2, "numero dois"), Term(999, "numero novenovenove") };
    std::vector<std::reference_wrapper<const Term>> test_crefs{};
    for (const Term& term : test_terms)
        test_crefs.push_back(std::cref(term));
    ioh.present_terms(test_crefs);

    // read_lines
    std::vector<string> lines{ioh.read_lines()};
    for (int i = 0; i < 10; i++) {
        cout << lines[i] << endl;
    }

    // dbhandler ctor
    DBHandler dbh{lines};
    for (auto i = 999; i < 1009; i++) {
        cout << dbh.m_database[i] << endl;
    }
    cout << "database size " << dbh.m_database.size() << endl;

    // get_terms_refs
    cout << "REFS" << endl;
    auto terms = dbh.get_terms_refs("pr");
    ioh.present_terms(terms);

    return;
}

/** 
 * main
 */
int main(int argc, char** argv) {
    // Printar ajuda e sair se não temos database.
    if (argc < 2) {
        print_help();
        return 1;
    }
    string database_file{argv[1]};
    IOHandler ioh{std::cin, std::cout, database_file};
    DBHandler dbh{ioh.read_lines()};
    while (true) {
        string prefix{ioh.request_line()};
        if (prefix == ".") {
            std::cout << " >>> Exiting." << std::endl;
            break;
        }
        std::vector<std::reference_wrapper<const Term>> terms = dbh.get_terms_refs(prefix);
        ioh.present_terms(terms);
    }

    return 0;
}
