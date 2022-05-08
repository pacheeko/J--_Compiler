Braydon Pacheco (30100449)
CPSC 411 Project

Compiler for j-- language. The scanner was built using the flex scanner generator tool. The parser was built using the bison parser tools.

Compile instructions:

Place all files into a directory and type 'make' to compile the program. Flex and Bison will create
many additional files at compile time.

Run instructions:

Type './main testfile' to run the parser, with testfile being the directory of the file you 
would like to parse. Several test files have been included. For example, execute the 
command './main test1.txt' with the test1.txt file in the same directory as the compiler to 
generate a .asm file in the same directory named 'test1.txt.asm'. Then run this asm file using spim
to get the output of the code given.

** The compiler has been compiled and tested on the CPSC linux machines. **
