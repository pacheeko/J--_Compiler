#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#define INDENT_CHAR ' '
extern int INDENTS;


enum Oper : uint8_t { ADD, SUB, DIV, MULT, MOD, LT, GT, LE, GE, EQ, NEQ, NOT, AND, OR};
enum Reserved : uint8_t {TRUE, FALSE, BOOL, INT, VOID, IF, ELSE, WHILE, BREAK, RETURN};

inline std::string getOper(uint8_t oper) {
    switch(oper) {
        case ADD: return "+";
        case SUB: return "-";
        case DIV: return "/";
        case MULT: return "*";
        case MOD: return "%";
        case LT: return "<";
        case GT: return "?";
        case LE: return "<=";
        case GE: return ">=";
        case EQ: return "==";
        case NEQ: return "!=";
        case NOT: return "!";
        case AND: return "&&";
        case OR: return "||";
        default: return "error";
    }
}

inline std::string getReserved(uint8_t word) {
    switch(word) {
        case TRUE: return "true";
        case FALSE: return "false";
        case BOOL: return "boolean";
        case INT: return "int";
        case VOID: return "void";
        case IF: return "if";
        case ELSE: return "else";
        case WHILE: return "while";
        case BREAK: return "break";
        case RETURN: return "return";
        default: return "error";
    }
}

/*
AST is an abstract class with a three tiered heirarchy of subclasses
Each class represents a node in the abstract syntax tree that is created when
the parser is run. Each class has a print function that is called recursively 
once the parser has finished.

***The skeleton of the class was taken from Shankar Ganesh tutorial ***
*/
class AST;
    class Prog; //Each AST will contain exactly one Prog node, which acts as the root of the tree

    class Block;
    class IfStmt;
    class ElseStmt;
    class AssnStmt;
    class NullStmt;
    class RetStmt;
    class BreakStmt;
    class WhileStmt;

    class Id;
    class Num;
    class Literal;
    class Arithmetic;
    class Compare;
    class Logical;
    class FuncCall;
    class String;

    class MainDecl;
    class FuncDecl;
    class VarDecl;
    class Param;

class AST
{
    protected:

    std::vector<AST*> children;
    
    virtual void AddChild(AST *child) = 0;
    u_int8_t type;
    std::string name;
    int lineno;
    AST * next;
    void * memoryLoc;

    public:

    virtual int numChildren() {
        return children.size();
    }
    virtual std::vector<AST*> getChildren() {
        return children;
    }

    virtual AST * getNext() {
        return next;
    }

    virtual bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    virtual void setNext(AST* ast) {
        next = ast;
    }

    virtual int getLineNo() {
        return lineno;
    }

    virtual std::string getType() {
        return "";
    }

    virtual std::string getNodeType() {
        return "";
    }

    virtual std::string getName() {
        return name;
    }

    virtual void setType(u_int8_t t) {
        type = t;
    }

    virtual void setLoc(void* loc) {
        memoryLoc = loc;
    }

    virtual void reverseChildren() {
        for (auto child : children) {   
            child->reverseChildren();
        }
    }

    AST() = default;

    virtual ~AST()
    {
        for (auto child : children)
        {
            delete child;
        }
        children.clear();
    }

    virtual void AddNode(AST *child) = 0;
    virtual void Print() = 0;
};

class Prog : public AST
{
    private:
    std::string prog_name;  

    protected:
    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Prog(const char* const str) : prog_name(str) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void reverseChildren() override {
        std::reverse(children.begin(), children.end());
        for (auto child : children) {   
            child->reverseChildren();
        }
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Program: {'filename': " << prog_name << "}" << "\n";
        INDENTS++;
        for (auto child : children) {   
            child->Print();
        }
        INDENTS--;

    }
};

class IfStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "if";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    IfStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--If Statement {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class ElseStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "else";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    ElseStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Else Statement {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};

class WhileStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "while";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    std::string conditional;

    public:
    WhileStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--While Statement {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child :children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Block : public AST {

    protected:
    int lineno;
    std::string nodeType = "block";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Block(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void reverseChildren() override {
        std::reverse(children.begin(), children.end());
        for (auto child : children) {   
            child->reverseChildren();
        }
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Block: {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class AssnStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "assnstmt";

    std::string identifier;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    AssnStmt(int line, const char* const id) : lineno(line), identifier(id) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Assign Statement {'Id': " << identifier << ", 'lineno': " << lineno << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class NullStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "null";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    NullStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Null Statement {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class BreakStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "break";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    BreakStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Break Statement {'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class RetStmt : public AST {

    protected:
    int lineno;
    std::string nodeType = "return";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    RetStmt(int line) : lineno(line) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Return Statement {'lineno': " << lineno << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class MainDecl : public AST
{
    protected:
    std::string name;
    std::string nodeType = "maindecl";
    u_int8_t type = 0;
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    MainDecl(int line, const char* const str) : lineno(line), name(str) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    std::string getName() override {
        return name;
    }

    std::string getType() override {
        return "";
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void reverseChildren() override {
        std::reverse(children.begin(), children.end());
        for (auto child : children) {   
            child->reverseChildren();
        }
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Main Function Declaration: {'name': " << name << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class VarDecl : public AST
{
    protected:
    std::string name;
    std::string nodeType = "vardecl";
    u_int8_t type;
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    VarDecl(int line, u_int8_t t, const char* const id) : lineno(line), name(std::string(id)), type(t) {};

    std::string getNodeType() override {
        return nodeType;
    }

    std::string getName() override {
        return name;
    }

    int getLineNo() override {
        return lineno;
    }

    std::string getType() override {
        return getReserved(type);
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Variable Declaration: {'type': " << getReserved(type) << ", 'id': " << name << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc  << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class FuncDecl : public AST
{
    protected:
    std::string name;
    std::string nodeType = "funcdecl";
    u_int8_t type;
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    FuncDecl(int line, const char* const id) : lineno(line), name(std::string(id)) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    std::string getName() override {
        return name;
    }

    std::string getType() override {
        return getReserved(type);
    }

    void AddNode(AST *node) override {
        AddChild(node);
    }

    void reverseChildren() override {
        std::reverse(children.begin(), children.end());
        for (auto child : children) {   
            child->reverseChildren();
        }
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Declaration {'return type': " << getReserved(type) << ", 'id': " << name  << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Param : public AST
{
    protected:
    std::string name;
    std::string nodeType = "param";
    u_int8_t type;
    int lineno;
    AST* next;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Param(int line, u_int8_t t, const char* const id) : lineno(line), name(std::string(id)), type(t) {};

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    std::string getName() override {
        return name;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void setNext(AST * n) override {
        next = n;
    }

    AST* GetNext() {
        return next;
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Formal Parameter {'type': " << getReserved(type) << ", 'id': " << name << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};

class Num : public AST {
  protected:
    int value, lineno;
    std::string nodeType = "num";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Num(int line, int val) : lineno(line), value(val) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Num {'value': " << value << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Literal : public AST {
  protected:
    u_int8_t value;
    int lineno;
    std::string nodeType = "literal";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Literal(int line, u_int8_t val) : lineno(line), value(val) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Literal {'value': " << getReserved(value)  << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class String : public AST {
  protected:
    std::string value, nodeType = "string";
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    String(int line, const char* const val) : lineno(line), value(val) {}
    
    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--String Literal {'value': " << value << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Id : public AST {
  protected:
    std::string id, nodeType = "id";
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    AST* next;

  public:
    Id(int line, const char* const value) : lineno(line), id(std::string(value)) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void setNext(AST *node) override {
        next = node;
    }

    AST * getNext() override {
        return next;
    }

    void AddNode(AST *node) override {
        AddChild(node);
    }



    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Id {'name': " << id << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Compare : public AST {
  protected:
    u_int8_t type;
    int lineno;
    std::string nodeType = "compare";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    AST * before;
    AST * after;

  public:
    Compare(int line, u_int8_t t, AST* b, AST* a) : lineno(line), type(t), before(b), after(a) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Comparison operator {'type': " << getOper(type) << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << " }" << "\n";
        INDENTS++;
        before->Print();
        after->Print();
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Arithmetic : public AST {
  protected:
    u_int8_t type;
    int lineno;
    std::string nodeType = "arithmetic";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    AST * before;
    AST * after;

  public:
    Arithmetic(int line, u_int8_t t, AST* a) : lineno(line), type(t), after(a) {}
    Arithmetic(int line, u_int8_t t, AST* b, AST*a) : lineno(line), type(t), before(b), after(a) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Arithmetic operator {'type': " << getOper(type) << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << " }" << "\n";
        INDENTS++;
        if (before != NULL) {
            before->Print();
        }
        after->Print();
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Logical : public AST {
  protected:
    u_int8_t type;
    int lineno;
    std::string nodeType = "logical";

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    AST * before;
    AST * after;

  public:
    Logical(int line, u_int8_t t, AST* a) : lineno(line), type(t), after(a) {}
    Logical(int line, u_int8_t t, AST* b, AST*a) : lineno(line), type(t), before(b), after(a) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Logical operator {'type': " << getOper(type) << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << " }" << "\n";
        INDENTS++;
        if (before != NULL) {
            before->Print();
        }
        after->Print();
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class FuncCall : public AST {
  protected:
    std::string id, nodeType = "funccall";
    int lineno;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    FuncCall(int line, const char* const value) : lineno(line), id(std::string(value)) {}

    std::string getNodeType() override {
        return nodeType;
    }

    int getLineNo() override {
        return lineno;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Invocation {'name': " << id << ", 'lineno': " << lineno << ", 'memLoc': " << memoryLoc << "}" << "\n";
        INDENTS++;
        std::cout << std::string(INDENTS*2, INDENT_CHAR) << "Arguments:\n";
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};
#endif
