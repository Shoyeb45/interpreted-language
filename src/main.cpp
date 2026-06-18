#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// custom headers
#include "core/utils.hpp"
#include "scanner/scanner.hpp"
#include "parser/parser.hpp"


int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./run tokenize <filename>" << std::endl;
        std::cerr << "Usage: ./run parse <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::vector<std::string> file_contents = read_file_contents(argv[2]);
    
    if (command == "tokenize") {
        std::vector<Token> tokens = LexicalScanner::scan_file(file_contents);

        bool is_error = false;
        for (auto &token: tokens) {
            if (token.is_error()) {
                std::cerr << token.to_lexical_error() << "\n";
                is_error = true;
                continue;
            }
            std::cout << token << "\n";
        }
        
        if (is_error) {
            std::exit(65);
        }
    } else if (command == "parse") {
        auto tokens = LexicalScanner::scan_file(file_contents);
        if (LexicalScanner::is_lexical_error_present(tokens)) {
            std::cerr << "Lexical errors found\n";
            LexicalScanner::print_lexical_error(tokens);
            std::exit(65);
        }

        Parser *parser = new Parser(tokens);
        parser->parse();
        parser->visualize();        

    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
