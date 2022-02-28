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
    Param *param;
    Block *block;
    Id *id;
    CondId *condid;
};

%token <strVal> ADD "+"
%token <strVal> SUB "-"
%token <strVal> DIV "/"
%token <strVal> MULT "*"
%token <strVal> MOD "%"
%token <strVal> LT "<"
%token <strVal> GT ">"
%token <strVal> LE "<="
%token <strVal> GE ">="
%token <strVal> EQ "=="
%token <strVal> NEQ "!="
%token <strVal> NOT "!"
%token <strVal> AND "&&"
%token <strVal> OR "||"
%token <strVal> ASSIGN "="
%token <strVal> OPENPAR "("
%token <strVal> CLOSEPAR ")"
%token <strVal> OPENBRACE "{"
%token <strVal> CLOSEBRACE "}"
%token <strVal> SEMICOLON ";"
%token <strVal> COMMA ","
%token <strVal> TRUE "true"
%token <strVal> FALSE "false"
%token <strVal> BOOL "boolean"
%token <strVal> INT "int"
%token <strVal> VOID "void"
%token <strVal> IF "if"
%token <strVal> ELSE "else"
%token <strVal> WHILE "while"
%token <strVal> BREAK "break"
%token <strVal> RETURN "return"
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
%type <param> formalparameter
%type <decl> formalparameterlist
%type <stmt> block
%type <block> blockstatement
%type <block> blockstatements
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

/* Define the start symbol */
%start start

/* Left associativity */
%left "+" "-" "*" "/";

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

literal         : NUM {std::cout << "Num: " << $1 << std::endl; $$ = new Num($1);}
                | STRING 
                | TRUE 
                | FALSE 
                ;

type            : BOOL  
                | INT   
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

functiondeclaration     : functionheader block {$$ = $1; $$->AddNode($2);}
                        ;

functionheader          : type functiondeclarator {$$ = $2; $$->SetRT($1->c_str());}
                        | VOID functiondeclarator {$$ = $2; $$->SetRT($1->c_str());}
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

formalparameterlist     : formalparameter   {$$ = $1; }
                        | formalparameterlist COMMA formalparameter {$$ = $3; $$->setNext($1);}
                        ;

formalparameter         : type identifier {$$ = new Param($1->c_str(), $2->c_str());}
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

blockstatements         : blockstatement {$$ = $1; }    
                        | blockstatements blockstatement {$$ = $2; $$->setNext($1);}
                        ;

blockstatement          : variabledeclaration 
                        | statement 
                        ;

statement               : block {$$ = $1;}
                        | SEMICOLON {$$ = new NullStmt();}
                        | statementexpression SEMICOLON
                        | BREAK SEMICOLON {$$ = new BreakStmt();}
                        | RETURN expression SEMICOLON {$$ = new RetStmt(); $$->AddNode($2);}
                        | RETURN SEMICOLON {$$ = new RetStmt();}
                        | IF OPENPAR expression CLOSEPAR statement {$$ = new IfStmt(); $$->AddNode($3); $$->AddNode($5);}
                        | IF OPENPAR expression CLOSEPAR statement ELSE statement {$$ = new IfStmt(); $$->AddNode($3); $$->AddNode($5); ElseStmt* es = new ElseStmt(); es->AddNode($7);}
                        | WHILE OPENPAR expression CLOSEPAR statement {$$ = new WhileStmt(); $$->AddNode($3); $$->AddNode($5);}
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
                        | identifier {$$ = new CondId($1->c_str());}
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
                        | additiveexpression ADD multiplicativeexpression {}
                        | additiveexpression SUB multiplicativeexpression
                        ;

relationalexpression    : additiveexpression
                        | relationalexpression GT additiveexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
                        | relationalexpression LT additiveexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
                        | relationalexpression LE additiveexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
                        | relationalexpression GE additiveexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
                        ;

equalityexpression      : relationalexpression
                        | equalityexpression EQ relationalexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
                        | equalityexpression NEQ relationalexpression {$$ = new Compare($2->c_str()), $$->AddNode($1), $$->AddNode($3);}
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

assignment              : identifier ASSIGN assignmentexpression {$$ = new AssnStmt($1->c_str()); $$->AddNode($3); }
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