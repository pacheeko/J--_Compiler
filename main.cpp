/********************************
 * Code modified from Shankar Ganesh tutorial code
 * *****************************/

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>
#include "scanner.hpp"
#include "parser.hh"
#include "vector"

int main(int argc, char **argv) {

    std::istream *input;

    std::ifstream file;

    extern AST* root;

    extern char* filename;
    
    filename = argv[1];

    if (argc == 2)
    {
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
    std::cout << "Parse Complete! Printing AST..." << std::endl;
    root->Print();  //Prints out the entire abstract syntax tree

    return 0;
}