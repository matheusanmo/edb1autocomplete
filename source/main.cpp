#include <string> // getline
#include <vector>
#include <istream>
#include <ostream>  // flush, endl
#include <iostream> // cout, cin

#define DBG_PRINTLINE std::cout << __FILE__ ":" << __LINE__ << std::endl

using std::string;

/**
 * term
 */
struct Term {
    size_t    m_weight;
    string    m_query;
    Term(size_t weight, const string& query);
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

    //////////
    // Testes
    //////////
    private:
        static bool test_constructor(void);
        static bool test_request_line(void);
        static bool test_present_terms(void);
        static bool test_readlines(void);

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
        string request_line(void) {
            m_ostream << "Type a word and hit ENTER or <ctrl>+d to quit: " << std::flush;
            string ret_str{};
            m_istream.clear();
            std::getline(m_istream, ret_str);
            return ret_str;
        }

        /** @brief Printa os termos recebidos usando a stream de saida.
         * */
        void present_terms(const std::vector<const Term&>& terms);
        /** @brief Abre o arquivo de texto `m_database_filename` e carrega as linhas num vector. 
         * */
        std::vector<string> read_lines(void);
}; //class IOHandler

/** 
 * dbhandler
 */
class DBHandler {
    private:
        std::vector<Term> m_database;
    public:
        explicit DBHandler(const std::vector<string>& lines);
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

    IOHandler ioh{std::cin, std::cout, ""};
    string str{ioh.request_line()};
    cout << str << endl;

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
