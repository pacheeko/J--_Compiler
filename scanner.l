/* Taken from Shankar Ganesh to be used as starter code */

%option c++
%option yyclass="CCLexer"
%option yylineno
%option noyywrap

/* Declarations */
%{
    #include <iostream>
    #include <fstream>
    #include "scanner.hpp"
    #include <string>
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
\"              {lexeme.append(YYText()); BEGIN(STRING);}  
<STRING>(\\\b|\\\f|\\\t|\\\r|\\\n|\\\'|\\\"|\\\\|\\\0)    {yymore();}
<STRING>\"      {lexeme.append(YYText()); BEGIN(INITIAL); return T_STRING;}
<STRING>\n      {std::cerr << "ERROR: newline in string at line " << lineno() << std::endl; warnings++; linewarnings++; BEGIN(BADSTRING);}
<STRING><<EOF>> {std::cerr << "ERROR: string literal opened but never closed at line " << lineno() << std::endl; warnings++; linewarnings++; return 0;}
<STRING>.       {lexeme.append(YYText());}
<BADSTRING>\"   {BEGIN(INITIAL);}
<BADSTRING><<EOF>>  {return 0;}
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
"&&"        return T_AND;
"||"        return T_OR;
"("         return T_OPENPAR;
")"         return T_CLOSEPAR;
"{"         return T_OPENBRACE;
"}"         return T_CLOSEBRACE;
";"         return T_SEMICOLON;
","         return T_COMMA;
{ID}        {lexeme = std::string(YYText()); return T_ID;}
{num}       {lexeme = std::string(YYText()); 
             return T_NUM;}
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

/* int yyFlexLexer::yywrap() { return 1; } */

/* Creates and returns unique pointer to lexer */
std::unique_ptr<CCLexer> createLexer(std::istream* input) {
    return std::make_unique<CCLexer>(input);
}
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