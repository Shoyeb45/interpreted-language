#include <string>
#include <vector>
#include "token.hpp"

namespace LexicalScanner {

std::vector<Token> scan_file(const std::vector<std::string> &file_contents);
bool is_lexical_error_present(const std::vector<Token> &tokens);
void print_lexical_error(const std::vector<Token> &tokens);

};