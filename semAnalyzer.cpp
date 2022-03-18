/*
Semantic Analyzer. Uses the AST given from the parser output and output
a new AST with semantic information. Keeps track of errors and prints error
messages as they occur. Checks for errors based on the following semantic information:

1. No main declaration found.
2. Multiple main declarations found.
3. A local declaration was not in an outermost block.
4. The number/type of arguments in a function call doesn't match the function's declaration.
5. The main function can't be called.
6. Break statements must be inside a while statement.
7. Type mismatch for an operator (||, &&, ==, !=, =, <, >, <=, >=, +, - (unary and binary), *, /, %, !).
8. No return statement in a non-void function.
9. A void function can't return a value.
10. A non-void function must return a value. Note that you're only checking for the existence of an appropriate return statement at the semantic checking stage, not whether it's actually executed.
11. A value returned from a function has the wrong type.
12. An if- or while-condition must be of Boolean type.
13. An identifier is redefined within the same scope.
14. An undeclared identifier is used.

*/

// TODO: 4 different pass functions, AST traverser 

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>
#include "vector"
#include <unordered_map>
using namespace std;
#include "ast.hpp"

//Data structures
struct entry {
    int scope;
    string type;
    void* attr;
};
static int errors = 0;
static int scope = 0;
static vector<unordered_map<string, entry>> symTables;
static vector<unordered_map<string,entry>*> scopeStack;

//Functions
inline AST* semanticAnalyzer(AST* root);
inline AST* postOrderTrav(AST* root, AST* (*func)(AST *));
inline AST* preOrderTrav(AST* root, AST* (*func)(AST *));
inline AST* prePostTrav(AST* root, AST* (*func)(AST *));
inline AST* firstPass(AST* node); 
inline AST* secondPass(AST* node); 
inline AST* thirdPass(AST* node); 
inline AST* fourthPass(AST* node); 
inline void addPreDefined();
inline void checkForMain(unordered_map<string, entry> global);

inline AST* semanticAnalyzer(AST* root) {
    unordered_map<string, entry> preDefined;
    symTables.push_back(preDefined);
    scopeStack.push_back(&symTables.at(0));
    unordered_map<string, entry> globalStack;
    symTables.push_back(globalStack);
    scopeStack.push_back(&symTables.at(1));
    addPreDefined();
    //First pass, checks for semantic checks 1 and 2
    postOrderTrav(root, &firstPass);
    checkForMain(*scopeStack.at(1));
    scope = 0;
    //Second pass, checks for semantic checks 3,13,14
    prePostTrav(root, &secondPass);
    return root;
}

inline AST* postOrderTrav(AST* root, AST* (*func)(AST *)) {
    scope++;
    for (AST* node : root->getChildren()) {
        postOrderTrav(node, func);
    }
    scope--;
    func(root);
    return root;
};

inline AST* preOrderTrav(AST* root, AST* (*func)(AST *)) {
    func(root);
    for (AST* node : root->getChildren()) {
        preOrderTrav(node, func);
    }
    return root;
};

inline AST* prePostTrav(AST* root, AST* (*func)(AST *)) {
    func(root);
    for (AST* node : root->getChildren()) {
        preOrderTrav(node, func);
    }
    func(root);
    return root;
};

inline AST* firstPass(AST* node) {
    if (scope == 1) {
        entry newEntry = {.scope = 1, .type = node->getType()};
        scopeStack.at(1)->insert({node->getName(), newEntry});
        auto loc = scopeStack.at(1)->at(node->getName());
        node->setLoc(&loc);
    }
        return node;
};

inline AST* secondPass(AST* node) {
    //
    return node;
};

inline AST* thirdPass(AST* node) {
    return node;
};

inline AST* fourthPass(AST* node) {
    return node;
};

inline void addPreDefined() {
    // Entry for getChar function
    entry getChar = {.scope = 0, .type = "int"};
    symTables.at(0).insert({"getchar", getChar});

    // Entry for halt function
    entry halt = {.scope = 0, .type = "void"};
    symTables.at(0).insert({"halt", halt});

    // Entry for printb function
    unordered_map<string, entry> printbTable;
    entry printbParam = {.scope = 1, .type = "boolean"};
    printbTable.insert({"b", printbParam});
    symTables.push_back(printbTable);
    entry printb = {.scope = 0, .type = "void", .attr = &symTables.back()};
    symTables.at(0).insert({"printb", printb});

    // Entry for printc function
    unordered_map<string, entry> printcTable;
    entry printcParam = {.scope = 1, .type = "int"};
    printcTable.insert({"c", printcParam});
    symTables.push_back(printcTable);
    entry printc = {.scope = 0, .type = "void", .attr = &printcTable };
    symTables.at(0).insert({"printc", printc});

    // Entry for printi function
    unordered_map<string, entry> printiTable;
    entry printiParam = {.scope = 1, .type = "int"};
    printiTable.insert({"i", printcParam});
    symTables.push_back(printiTable);
    entry printi = {.scope = 0, .type = "void", .attr = &printiTable };
    symTables.at(0).insert({"printi", printi});
    // Entry for prints function
    unordered_map<string, entry> printsTable;
    entry printsParam = {.scope = 1, .type = "string"};
    printsTable.insert({"s", printsParam});
    symTables.push_back(printsTable);
    entry prints = {.scope = 0, .type = "void", .attr = &printsTable };
    symTables.at(0).insert({"prints", prints});
};

inline void checkForMain(unordered_map<string, entry> global) {
    int mains = 0;

    for (auto& ent : global) {
        if (ent.second.type.compare("") == 0) {
            mains++;
        }
    }
    if (mains == 0) {
        cerr << "Error: No main function found." << endl;
        errors++;
    }
    else if (mains > 1) {
        cerr << "Error: Multiple main functions found." << endl;
        errors++;
    }
}