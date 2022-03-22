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

    // Variables used to print out information about the abstract syntax tree
    AST* root = nullptr;
    char* filename;
}
//
%define api.token.prefix {T_}

/* Semantic type / YYSTYPE */
%union{
    std::string* strVal;
    int ival;
    uint8_t enumVal;
    Prog *prog;
    AST *ast;
    MainDecl *maindecl;
    VarDecl *vardecl;
    FuncDecl *funcdecl;
    Param *param;
    Block *block;
    Id *id;
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
%token OPENBRACE "{"
%token CLOSEBRACE "}"
%token SEMICOLON ";"
%token COMMA ","
%token TRUE "true"
%token FALSE "false"
%token BOOL "boolean"
%token INT "int"
%token VOID "void"
%token IF "if"
%token ELSE "else"
%token WHILE "while"
%token BREAK "break"
%token RETURN "return"
%token <strVal> STRING "string"
%token <strVal> ID "id"
%token <ival> NUM "number"

%type <ast> program
%type <ast> mainfunctiondeclaration
%type <ast> mainfunctiondeclarator
%type <ast> identifier
%type <ast> globaldeclaration
%type <ast> globaldeclarations
%type <ast> variabledeclaration
%type <ast> functiondeclarator
%type <ast> functiondeclaration
%type <ast> functionheader
%type <enumVal> type
%type <ast> formalparameter
%type <ast> formalparameterlist
%type <ast> block
%type <ast> blockstatement
%type <ast> blockstatements
%type <ast> statement
%type <ast> statementexpression
%type <ast> expression
%type <ast> equalityexpression
%type <ast> assignment
%type <ast> assignmentexpression
%type <ast> postfixexpression
%type <ast> literal
%type <ast> relationalexpression
%type <ast> additiveexpression
%type <ast> multiplicativeexpression
%type <ast> unaryexpression
%type <ast> conditionalandexpression
%type <ast> conditionalorexpression
%type <ast> argumentlist
%type <ast> functioninvocation
%type <ast> primary

/* Define the start symbol */
%start start

/* Left associativity */
%left "+" "-" "*" "/";

/* *** Grammer taken from CPSC 411 d2l page *** */
%%
start          : %empty {root = new Prog(filename);}
               | program {root = $1;}
               ;
/* 
Only one program node for the abstract syntax tree 
Each global declaration is a child of the program node
   */
program         : globaldeclarations {$$ = new Prog(filename); 
                                     $$->AddNode($1);
                                     if ($1->hasNext()){
                                            AST* tmp = $1;
                                            while (tmp->hasNext()){
                                                tmp = tmp->getNext();
                                                $$->AddNode(tmp);
                                            }
                                        }}
                ;   


literal         : NUM {$$ = new Num(@$.begin.line, $1);}
                | STRING {$$ = new String(@$.begin.line, $1->c_str());}
                | TRUE {$$ = new Literal(@$.begin.line, Reserved::TRUE);}
                | FALSE {$$ = new Literal(@$.begin.line, Reserved::FALSE);}
                ;

type            : BOOL {$$ = Reserved::BOOL;}
                | INT {$$ = Reserved::INT;}
                ;

globaldeclarations      : globaldeclaration 
                        | globaldeclarations globaldeclaration  {$$ = $2; $$->setNext($1);}
                        ;

globaldeclaration       : variabledeclaration   
                        | functiondeclaration  
                        | mainfunctiondeclaration 
                        ;

variabledeclaration     : type identifier SEMICOLON {$$ = new VarDecl(@$.begin.line, $1, $2->getName().c_str()); $$->AddNode($2);}
                        ;

identifier              : ID {$$ = new Id(@$.begin.line, $1->c_str());}
                        ;

functiondeclaration     : functionheader block {$$ = $1; $$->AddNode($2);}
                        ;

functionheader          : type functiondeclarator {$$ = $2; $$->setType($1);}
                        | VOID functiondeclarator {$$ = $2; $$->setType(Reserved::VOID);}
                        ;

functiondeclarator      : identifier OPENPAR formalparameterlist CLOSEPAR {$$ = new FuncDecl(@$.begin.line, $1->getName().c_str());
                                                                            $$->AddNode($3);
                                                                            $$->addParam();
                                                                            if ($3->hasNext()){
                                                                                AST* tmp = $3;
                                                                                while (tmp->hasNext()){
                                                                                    tmp = tmp->getNext();
                                                                                    $$->AddNode(tmp);
                                                                                    $$->addParam();
                                                                                }
                                                                            }}
                        | identifier OPENPAR CLOSEPAR {$$ = new FuncDecl(@$.begin.line, $1->getName().c_str());} 
                        ;

formalparameterlist     : formalparameter 
                        | formalparameterlist COMMA formalparameter {$3->setNext($1); $$ = $3;}
                        ;

formalparameter         : type identifier {$$ = new Param(@$.begin.line, $1, $2->getName().c_str());}
                        ;

mainfunctiondeclaration : mainfunctiondeclarator block {$$ = $1; $$->AddNode($2);}
                        ;

mainfunctiondeclarator  : identifier OPENPAR CLOSEPAR    {$$ = new MainDecl(@$.begin.line, $1->getName().c_str());}
                        ;

block                   : OPENBRACE blockstatements CLOSEBRACE {$$ = new Block(@$.begin.line);
                                                                $$->AddNode($2);
                                                                if ($2->hasNext()){
                                                                    AST* tmp = $2;
                                                                    while (tmp->hasNext()){
                                                                        tmp = tmp->getNext();
                                                                        $$->AddNode(tmp);
                                                                    }
                                                                } }
                        | OPENBRACE CLOSEBRACE                  {$$ = new Block(@$.begin.line);}
                        ;

blockstatements         : blockstatement  
                        | blockstatements blockstatement {$$ = $2; $$->setNext($1);}
                        ;

blockstatement          : variabledeclaration 
                        | statement 
                        ;

statement               : block 
                        | SEMICOLON {$$ = new NullStmt(@$.begin.line);}
                        | statementexpression SEMICOLON
                        | BREAK SEMICOLON {$$ = new BreakStmt(@$.begin.line);}
                        | RETURN expression SEMICOLON {$$ = new RetStmt(@$.begin.line); $$->AddNode($2);}
                        | RETURN SEMICOLON {$$ = new RetStmt(@$.begin.line);}
                        | IF OPENPAR expression CLOSEPAR statement {$$ = new IfStmt(@$.begin.line); $$->AddNode($3); $$->AddNode($5);}
                        | IF OPENPAR expression CLOSEPAR statement ELSE statement {$$ = new IfStmt(@$.begin.line); $$->AddNode($3); $$->AddNode($5); ElseStmt* es = new ElseStmt(@$.begin.line); es->AddNode($7); $$->AddNode(es);}
                        | WHILE OPENPAR expression CLOSEPAR statement {$$ = new WhileStmt(@$.begin.line); $$->AddNode($3); $$->AddNode($5);}
                        ;

statementexpression     : assignment 
                        | functioninvocation 
                        ;

primary                 : literal
                        | OPENPAR expression CLOSEPAR {$$ = $2;}
                        | functioninvocation
                        ;

argumentlist            : expression
                        | argumentlist COMMA expression {$3->setNext($1); $$ = $3;}
                        ;

functioninvocation      : identifier OPENPAR argumentlist CLOSEPAR {$$ = new FuncCall(@$.begin.line, $1->getName().c_str());
                                                                    $$->AddNode($3);
                                                                    if ($3->hasNext()){
                                                                        AST* tmp = $3;
                                                                        while (tmp->hasNext()){
                                                                            tmp = tmp->getNext();
                                                                            $$->AddNode(tmp);
                                                                        }
                                                                    }}
                        | identifier OPENPAR CLOSEPAR   {$$ = new FuncCall(@$.begin.line, $1->getName().c_str());}
                        ;

postfixexpression       : primary 
                        | identifier {$$ = new Id(@$.begin.line, $1->getName().c_str());}
                        ;

unaryexpression         : SUB unaryexpression {$$ = new Arithmetic(@$.begin.line, Oper::SUB); $$->AddNode($2);}
                        | NOT unaryexpression {$$ = new Logical(@$.begin.line, Oper::NOT); $$->AddNode($2);}  
                        | postfixexpression {}
                        ;

multiplicativeexpression: unaryexpression
                        | multiplicativeexpression MULT unaryexpression {$$ = new Arithmetic(@$.begin.line, Oper::MULT); $$->AddNode($1); $$->AddNode($3);}
                        | multiplicativeexpression DIV unaryexpression {$$ = new Arithmetic(@$.begin.line, Oper::DIV); $$->AddNode($1); $$->AddNode($3);}
                        | multiplicativeexpression MOD unaryexpression {$$ = new Arithmetic(@$.begin.line, Oper::MOD); $$->AddNode($1); $$->AddNode($3);}
                        ;

additiveexpression      : multiplicativeexpression
                        | additiveexpression ADD multiplicativeexpression {$$ = new Arithmetic(@$.begin.line, Oper::ADD); $$->AddNode($1); $$->AddNode($3);}
                        | additiveexpression SUB multiplicativeexpression {$$ = new Arithmetic(@$.begin.line, Oper::SUB); $$->AddNode($1); $$->AddNode($3);}
                        ;

relationalexpression    : additiveexpression
                        | relationalexpression GT additiveexpression {$$ = new Compare(@$.begin.line, Oper::GT); $$->AddNode($1); $$->AddNode($3);}
                        | relationalexpression LT additiveexpression {$$ = new Compare(@$.begin.line, Oper::LT); $$->AddNode($1); $$->AddNode($3);}
                        | relationalexpression LE additiveexpression {$$ = new Compare(@$.begin.line, Oper::LE); $$->AddNode($1); $$->AddNode($3);}
                        | relationalexpression GE additiveexpression {$$ = new Compare(@$.begin.line, Oper::GE); $$->AddNode($1); $$->AddNode($3);}
                        ;

equalityexpression      : relationalexpression
                        | equalityexpression EQ relationalexpression {$$ = new Compare(@$.begin.line, Oper::EQ); $$->AddNode($1); $$->AddNode($3);}
                        | equalityexpression NEQ relationalexpression {$$ = new Compare(@$.begin.line, Oper::NEQ); $$->AddNode($1); $$->AddNode($3);}
                        ;

conditionalandexpression: equalityexpression
                        | conditionalandexpression AND equalityexpression {$$ = new Logical(@$.begin.line, Oper::AND); $$->AddNode($1); $$->AddNode($3);}
                        ;

conditionalorexpression : conditionalandexpression
                        | conditionalorexpression OR conditionalandexpression {$$ = new Logical(@$.begin.line, Oper::OR); $$->AddNode($1); $$->AddNode($3);}
                        ;

assignmentexpression    : conditionalorexpression 
                        | assignment
                        ;

assignment              : identifier ASSIGN assignmentexpression {$$ = new AssnStmt(@$.begin.line, $1->getName().c_str()); $$->AddNode($1); $$->AddNode($3);}
                        ;

expression              : assignmentexpression
                        ;
   
%%

/* Parser will call this function when it fails to parse */
void JCC::Parser::error(const location_type &loc, const std::string &errmsg)
{
   std::cerr << "Error: " << errmsg << " at " << loc << "\n";
}
