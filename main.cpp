/********************************
 * Code modified from Shankar Ganesh tutorial code
 * *****************************/

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>
#include "vector"
#include "scanner.hpp"
#include "parser.hh"
#include "codeGen.cpp"

int main(int argc, char **argv) {

    std::istream *input;

    std::ifstream file;

    extern AST* root;

    extern char* filename;
    
    filename = argv[1];

    if (argc != 2) {
        std::cerr << "You must provide exactly 1 command line argument: The file path you wish to parse." << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        file.open(argv[1]);

        if (!file.good())
        {
            std::cerr << "Error: " << strerror(errno) << "\n";
            return EXIT_FAILURE;
        }

        input = &file;
        }
    auto lexer = createLexer(input);
    auto parser = std::make_unique<JCC::Parser>(lexer);
   
    if( parser->parse() != 0 )
    {
        std::cerr << "Parse failed!!\n";
        if (file.is_open()) file.close();
        return 1;
    }
    root->reverseChildren();
    root = semanticAnalyzer(root);
    if (errors > 0) {
        std::cerr << errors << " error(s) found. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Generate the MIPS file from the AST
    generateCode(root);

    return 0;
}