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

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <fstream>
#include "vector"
#include <unordered_map>
#include <stack> 
#include <memory>
using namespace std;
#include "ast.hpp"

//Data structures
struct entry {
    int scope;
    int paramNum;
    string type;
    unordered_map<string,entry>* symTable;
    string nodeType;
    string attr;
};
static unordered_map<string, entry> symTables[30];
static vector<unordered_map<string,entry>*> scopeStack;
static int whileLoops = 0, numOfBlocks = 0, scope = 0, errors = 0, symIt = 0;
static bool before = true;


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
inline string getIdType(string name);
inline string typeCheck(AST* node);
inline bool checkForReturn(AST* node, string returnType);

inline AST* semanticAnalyzer(AST* root) {
    unordered_map<string, entry> preDefined;
    symTables[symIt] = preDefined;
    unordered_map<string, entry>* prePtr = &symTables[symIt];
    symIt++;
    scopeStack.push_back(prePtr);
    addPreDefined();
    unordered_map<string, entry> globalStack;
    symTables[symIt] = globalStack;
    unordered_map<string, entry>* globalPtr = &symTables[symIt];
    symIt++;
    scopeStack.push_back(globalPtr);

    //First pass, checks for semantic checks 1 and 2
    scope = 1;
    postOrderTrav(root, &firstPass);
    checkForMain(*scopeStack.at(1));
    scope = 1;
    //Second pass, checks for semantic checks 3,13,14
    prePostTrav(root, &secondPass);
    prePostTrav(root, &thirdPass);
    prePostTrav(root, &fourthPass);
    return root;
}

// Calls the given function only after calling it on all of the nodes children
inline AST* postOrderTrav(AST* root, AST* (*func)(AST *)) {
    bool scopeChange = false;
    if ((root->getNodeType() == "maindecl") || (root->getNodeType() == "funcDecl")) {
        scope++;
        scopeChange = true;
        }
    for (AST* node : root->getChildren()) {
        postOrderTrav(node, func);
    }
    if (scopeChange) {
        scope--;
    }
    func(root);
    return root;
};

// Calls the given function only before calling it on all of the nodes children
inline AST* preOrderTrav(AST* root, AST* (*func)(AST *)) {
    func(root);
    bool scopeChange = false;
    if ((root->getNodeType() == "maindecl") || (root->getNodeType() == "funcDecl")) {
        scope++;
        scopeChange = true;
        }
    for (AST* node : root->getChildren()) {
        preOrderTrav(node, func);
    }
    if (scopeChange) {
        scope--;
    }
    return root;
};

// Calls the given function before and after calling it on all of the nodes children
inline AST* prePostTrav(AST* root, AST* (*func)(AST *)) {
    before = true;
    func(root);
    bool scopeChange = false;
    for (AST* node : root->getChildren()) {
        prePostTrav(node, func);
    }
    before = false;
    func(root);
    return root;
};

inline AST* firstPass(AST* node) {
    // Semantic check 13 for global scope. Also sets up symbol tables for function declarations and global variables
    if (scope == 1) {
        if (node->getNodeType() == "vardecl") {
            entry newEntry = {.scope = 1, .type = node->getType(), .nodeType = node->getNodeType()};
            if (!scopeStack.at(1)->insert({node->getName(), newEntry}).second) {
                cerr << "Error: A global variable was re-declared near line: " << node->getLineNo() << "." << endl;
                errors++;
            };
            auto loc = scopeStack.at(1)->at(node->getName());
            node->setLoc(&loc);
        }
        else if (node->getNodeType() == "maindecl" || node->getNodeType() == "funcdecl") {
            unordered_map<string, entry> funcTable;
            symTables[symIt] = funcTable;
            entry newEntry = {.scope = 1, .type = node->getType(), .symTable = &symTables[symIt], .nodeType = node->getNodeType()};
            if (!scopeStack.at(1)->insert({node->getName(), newEntry}).second) {
                cerr << "Error: A function was re-declared near line: " << node->getLineNo() << "." << endl;
                errors++;
            };
            auto loc = scopeStack.at(1)->at(node->getName());
            node->setLoc(&loc);
            symIt++;
        }
    }
        return node;
};

inline AST* secondPass(AST* node) {
    // Semantic checks 3, 13, 14: check if node has already been identified in the same scope
    if (node->getNodeType() == "block") {
        if (before) {
            numOfBlocks++;
        }
        else {
            numOfBlocks--;
        }
    }

    else if (before && (node->getNodeType() == "vardecl")) {
        if (scope > 1) {
            // Semantic check 3: A local declaration was not in an outermost block.
            if (numOfBlocks > 1) {
                cerr << "Error: A local declaration was not in an outermost block near line: " << node->getLineNo() << "." << endl;
                errors++;
            }
            // Semantic check 13: identifier is redefined within the same scope.
            entry newEntry = {.scope = scope, .type = node->getType(), .nodeType = node->getNodeType()};
            if (!scopeStack.back()->insert({node->getName(), newEntry}).second) {
                    cerr << "Error: Variable redeclaration in same scope around line: " << node->getLineNo() << "." << endl;
                    errors++;
            }
            auto loc = scopeStack.at(scope)->at(node->getName());
            node->setLoc(&loc);
        }
    }
    // Update scope stack for the current function declaration
    else if (node->getNodeType() == "maindecl" || node->getNodeType() == "funcdecl") {
        if (before) {
            unordered_map<string, entry>* ptr = scopeStack.at(1)->at(node->getName()).symTable;
            scopeStack.push_back(ptr);
            scope++;
        }
        else {
            scope--;
            scopeStack.pop_back();
        }
    }
    // Sematic check 14: An undeclared identifier is used.
    else if (before && ((node->getNodeType() == "funccall") || (node->getNodeType() == "id"))) { 
        int size = scopeStack.size() - 1;
        bool exists = false;
        for (int i = size; i >= 0; i--) {
            if (scopeStack.at(i)->find(node->getName()) != scopeStack.at(i)->end()) {
                i = -1;
                exists = true;
            }    
        }
        if (!exists) {
            if (node->getNodeType() == "funccall") {
                cerr << "Error: Function called that was never declared around line: " << node->getLineNo() << "." << endl;
                errors++;
            }
            else if (node->getNodeType() == "id") {
                cerr << "Error: Identifier used that was never declared around line: " << node->getLineNo() << "." << endl;
                errors++;
            }
        }
        
    }
    // Add Params to local function symbol table if inside function
    else if (before && (node->getNodeType() == "param")) {
        entry newEntry = {.scope = scope, .paramNum = node->getParamNum(), .type = node->getType(), .nodeType = node->getNodeType()};
        if (!scopeStack.at(scope)->insert({node->getName(), newEntry}).second) {
            cerr << "Error: Param reused in same function near line: " << node->getLineNo() << "." << endl;
            errors++;
        }
        auto loc = scopeStack.at(scope)->at(node->getName());
        node->setLoc(&loc);
    }

    return node;
};

inline AST* thirdPass(AST* node) {
    /*
    Semantic checks:
    4. The number/type of arguments in a function call doesn't match the function's declaration.
    5. The main function can't be called.
    7. Type mismatch for an operator (||, &&, ==, !=, =, <, >, <=, >=, +, - (unary and binary), *, /, %, !).
    8. No return statement in a non-void function.
    9. A void function can't return a value.
    10. A non-void function must return a value. Note that you're only checking for the existence of an appropriate return statement at the semantic checking stage, not whether it's actually executed.
    11. A value returned from a function has the wrong type.
    12. An if- or while-condition must be of Boolean type.
    */

    // Semantic checks 4 and 5
    if (before && (node->getNodeType() == "funccall")) {
        entry funcDecl;
        int size = scopeStack.size();
        bool exists = false;
        for (int i = 0; i < size; i++) {
            auto it = scopeStack.at(i)->find(node->getName());
            if (it != scopeStack.at(i)->end()) {
                funcDecl = scopeStack.at(i)->at(node->getName());
                exists = true;
            }
        }
        if (!exists) {

        }
        //Semantic check 5
        else if (funcDecl.nodeType == "maindecl") {
            cerr << "Error: Main function called near line: " << node->getLineNo() << "." << endl;
            errors++;
        }
        //Semantic check 4
        else {
            int funcCallParams = node->getChildren().size();
            int funcDeclParams = 0;
            unordered_map<string, entry> * funcTable = funcDecl.symTable;
            for (auto& it : *funcTable) {
                if (it.second.nodeType == "param") {
                    funcDeclParams++;
                }
            }
            if (funcCallParams != funcDeclParams) {
                cerr << "Error: Function invocation near line " << node->getLineNo() << " uses " << funcCallParams << " argument(s) when it should use " << funcDeclParams << " argument(s)." << endl;
                errors++;
            }

            for (int parNum = 1; parNum <= funcCallParams; parNum++) {
                string nodeType = node->getChildren().at(parNum-1)->getNodeType();
                string type;
                if (nodeType == "string") {
                    type = "string";
                }
                else {
                    type = getIdType(node->getChildren().at(parNum-1)->getName());
                }
                if (type != "") {
                    for (auto& it : *funcTable) {
                        if (it.second.paramNum == parNum) {
                            if (!(type == it.second.type)) {
                                cerr << "Error: Wrong type used in function call near line: " << node->getLineNo() << ". ";
                                cerr << type << " used instead of " << it.second.type << "." << endl;
                                errors++;
                            }
                        }
                    }
                }
            }
        }
    }

    /* Update scope stack for the current function declaration
    Semantic checks:
    8. No return statement in a non-void function.
    9. A void function can't return a value.
    10. A non-void function must return a value. Note that you're only checking for the existence of an appropriate return statement at the semantic checking stage, not whether it's actually executed.
    11. A value returned from a function has the wrong type.
    */
    else if (node->getNodeType() == "maindecl" || node->getNodeType() == "funcdecl") {
        if (before) {
            unordered_map<string, entry>* ptr = scopeStack.at(1)->at(node->getName()).symTable;
            scopeStack.push_back(ptr);
            scope++;

            string returnType = node->getType();
            bool retStmt = false;
            for (auto child : node->getChildren()) {
                if (child->getNodeType() == "block") {
                    for (auto blockChild : child->getChildren()) {
                        if (retStmt == false) {
                            retStmt = checkForReturn(blockChild, returnType);
                        }
                    }
                }
            }
            if (retStmt == false) {
                //8. No return statement in a non-void function.
                if ((returnType == "int") || (returnType == "boolean")) {
                    cerr << "Error: Non-void function of type " << returnType << " does not return a value near line: " << node->getLineNo() << ". " << endl;
                    errors++;                    
                }
            }

        }
        else {
            scope--;
            scopeStack.pop_back();
        }
    }
    //12. An if- or while-condition must be of Boolean type.
    else if (before && (node->getNodeType() == "if" || node->getNodeType() == "while")) {
        AST* child = node->getChildren().at(0);
        string type = typeCheck(child);
        if (type != "boolean") {
            if (node->getNodeType() == "if") {
                cerr << "Error: If condition statement not of boolean type near line: " << node->getLineNo() << ". " << endl;
                errors++;
            }
            else {
                cerr << "Error: While condition statement not of boolean type near line: " << node->getLineNo() << ". " << endl;
                errors++;
            }
        }
    }

    else if (before && (node->getNodeType() == "assnstmt")) {
        string varType = getIdType(node->getName());
        string assnType = typeCheck(node->getChildren().at(1));
        if (varType != assnType) {
                cerr << "Error: Variable type " << varType << " does not match assignment type " << assnType << " near line: " << node->getLineNo() << ". " << endl;
                errors++;            
        }
    }
    return node;
};

inline AST* fourthPass(AST* node) {
    //6. Break statements must be inside a while statement.
    if (before) {
        if (node->getNodeType() == "while") {
            whileLoops ++;
        }
        else if (node->getNodeType() == "break") {
            if (whileLoops < 1) {
                cerr << "Error: Break statemenout outside while statement near line: " << node->getLineNo() << ". " << endl;
                errors++;  
            }
        }
    }
    else {
        if (node->getNodeType() == "while") {
            whileLoops --;
        }
    }

    return node;
};

// Adds the j-- library functions to the symbol table at scope stack 0 so
// that these functions are always in scope
inline void addPreDefined() {
    // Entry for getChar function
    entry getChar = {.scope = 0, .type = "int"};
    symTables[0].insert({"getchar", getChar});

    // Entry for halt function
    entry halt = {.scope = 0, .type = "void"};
    symTables[0].insert({"halt", halt});

    // Entry for printb function
    unordered_map<string, entry> printbTable;
    entry printbParam = {.scope = 1, .paramNum = 1, .type = "boolean"};
    printbTable.insert({"b", printbParam});
    symTables[symIt] = printbTable;
    entry printb = {.scope = 0, .type = "void", .symTable = &symTables[symIt]};
    symTables[0].insert({"printb", printb});
    symIt++;

    // Entry for printc function
    unordered_map<string, entry> printcTable;
    entry printcParam = {.scope = 1, .paramNum = 1, .type = "int"};
    printcTable.insert({"c", printcParam});
    symTables[symIt] = printcTable;
    entry printc = {.scope = 0, .type = "void", .symTable = &symTables[symIt]};
    symTables[0].insert({"printc", printc});
    symIt++;

    // Entry for printi function
    unordered_map<string, entry> printiTable;
    entry printiParam = {.scope = 1, .paramNum = 1, .type = "int"};
    printiTable.insert({"i", printcParam});
    symTables[symIt] = printiTable;
    entry printi = {.scope = 0, .type = "void", .symTable = &symTables[symIt] };
    symTables[0].insert({"printi", printi});
    symIt++;

    // Entry for prints function
    unordered_map<string, entry> printsTable;
    entry printsParam = {.scope = 1, .paramNum = 1, .type = "string"};
    printsTable.insert({"s", printsParam});
    symTables[symIt] = printsTable;
    entry prints = {.scope = 0, .type = "void", .symTable = &symTables[symIt] };
    symTables[0].insert({"prints", prints});
    symIt++;
};

// Checks the global scopeStack for main functions
inline void checkForMain(unordered_map<string, entry> global) {
    int mains = 0;

    for (auto& ent : global) {
        if (ent.second.nodeType == "maindecl") {
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

// Takes as input the name of an identifier, searches for it in the 
// scope stack, and returns its type as a string
inline string getIdType(string name) {
    int size = scopeStack.size() - 1;
    for (int i = size; i >= 0; i--) {
        auto it = scopeStack.at(i)->find(name);
        if (it != scopeStack.at(i)->end()) {
            return it->second.type;
        }    
    }
    return "";
}

//Returns the type of given node, and recursively checks the left and right side
//of operations and returns error messages if their types are wrong.
inline string typeCheck(AST* node) {
    string nodeType = node->getNodeType();
    if (nodeType == "literal") {
        return "boolean";
    }
    else if (nodeType == "string") {
        return "string";
    }
    else if (nodeType == "num") {
        return "int";
    }
    else if ((nodeType == "id") || nodeType == "funccall") {
        return getIdType(node->getName());
    }
    else if (nodeType == "funccall") {
        return node->getType();
    }
    else if (nodeType == "compare") {
        string left = typeCheck(node->getChildren().at(0));
        string right = typeCheck(node->getChildren().at(1));
        if (!(node->getType() == "==") && !(node->getType() == "!=")) {
            if (left != "int") {
                cerr << "Error: Bad type used in compare operation near line: " << node->getLineNo() << ". " << "Type " << left << " used instead of int." << endl;
                errors++;            
            }
            if (right != "int") {
                cerr << "Error: Bad type used in compare operation near line: " << node->getLineNo() << ". " << "Type " << right << " used instead of int." << endl;
                errors++;    
            }
        }
        else {
            if (left != right) {
                cerr << "Error: Trying to compare type " << left << " to type " << right << " near line " << node->getLineNo() << "." << endl;
                errors++; 
            }
        }
        return "boolean";
    }
    else if (nodeType == "logical") {
        string left = typeCheck(node->getChildren().at(0));
        if (left != "boolean") {
            cerr << "Error: Bad type used in logical operation near line: " << node->getLineNo() << ". " << "Type " << left << " used instead of boolean." << endl;
            errors++;            
        }
        if (node->getChildren().size() > 1) {
            string right = typeCheck(node->getChildren().at(1));
            if (right != "boolean") {
                cerr << "Error: Bad type used in logical operation near line: " << node->getLineNo() << ". " << "Type " << right << " used instead of boolean." << endl;
                errors++;    
            }
        }
        return "boolean";
    }
    else if (nodeType == "arithmetic") {
        string left = typeCheck(node->getChildren().at(0));
        if (left != "int") {
            cerr << "Error: Bad type used in arithmetic operation near line: " << node->getLineNo() << ". " << "Type " << left << " used instead of int." << endl;
            errors++;            
        }
        if (node->getChildren().size() > 1) {        
            string right = typeCheck(node->getChildren().at(1));
            if (right != "int") {
                cerr << "Error: Bad type used in arithmetic operation near line: " << node->getLineNo() << ". " << "Type " << right << " used instead of int." << endl;
                errors++;    
            }
        }
        return "int";
    }
    return "";
}

// Recursively checks for return statements inside a function, returns
// true if it finds a return statement.
inline bool checkForReturn(AST* node, string returnType) {
    bool retStmt = false;
    if (node->getNodeType() == "return") {
        retStmt = true;
        if (node->getChildren().empty()) {
            //10. A non-void function must return a value.
            if (returnType == "int" || returnType == "boolean") {
                cerr << "Error: Non-void function of type " << returnType << " does not return a value near line: " << node->getLineNo() << ". " << endl;
                errors++;
            }
        }
        else {
            string retValue = node->getChildren().at(0)->getType();
            if (retValue == "") {
                retValue = getIdType(node->getChildren().at(0)->getName());
                if (retValue == "") {
                    cerr << "Error: Function returns undeclared variable " << node->getChildren().at(0)->getName() << " near line: " << node->getLineNo() << ". " << endl;
                    errors++;
                }
            }

            //9. A void function can't return a value.
            if (returnType == "void") {
                cerr << "Error: Void function returns non-void type " << retValue << " near line: " << node->getLineNo() << ". " << endl;
                errors++;
            }
            //9. A void function can't return a value.
            else if (returnType == "") {
                cerr << "Error: Main function returns non-void type " << retValue << " near line: " << node->getLineNo() << ". " << endl;
                errors++;
            }
            //11. A value returned from a function has the wrong type.
            else {
                if (!(returnType == retValue)) {
                cerr << "Error: Non-void function returns type " << retValue << " when it should return type " << returnType << " near line: " << node->getLineNo() << ". " << endl;
                errors++;
                }
            }
        }
    }
    bool rt;
    for (auto child : node->getChildren()) {
        rt = checkForReturn(child, returnType);
        if (retStmt == false) {
            retStmt = rt;
        }
    }
    return retStmt;
}