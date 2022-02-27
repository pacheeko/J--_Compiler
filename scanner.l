%option c++
%option yyclass="JCC::Lexer"
%option noyywrap

/* Declarations */
%{
    #include <iostream>
    #include <fstream>
    #include "scanner.hpp"
    #include "parser.hh"
    #include "vector"
    
    // The token this time is in the parser class
    // If no namespace and class is used, then the default is yy::Parser::token
    using Token = JCC::Parser::token;

    // Should have this if you're using C++
    // Defines yylex with parameters. Flex will produce this function.
    // semantic_type is YYSTYPE, the union defined in bison. 
    #undef  YY_DECL
    #define YY_DECL int JCC::Lexer::yylex(JCC::Parser::semantic_type *yylval, JCC::Parser::location_type *loc)

    // Everytime yylex is called, the location is updated for a token
    // loc->step() sets the begin character for the token. 
    // And loc->columns sets the number of characters for the token
    #define YY_USER_ACTION loc->step(); loc->columns(yyleng);

    double num;
    int strLength;
    /*Warnings are counted on each line, if there are more than 10
      warnings on a single line, the program exits with an error.
      The number of warnings resets when a new line is found. */
    int warnings = 0;
    int linewarnings = 0;
    int checkWarnings();
%}

/* Definitions here */
alpha       [a-zA-Z]
digit       [0-9]

num     {digit}{digit}*
ID      ({alpha}|_)({alpha}|{digit}|_)*

/* Comment States */
%x STRING
%x BADSTRING
%x COMMENT

%% 

 /* Rules here */
[ \t\r]+    ;          
\n          {linewarnings = 0;} 
"//"        {BEGIN(COMMENT);}
<COMMENT>\n   {BEGIN(INITIAL);}
<COMMENT><<EOF>> {return 0;}
<COMMENT>.    ;
\"              {BEGIN(STRING);}  
<STRING>(\\\b|\\\f|\\\t|\\\r|\\\n|\\\'|\\\"|\\\\|\\\0)    {yymore();}
<STRING>\"      {BEGIN(INITIAL); std::cout << "string here" << std::endl; return Token::T_STRING;}
<STRING>\n      {std::cerr << "ERROR: newline in string at line " << lineno() << std::endl; warnings++; linewarnings++; BEGIN(BADSTRING);}
<STRING><<EOF>> {std::cerr << "ERROR: string literal opened but never closed at line " << lineno() << std::endl; warnings++; linewarnings++; return 0;}
<STRING>.       {}
<BADSTRING>\"   {BEGIN(INITIAL);}
<BADSTRING><<EOF>>  {return 0;}
<BADSTRING>.    ;
"true"      return Token::T_TRUE;
"false"     return Token::T_FALSE;
"boolean"   {yylval->strVal = new std::string("bool"); return Token::T_BOOL;}
"int"       {yylval->strVal = new std::string("int"); return Token::T_INT;}
"void"      return Token::T_VOID;
"if"        return Token::T_IF;
"else"      return Token::T_ELSE;
"while"     return Token::T_WHILE;
"break"     return Token::T_BREAK;
"return"    return Token::T_RETURN;
"+"         return Token::T_ADD;
"-"         return Token::T_SUB;
"*"         return Token::T_MULT;
"/"         return Token::T_DIV;
"%"         return Token::T_MOD;
"<"         return Token::T_LT;
">"         return Token::T_GT;
"<="        return Token::T_LE;
">="        return Token::T_GE;
"="         return Token::T_ASSIGN;
"=="        return Token::T_EQ;
"!="        return Token::T_NEQ;
"!"         return Token::T_NOT;
"&&"        return Token::T_AND;
"||"        return Token::T_OR;
"("         return Token::T_OPENPAR;
")"         return Token::T_CLOSEPAR;
"{"         return Token::T_OPENBRACE;
"}"         return Token::T_CLOSEBRACE;
";"         return Token::T_SEMICOLON;
","         return Token::T_COMMA;
{ID}        {yylval->strVal = new std::string(yytext);  return Token::T_ID;}
{num}       {yylval->ival = std::stoi(yytext);          return Token::T_NUM;}
.           {std::cerr << "WARNING: bad character around line " << lineno() << std::endl; 
             warnings++;
             linewarnings++;
             int error = checkWarnings();
             if (error == -1) {
                 std::cerr << "ERROR: Too many warnings near line " << lineno() << ". Exiting." << std::endl;
                 exit(EXIT_FAILURE);}
             else if (checkWarnings() == -2) {
                 std::cerr << "ERROR: Too many overall warnings. Exiting." << std::endl;
                 exit(EXIT_FAILURE);
             }
            }
%% 

/* User routines here*/
std::unique_ptr<JCC::Lexer> createLexer(std::istream* input) {
    return std::make_unique<JCC::Lexer>(input);
}
/* int yyFlexLexer::yywrap() { return 1; } */

/* Checks to see if the number of warnings in a line is > 10 */
int checkWarnings() {
    if (linewarnings > 10) {
        return -1;
    }
    else if (warnings > 20) {
        return -2;
    }
    else {
        return 0;
    }
}