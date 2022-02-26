/* Create C++ parser */
%skeleton "lalr1.cc"
%require "3.5.1"

/* Generate headers */
%defines

%define api.namespace {JCC}
%define api.parser.class {Parser}

%locations


%code requires{
    #include <vector>
    #include <string>
    #include <memory>
    #include "ast.hpp"
    
    namespace JCC {
        class Lexer;
    }
}

%parse-param {std::unique_ptr<JCC::Lexer> &lexer}

%code {
    #include <fstream>
    #include "scanner.hpp"

    // Define yylex
    #undef yylex
    #define yylex lexer->yylex

    AST* root = nullptr;
    char* filename;
}


%define api.token.prefix {T_}

/* Semantic type / YYSTYPE */
%union{
    std::string* strVal;
    int ival;
    Prog *prog;
    Stmt *stmt;
    Decl *decl;
    Exp *exp;
    MainDecl *maindecl;
    VarDecl *vardecl;
    FuncDecl *funcdecl;

};

%token ADD "+"
%token SUB "-"
%token DIV "/"
%token MULT "*"
%token MOD "%"
%token LT "<"
%token GT ">"
%token LE "<="
%token GE ">="
%token EQ "=="
%token NEQ "!="
%token NOT "!"
%token AND "&&"
%token OR "||"
%token ASSIGN "="
%token OPENPAR "("
%token CLOSEPAR ")"
%token <strVal> OPENBRACE "{"
%token <strVal> CLOSEBRACE "}"
%token SEMICOLON ";"
%token COMMA ","
%token <strVal> TRUE "true"
%token <strVal> FALSE "false"
%token <strVal> BOOL "boolean"
%token <strVal> INT "int"
%token VOID "void"
%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token BREAK "break"
%token RETURN "return"
%token <strVal> STRING "string"
%token <strVal> ID "id"
%token <ival> NUM "number"

%type <prog> program
%type <maindecl> mainfunctiondeclaration
%type <maindecl> mainfunctiondeclarator
%type <strVal> identifier
%type <decl> globaldeclaration
%type <decl> globaldeclarations
%type <vardecl> variabledeclaration
%type <funcdecl> functiondeclaration
/* Define the start symbol */
%start start


%%
start          : %empty {root = new Prog(filename);}
               | program {root = $1;}

program          : /* empty */   
                | globaldeclarations {$$ = new Prog(filename);
                  std::cout << "program" << std::endl;
                  $$->AddNode($1);}
                ;   

literal         : NUM {std::cout << std::to_string($1) << std::endl; } 
                | STRING {std::cout << $1 << std::endl; } 
                | TRUE {std::cout << $1 << std::endl; } 
                | FALSE {std::cout << $1 << std::endl; } 
                ;

type            : BOOL
                | INT
                ;

globaldeclarations      : globaldeclaration {$$ = $1; {std::cout << "global dec 2" << std::endl;}}
                        | globaldeclarations globaldeclaration  {$$ = $1;}
                        ;

globaldeclaration       : variabledeclaration   {$$ = $1;}
                        | functiondeclaration   {$$ = $1;}
                        | mainfunctiondeclaration   {$$ = $1; {std::cout << "global dec 1" << std::endl;}}
                        ;

variabledeclaration     : type identifier SEMICOLON
                        ;

identifier              : ID {std::cout << $1->c_str() << std::endl; } 
                        ;

functiondeclaration     : functionheader block
                        ;

functionheader          : type functiondeclarator
                        | VOID functiondeclarator
                        ;

functiondeclarator      : identifier OPENPAR formalparameterlist CLOSEPAR
                        | identifier OPENPAR CLOSEPAR
                        ;

formalparameterlist     : formalparameter
                        | formalparameterlist COMMA formalparameter
                        ;

formalparameter         : type identifier
                        ;

mainfunctiondeclaration : mainfunctiondeclarator block {std::cout << "mainfuncdeclaration" << std::endl;}
                        ;

mainfunctiondeclarator  : identifier OPENPAR CLOSEPAR    {std::cout << "mainfuncdeclarator" << std::endl;
                                                          $$ = new MainDecl($1->c_str()); }
                        ;

block                   : OPENBRACE blockstatements CLOSEBRACE {std::cout << "blockstatements done" << std::endl;}
                        | OPENBRACE CLOSEBRACE  {std::cout << "no blocks" << std::endl;}
                        ;

blockstatements         : blockstatement
                        | blockstatements blockstatement
                        ;

blockstatement          : variabledeclaration
                        | statement
                        ;

statement               : block
                        | SEMICOLON
                        | statementexpression SEMICOLON
                        | BREAK SEMICOLON
                        | RETURN expression SEMICOLON
                        | RETURN SEMICOLON
                        | IF OPENPAR expression CLOSEPAR statement
                        | IF OPENPAR expression CLOSEPAR statement ELSE statement
                        | WHILE OPENPAR expression CLOSEPAR statement
                        ;

statementexpression     : assignment
                        | functioninvocation
                        ;

primary                 : literal
                        | OPENPAR expression CLOSEPAR
                        | functioninvocation
                        ;

argumentlist            : expression
                        | argumentlist COMMA expression
                        ;

functioninvocation      : identifier OPENPAR argumentlist CLOSEPAR
                        | identifier OPENPAR CLOSEPAR
                        ;

postfixexpression       : primary
                        | identifier
                        ;

unaryexpression         : SUB unaryexpression
                        | NOT unaryexpression
                        | postfixexpression
                        ;

multiplicativeexpression: unaryexpression
                        | multiplicativeexpression MULT unaryexpression
                        | multiplicativeexpression DIV unaryexpression
                        | multiplicativeexpression MOD unaryexpression
                        ;

additiveexpression      : multiplicativeexpression
                        | additiveexpression ADD multiplicativeexpression
                        | additiveexpression SUB multiplicativeexpression
                        ;

relationalexpression    : additiveexpression
                        | relationalexpression LT additiveexpression
                        | relationalexpression GT additiveexpression
                        | relationalexpression LE additiveexpression
                        | relationalexpression GE additiveexpression
                        ;

equalityexpression      : relationalexpression
                        | equalityexpression EQ relationalexpression
                        | equalityexpression NEQ relationalexpression
                        ;

conditionalandexpression: equalityexpression
                        | conditionalandexpression AND equalityexpression
                        ;

conditionalorexpression : conditionalandexpression
                        | conditionalorexpression OR conditionalandexpression
                        ;

assignmentexpression    : conditionalorexpression
                        | assignment
                        ;

assignment              : identifier ASSIGN assignmentexpression 
                        ;

expression              : assignmentexpression 
                        ;

%%

/* Parser will call this function when it fails to parse */
/* Tip: You can store the current token in the lexer to output meaningful error messages */
void JCC::Parser::error(const location_type &loc, const std::string &errmsg)
{
   std::cerr << "Error: " << errmsg << " at " << loc << "\n";
}

