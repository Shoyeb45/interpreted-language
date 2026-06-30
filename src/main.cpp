#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// custom headers
#include "core/utils.hpp"
#include "evaluator/evaluator.hpp"
#include "parser/parser.hpp"
#include "scanner/scanner.hpp"
#include "executor/executor.hpp"
#include "interpreter/interpreter.hpp"
#include "resolver/resolver.hpp"

std::vector<Token> scan(std::vector<std::string> &file_contents) {
    auto tokens = LexicalScanner::scan_file(file_contents);

    if (LexicalScanner::is_lexical_error_present(tokens)) {
        LexicalScanner::print_lexical_error(tokens);
        std::exit(65);
    }

    return tokens;
}

int main(int argc, char *argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./run tokenize <filename>" << std::endl;
        std::cerr << "Usage: ./run parse <filename>" << std::endl;
        std::cerr << "Usage: ./run evaluate <filename>" << std::endl;
        std::cerr << "Usage: ./run run <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::vector<std::string> file_contents = read_file_contents(argv[2]);

    if (command == "tokenize") {
        std::vector<Token> tokens = LexicalScanner::scan_file(file_contents);

        bool is_error = false;
        for (auto &token : tokens) {
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
        auto tokens = scan(file_contents);
        
        Parser *parser = new Parser(tokens);
        parser->parse();

        if (parser->is_error()) {
            parser->report_error();
            std::exit(65);
        } else {
            parser->visualize();
        }

    } else if (command == "evaluate") {
        auto tokens = LexicalScanner::scan_file(file_contents);

        if (LexicalScanner::is_lexical_error_present(tokens)) {
            LexicalScanner::print_lexical_error(tokens);
            std::exit(65);
        }

        Parser *parser = new Parser(tokens);

        if (parser->is_error()) {
            parser->report_error();
            std::exit(65);
        }
        Expr *root = parser->parse();

        Evaluator *evaluator = new Evaluator(root);
        std::string evaluation_result = evaluator->evaluate();

        if (evaluator->errors.size() > 0) {
            for (int i = 0; i < evaluator->errors.size(); i++)
                std::cerr << evaluator->errors[i] << "\n";
            std::exit(70);
        }
        std::cout << evaluation_result << "\n";
    } else if (command == "run") {
        auto tokens = scan(file_contents);
        Parser* parser = new Parser(tokens);

        std::vector<Stmt *> stmts = parser->parse_stmt();
        if (parser->is_error()) {
            parser->report_error();
            std::exit(65);
        }
        // parser->visualize();

        Interpreter *interpreter = new Interpreter(stmts);
        Resolver *resolver = new Resolver(interpreter);
        resolver->resolve();    
        interpreter->execute();
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}
