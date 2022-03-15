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
    Stmt *stmt;
    Decl *decl;
    Exp *exp;
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
%type <enumVal> type
%type <param> formalparameter
%type <decl> formalparameterlist
%type <stmt> block
%type <stmt> blockstatement
%type <stmt> blockstatements
%type <stmt> statement
%type <stmt> statementexpression
%type <exp> expression
%type <exp> equalityexpression
%type <stmt> assignment
%type <exp> assignmentexpression
%type <exp> postfixexpression
%type <exp> literal
%type <exp> relationalexpression
%type <exp> additiveexpression
%type <exp> multiplicativeexpression
%type <exp> unaryexpression
%type <exp> conditionalandexpression
%type <exp> conditionalorexpression
%type <exp> argumentlist
%type <exp> functioninvocation
%type <exp> primary

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
                                            Decl* tmp = $1;
                                            while (tmp->hasNext()){
                                                tmp = tmp->getNext();
                                                $$->AddNode(tmp);
                                            }
                                        }}
                ;   


literal         : NUM {$$ = new Num($1);}
                | STRING {$$ = new String($1->c_str());}
                | TRUE {$$ = new Literal(Reserved::TRUE);}
                | FALSE {$$ = new Literal(Reserved::FALSE);}
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

variabledeclaration     : type identifier SEMICOLON {$$ = new VarDecl($1, $2->c_str()); }
                        ;

identifier              : ID {std::cout << @$.begin << std::endl;}
                        ;

functiondeclaration     : functionheader block {$$ = $1; $$->AddNode($2);}
                        ;

functionheader          : type functiondeclarator {$$ = $2; $$->SetRT($1);}
                        | VOID functiondeclarator {$$ = $2; $$->SetRT(Reserved::VOID);}
                        ;

functiondeclarator      : identifier OPENPAR formalparameterlist CLOSEPAR {$$ = new FuncDecl($1->c_str());
                                                                            $$->AddNode($3);
                                                                            if ($3->hasNext()){
                                                                                Decl* tmp = $3;
                                                                                while (tmp->hasNext()){
                                                                                    tmp = tmp->getNext();
                                                                                    $$->AddNode(tmp);
                                                                                }
                                                                            } }
                        | identifier OPENPAR CLOSEPAR {$$ = new FuncDecl($1->c_str());}
                        ;

formalparameterlist     : formalparameter
                        | formalparameterlist COMMA formalparameter {$$ = $3; $$->setNext($1);}
                        ;

formalparameter         : type identifier {$$ = new Param($1, $2->c_str());}
                        ;

mainfunctiondeclaration : mainfunctiondeclarator block {$$ = $1; $$->AddNode($2);}
                        ;

mainfunctiondeclarator  : identifier OPENPAR CLOSEPAR    {$$ = new MainDecl($1->c_str()); }
                        ;

block                   : OPENBRACE blockstatements CLOSEBRACE {$$ = new Block();
                                                                $$->AddNode($2);
                                                                if ($2->hasNext()){
                                                                    Stmt* tmp = $2;
                                                                    while (tmp->hasNext()){
                                                                        tmp = tmp->getNext();
                                                                        $$->AddNode(tmp);
                                                                    }
                                                                } }
                        | OPENBRACE CLOSEBRACE                  {$$ = new Block();}
                        ;

blockstatements         : blockstatement  
                        | blockstatements blockstatement {$$ = $2; $$->setNext($1);}
                        ;

blockstatement          : variabledeclaration 
                        | statement 
                        ;

statement               : block 
                        | SEMICOLON {$$ = new NullStmt();}
                        | statementexpression SEMICOLON
                        | BREAK SEMICOLON {$$ = new BreakStmt();}
                        | RETURN expression SEMICOLON {$$ = new RetStmt(); $$->AddNode($2);}
                        | RETURN SEMICOLON {$$ = new RetStmt();}
                        | IF OPENPAR expression CLOSEPAR statement {$$ = new IfStmt(); $$->AddNode($3); $$->AddNode($5);}
                        | IF OPENPAR expression CLOSEPAR statement ELSE statement {$$ = new IfStmt(); $$->AddNode($3); $$->AddNode($5); ElseStmt* es = new ElseStmt(); es->AddNode($7); $$->AddNode(es);}
                        | WHILE OPENPAR expression CLOSEPAR statement {$$ = new WhileStmt(); $$->AddNode($3); $$->AddNode($5);}
                        ;

statementexpression     : assignment 
                        | functioninvocation 
                        ;

primary                 : literal
                        | OPENPAR expression CLOSEPAR {$$ = $2;}
                        | functioninvocation
                        ;

argumentlist            : expression
                        | argumentlist COMMA expression {$$ = $3; $$->setNext($1);}
                        ;

functioninvocation      : identifier OPENPAR argumentlist CLOSEPAR {$$ = new FuncCall($1->c_str()); 
                                                                    $$->AddNode($3);
                                                                    if ($3->hasNext()){
                                                                        Exp* tmp = $3;
                                                                        while (tmp->hasNext()){
                                                                            tmp = tmp->getNext();
                                                                            $$->AddNode(tmp);
                                                                        }
                                                                    }}
                        | identifier OPENPAR CLOSEPAR   {$$ = new FuncCall($1->c_str());}
                        ;

postfixexpression       : primary 
                        | identifier {$$ = new Id($1->c_str());}
                        ;

unaryexpression         : SUB unaryexpression {$$ = new Arithmetic(Oper::SUB, $2);}
                        | NOT unaryexpression {$$ = new Logical(Oper::NOT, $2);}  
                        | postfixexpression {}
                        ;

multiplicativeexpression: unaryexpression
                        | multiplicativeexpression MULT unaryexpression {$$ = new Arithmetic(Oper::MULT, $1, $3) ;}
                        | multiplicativeexpression DIV unaryexpression {$$ = new Arithmetic(Oper::DIV, $1, $3) ;}
                        | multiplicativeexpression MOD unaryexpression {$$ = new Arithmetic(Oper::MOD, $1, $3) ;}
                        ;

additiveexpression      : multiplicativeexpression
                        | additiveexpression ADD multiplicativeexpression {$$ = new Arithmetic(Oper::ADD, $1, $3) ;}
                        | additiveexpression SUB multiplicativeexpression {$$ = new Arithmetic(Oper::SUB, $1, $3) ;}
                        ;

relationalexpression    : additiveexpression
                        | relationalexpression GT additiveexpression {$$ = new Compare(Oper::GT, $1, $3);}
                        | relationalexpression LT additiveexpression {$$ = new Compare(Oper::LT, $1, $3);}
                        | relationalexpression LE additiveexpression {$$ = new Compare(Oper::LE, $1, $3);}
                        | relationalexpression GE additiveexpression {$$ = new Compare(Oper::GE, $1, $3);}
                        ;

equalityexpression      : relationalexpression
                        | equalityexpression EQ relationalexpression {$$ = new Compare(Oper::EQ, $1, $3);}
                        | equalityexpression NEQ relationalexpression {$$ = new Compare(Oper::NEQ, $1, $3);}
                        ;

conditionalandexpression: equalityexpression
                        | conditionalandexpression AND equalityexpression {$$ = new Logical(Oper::AND, $1, $3);}
                        ;

conditionalorexpression : conditionalandexpression
                        | conditionalorexpression OR conditionalandexpression {$$ = new Logical(Oper::OR, $1, $3);}
                        ;

assignmentexpression    : conditionalorexpression 
                        | assignment
                        ;

assignment              : identifier ASSIGN assignmentexpression {$$ = new AssnStmt($1->c_str()); $$->AddNode($3); }
                        ;

expression              : assignmentexpression
                        ;
   
%%

/* Parser will call this function when it fails to parse */
void JCC::Parser::error(const location_type &loc, const std::string &errmsg)
{
   std::cerr << "Error: " << errmsg << " at " << loc << "\n";
}
