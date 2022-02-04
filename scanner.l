%option c++
%option yyclass="CCLexer"
%option yylineno
%option noyywrap

/* Declarations */
%{
    #include <iostream>
    #include <fstream>
    #include "scanner.hpp"
%}

/* Definitions here */
alpha       [a-zA-Z]
digit       [0-9]

num     {digit}{digit}*
ID      ({alpha}|_)({alpha}|{digit}|_)*

/* Comment States */
%x STRING
%x ONELINECOMMENT
%x MULTILINECOMMENT
%x BADSTRING

%% 

 /* Rules here */
[ \t\r]+    ;          
\n          ; 
"/*"(.|\n)*"*/"	{std::cout << "multiline comment" << std::endl;};
"//"(.)*[\n]?   {std::cout << "single line comment" << std::endl;};
\"              {yymore(); BEGIN(STRING);}  
<STRING>\"      {lexeme = std::string(YYText()); BEGIN(INITIAL); return T_STRING;}
<STRING>\n     {std::cerr << "ERROR: newline in string at line " << lineno() << std::endl; BEGIN(BADSTRING);}
<STRING><<EOF>> {std::cerr << "ERROR: string literal opened but never closed" << std::endl;}
<STRING>.       {yymore();}
<BADSTRING>\"   {BEGIN(INITIAL);}
<BADSTRING>.    ;
"true"      return T_TRUE;
"false"     return T_FALSE;
"boolean"   return T_BOOL;
"int"       return T_INT;
"void"      return T_VOID;
"if"        return T_IF;
"else"      return T_ELSE;
"while"     return T_WHILE;
"break"     return T_BREAK;
"return"    return T_RETURN;
"+"         return T_ADD;
"-"         return T_SUB;
"*"         return T_MULT;
"/"         return T_DIV;
"%"         return T_MOD;
"<"         return T_LT;
">"         return T_GT;
"<="        return T_LE;
">="        return T_GE;
"="         return T_ASSIGN;
"=="        return T_EQ;
"!="        return T_NEQ;
"!"         return T_NOT;
"&/&"       return T_AND;
"|/|"       return T_OR;
"("         return T_OPENPAR;
")"         return T_CLOSEPAR;
"{"         return T_OPENBRACE;
"}"         return T_CLOSEBRACE;
";"         return T_SEMICOLON;
","         return T_COMMA;
{ID}        {lexeme = std::string(YYText()); return T_ID;}
{num}       {lexeme = std::string(YYText()); return T_NUM;}
.         {std::cout << "WARNING: Ignoring bad character at or near line " << lineno() << std::endl;}
%% 

/* User routines here*/

/* int yyFlexLexer::yywrap() { return 1; } */

/* Creates and returns unique pointer to lexer */
std::unique_ptr<CCLexer> createLexer(std::istream* input) {
    return std::make_unique<CCLexer>(input);
}