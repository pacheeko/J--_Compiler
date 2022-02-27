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
    int a;
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
%token <strVal> SEMICOLON ";"
%token <strVal> COMMA ","
%token <strVal> TRUE "true"
%token <strVal> FALSE "false"
%token <strVal> BOOL "boolean"
%token <strVal> INT "int"
%token <strVal> VOID "void"
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
%type <funcdecl> functiondeclarator
%type <funcdecl> functiondeclaration
%type <funcdecl> functionheader
%type <strVal> type

/* Define the start symbol */
%start start


%%
start          : %empty {root = new Prog(filename);}
               | program {root = $1;}
               ;

program         : globaldeclarations {$$ = new Prog(filename); 
                                     $$->AddNode($1);
                                     if ($1->hasNext()){
                                            Decl* tmp = $1;
                                            while (tmp->hasNext()){
                                                tmp = tmp->getNext();
                                                $$->AddNode(tmp);
                                            }
                                        }}
                ;   

literal         : NUM 
                | STRING 
                | TRUE 
                | FALSE 
                ;

type            : BOOL  {$$ = $1;}
                | INT   {$$ = $1;}
                ;

globaldeclarations      : globaldeclaration {$$ = $1;}
                        | globaldeclarations globaldeclaration  {$$ = $2; $$->setNext($1);}
                        ;

globaldeclaration       : variabledeclaration   {$$ = $1;}
                        | functiondeclaration   {$$ = $1;}
                        | mainfunctiondeclaration   {$$ = $1;}
                        ;

variabledeclaration     : type identifier SEMICOLON {$$ = new VarDecl($1->c_str(), $2->c_str()); }
                        ;

identifier              : ID {$$ = $1;}
                        ;

functiondeclaration     : functionheader block {$$ = $1;}
                        ;

functionheader          : type functiondeclarator {$$ = $2; $$->SetRT($1->c_str());}
                        | VOID functiondeclarator {$$ = $2; $$->SetRT($1->c_str());}
                        ;

functiondeclarator      : identifier OPENPAR formalparameterlist CLOSEPAR {$$ = new FuncDecl($1->c_str());}
                        | identifier OPENPAR CLOSEPAR {$$ = new FuncDecl($1->c_str());}
                        ;

formalparameterlist     : formalparameter
                        | formalparameterlist COMMA formalparameter
                        ;

formalparameter         : type identifier
                        ;

mainfunctiondeclaration : mainfunctiondeclarator block
                        ;

mainfunctiondeclarator  : identifier OPENPAR CLOSEPAR    {$$ = new MainDecl($1->c_str()); }
                        ;

block                   : OPENBRACE blockstatements CLOSEBRACE 
                        | OPENBRACE CLOSEBRACE 
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
                        | relationalexpression GT additiveexpression
                        | relationalexpression LT additiveexpression
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