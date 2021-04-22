/* main.cpp */
#include <string>     // getline
#include <vector>
#include <istream>
#include <sstream>    // stringstream
#include <fstream>    // ifstream
#include <ostream>    // flush, endl
#include <iostream>   // cout, cin
#include <functional> // reference_wrapper, cref

#define DBG_PRINTLINE std::cout << __FILE__ ":" << __LINE__ << std::endl

using std::string;

/**
 * term
 */
struct Term {
    size_t    m_weight;
    string    m_query;
    Term(size_t weight=0, const string& query="") 
        : m_weight(weight)
        , m_query(query)
    { /* empty */ }
    Term(const string& line) {
        std::stringstream ss{line};
        ss >> m_weight;
        ss >> m_query;
    }
}; // struct Term

/**
 * iohandler
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

    ////////////////////
    // Metodos publicos
    ////////////////////
    public:
        /** @brief Construtor. */
        IOHandler(std::istream& is, std::ostream& os, string dbfilename) 
            : m_istream(is)
            , m_ostream(os)
            , m_database_filename(dbfilename)
        { /* empty */ }

        /** @brief Exibe mensagem pedindo input, recebe e retorna string. 
         * */
        string request_line(void) const {
            m_ostream << ">>> Type a word and hit ENTER or <ctrl>+d to quit: " << std::flush;
            string ret_str{};
            m_istream.clear();
            std::getline(m_istream, ret_str);
            return ret_str;
        }

        /** @brief Printa os termos recebidos usando a stream de saida.
         * */
        void present_terms(const std::vector<std::reference_wrapper<const Term>>& terms) const {
            std::cout << ">>> The matches are:\n";
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
 * dbhandler
 */
class DBHandler {
    ////////////////////
    // Membros privados
    ////////////////////
    private:
        std::vector<Term> m_database;

    ////////////////////
    // Metodos publicos
    ////////////////////
    public:
        explicit DBHandler(const std::vector<string>& lines) {
            m_database.clear();
            bool firstline = true;
            for (const string& line : lines) {
                if (firstline) {
                    firstline = false;
                    continue;
                }
                m_database.push_back(Term(line));
            }
        }
        std::vector<Term> get_terms(string prefix);
        std::vector<const Term&> get_terms_unstable_refs(string prefix);
}; // class DBHandler

/**
 * print help
 */
void print_help() {
    using std::cout;
    using std::endl;
    cout << "Usage: autocomplete <database_file>\n";
    cout << "  Where <database_file> is the ascii file that contains the query terms and weights." << endl;
    return;
}

/**
 * test_routine
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


    return;
}

/** 
 * main
 */
int main(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return 1;
    }
    string database_file{argv[1]};
    test_routine();
    // IOHandler ioh{std::cin, std::cout, string dbfilename};
    // DBHandler dbh{ioh.read_lines()};
    // while (true) {
    //     string prefix{ioh.request_line()};
    //     std::vector<const Term&> terms{dbh.get_terms_unstable_refs(prefix)};
    //     ioh.present_terms(terms)
    // }
    return 0;
}
