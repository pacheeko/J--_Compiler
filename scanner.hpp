/********************************
 * Taken from Shankar Ganesh to be used as starter code.
 * *****************************/

#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <iostream>
#include <fstream>
#include <memory>

// Token enum
enum {
    T_ID = 1,
    T_NUM,
    T_STRING,
    T_ENDOFFILE,
    //Reserved words
    T_TRUE,
    T_FALSE,
    T_BOOL,
    T_INT,
    T_VOID,
    T_IF,
    T_ELSE,
    T_WHILE,
    T_BREAK,
    T_RETURN,
    //Operators
    T_ADD,
    T_SUB,
    T_MULT,
    T_DIV,
    T_MOD,
    T_LT,
    T_GT,
    T_LE,
    T_GE,
    T_ASSIGN,
    T_EQ,
    T_NEQ,
    T_NOT,
    T_AND,
    T_OR,
    //Other tokens
    T_OPENPAR,
    T_CLOSEPAR,
    T_OPENBRACE,
    T_CLOSEBRACE,
    T_SEMICOLON,
    T_COMMA
};

// Lexer class. Inherits yyFlexLexer
class CCLexer : public yyFlexLexer {
    public:

    CCLexer(std::istream *in) : yyFlexLexer(in) { yylineno = 1; }
    
    virtual ~CCLexer() = default;

    int yylex();

    int getLine() {return yylineno;}

    std::string lexeme;

};

// Prototype. Defined in scanner.l
std::unique_ptr<CCLexer> createLexer(std::istream* input);


inline char const* getName(int tok){
	switch (tok)
    {

    case T_ID:
        return "ID";

    case T_NUM:
        return "NUM";

    case T_STRING:
        return "STRING LITERAL";

    //Reserved Words    
    case T_TRUE:
        return "TRUE";

    case T_FALSE:
        return "FALSE";
    
    case T_BOOL:
        return "BOOL";
    
    case T_INT:
        return "INT";
    
    case T_VOID:
        return "VOID";

    case T_IF:
        return "IF";

    case T_ELSE:
        return "ELSE";

    case T_WHILE:
        return "WHILE";

    case T_BREAK:
        return "BREAK";

    case T_RETURN:
        return "RETURN";

    //Operators
    case T_ADD:
        return "+";

    case T_SUB:
        return "-";

    case T_MULT:
        return "*";

    case T_DIV:
        return "/";

    case T_MOD:
        return "%";

    case T_LT:
        return "<";

    case T_GT:
        return ">";

    case T_LE:
        return "<=";
    
    case T_GE:
        return ">=";

    case T_ASSIGN:
        return "=";

    case T_EQ:
        return "==";

    case T_NEQ:
        return "!=";

    case T_NOT:
        return "!";

    case T_AND:
        return "&&";

    case T_OR:
        return "||";

    //Other tokens
    case T_OPENPAR:
        return "(";

    case T_CLOSEPAR:
        return ")";
    
    case T_OPENBRACE:
        return "{";

    case T_CLOSEBRACE:
        return "}";

    case T_SEMICOLON:
        return ";";

    case T_COMMA:
        return ",";

    default:
        return "";
    }
}

#endif
