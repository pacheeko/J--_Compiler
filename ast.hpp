#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
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
        default: return "";
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
        default: return "";
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

    class Stmt;
        class Block;
        class IfStmt;
        class ElseStmt;
        class AssnStmt;
        class NullStmt;
        class RetStmt;
        class BreakStmt;
        class WhileStmt;

    class Exp;
        class Id;
        class Num;
        class Literal;
        class Arithmetic;
        class Compare;
        class Logical;
        class FuncCall;

    class Decl;
        class MainDecl;
        class FuncDecl;
        class VarDecl;
        class Param;

class AST
{
    protected:

    std::vector<AST*> children;

    virtual void AddChild(AST *child) = 0;

    public:

    int numChildren() {
        return children.size();
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

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Program: {'filename': " << prog_name << "}" << "\n";
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {   
            children[i]->Print();
        }
        INDENTS--;

    }
};

class Stmt : public AST{
    protected:
    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Stmt* next;

    public:

    bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    void setNext(Stmt *node) {
        next = node;
    }

    Stmt * getNext() {
        return next;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;

    }
};

class IfStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    IfStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--If Statement {" << "}: " << "\n" ;
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class ElseStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    ElseStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Else Statement {}: " << "\n";
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};

class WhileStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    std::string conditional;

    public:
    WhileStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--While Statement {}: " << "\n";
        INDENTS++;
        for (auto child :children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Block : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Block() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        if (children.empty()) {
            std::cout << "--Block: Empty" << "\n";
        } 
        else {
            std::cout << "--Block: " << "\n";
        }
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};

class AssnStmt : public Stmt {

    protected:

    std::string identifier;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    AssnStmt(const char* const id) : identifier(id) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Assign Statement {'Id': " << identifier << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class NullStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    NullStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Null Statement {}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class BreakStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    BreakStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Break Statement {}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class RetStmt : public Stmt {

    protected:

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    RetStmt() {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Return Statement {}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Decl : public AST{
    protected:
    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Decl * next;
    public:

    bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    void setNext(Decl *node) {
        next = node;
    }

    Decl * getNext() {
        return next;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;

    }
};

class MainDecl : public Decl
{
    protected:
    std::string name;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    MainDecl(const char* const str) : name(str) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
          std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Main Function Declaration: {'name': " << name << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class VarDecl : public Decl
{
    protected:
    std::string name;
    u_int8_t type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    VarDecl(u_int8_t t, const char* const id) : name(std::string(id)), type(t) {};

    void SetType(u_int8_t t) {
        type = t;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
          std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Variable Declaration: {'type': " << type << ", 'id': " << name << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class FuncDecl : public Decl
{
    protected:
    std::string name;
    u_int8_t return_type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    FuncDecl(const char* const id) : name(std::string(id)) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void SetRT(u_int8_t rt) {
        return_type = rt;
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Declaration {'return type': " << getReserved(return_type) << ", 'id': " << name << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Param : public Decl
{
    protected:
    std::string name;
    u_int8_t type;

    Param* next;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Param(u_int8_t t, const char* const id) : name(std::string(id)), type(t) {};

    bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    std::string GetName() {
        return name;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void SetNext(Param * n) {
        next = n;
    }

    Param* GetNext() {
        return next;
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Formal Parameter {'type': " << getReserved(type) << ", 'id': " << name << "}" << "\n";
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;
    }
};

class Exp : public AST {

    protected:
    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Exp * next;

    public:

    bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    void setNext(Exp *node) {
        next = node;
    }

    Exp * getNext() {
        return next;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << "--Exp {}" << std::endl;
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        INDENTS++;
        for (int i = children.size(); i --> 0;)
        {
            children[i]->Print();
        }
        INDENTS--;

    }
};

class Num : public Exp {
  protected:
    int value;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Num(int val) : value(val) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Num {'value': " << value << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Literal : public Exp {
  protected:
    u_int8_t value;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Literal(u_int8_t val) : value(val) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Literal {'value': " << getReserved(value) << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class String : public Exp {
  protected:
    std::string value;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    String(const char* const val) : value(val) {}
    
    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--String Literal {'value': " << value << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Id : public Exp {
  protected:
    std::string id;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Exp* next;

  public:
    Id(const char* const value) : id(std::string(value)) {}

    bool hasNext() {
        if (next != NULL) {
            return true;
        }
        else {
            return false;
        }
    }

    void setNext(Exp *node) {
        next = node;
    }

    Exp * getNext() {
        return next;
    }

    void AddNode(AST *node) override
    {
        AddChild(node);
    }



    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Id {'name': " << id << "}" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Compare : public Exp {
  protected:
    u_int8_t type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Exp * before;
    Exp * after;

  public:
    Compare(u_int8_t t, Exp* b, Exp* a) : type(t), before(b), after(a) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Comparison operator {'type': " << getOper(type) << " }" << "\n";
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

class Arithmetic : public Exp {
  protected:
    u_int8_t type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Exp * before;
    Exp * after;

  public:
    Arithmetic(u_int8_t t, Exp* a) : type(t), after(a) {}
    Arithmetic(u_int8_t t, Exp* b, Exp*a) : type(t), before(b), after(a) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Arithmetic operator {'type': " << getOper(type) << " }" << "\n";
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

class Logical : public Exp {
  protected:
    u_int8_t type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    Exp * before;
    Exp * after;

  public:
    Logical(u_int8_t t, Exp* a) : type(t), after(a) {}
    Logical(u_int8_t t, Exp* b, Exp*a) : type(t), before(b), after(a) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Logical operator {'type': " << getOper(type) << " }" << "\n";
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

class FuncCall : public Exp {
  protected:
    std::string id;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    FuncCall(const char* const value) : id(std::string(value)) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Invocation {'name': " << id << "}" << "\n";
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
