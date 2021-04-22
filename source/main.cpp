#include <string>
using std::string;

#include <array>
#include <istream>
#include <ostream>

struct Term {
    size_type m_weight;
    string    m_query;
    Term(size_type weight, const string& query);
}; // struct Term

class IOHandler {
    private:
        std::istream m_istream;
        std::ostream m_ostream;
        string       m_database_filename;

    public:
        IOHandler(std::istream is, std::ostream os, string dbfilename);
        string request_line(void);
        void present_terms(const std::array<const Term&>& terms);
        std::array<string> read_lines(void);
}; //class IOHandler

class DBHandler {
    private:
        std::array<Term> m_database;
    public:
        explicit DBHandler(const std::array<string>& lines);
        std::array<Term> get_terms(string prefix);
        std::array<const Term&> get_terms_unstable_refs(string prefix);
}; // class DBHandler

void print_help() {
    return;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        print_help();
        return 1;
    }
    string database_file{argv[2]};
    // IOHandler ioh{std::cin, std::cout, string dbfilename};
    // DBHandler dbh{ioh.read_lines()};
    // while (true) {
    //     string prefix{ioh.request_line()};
    //     std::array<const Term&> terms{dbh.get_terms_unstable_refs(prefix)};
    //     ioh.present_terms(terms)
    // }
    return 0;
}
