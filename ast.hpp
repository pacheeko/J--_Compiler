#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#define INDENT_CHAR ' '
extern int INDENTS;

class AST;
    class Prog;

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
        class CondId;
        class Num;
        class Times;
        class Plus;
        //class Actuals;
        //class FuncCall;
        class Compare;

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

    std::string conditional;

    public:
    IfStmt() {};
    IfStmt(const char* const elseStmt) : conditional(elseStmt) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override
    {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--If Statement {}: " << "\n" ;
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

    std::string conditional;

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

    std::string identifier;
    std::string assignment;

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

    std::string identifier;
    std::string assignment;

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

    std::string identifier;
    std::string assignment;

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
        std::cout << "test" << std::endl;
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
    std::string type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    VarDecl(const char* const str, const char* const id) : name(std::string(id)), type(std::string(str)) {};

    void SetType(const char* const str) {
        type = str;
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
    std::string name = "name";
    std::string return_type = "rt";

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

    void SetRT(const char* const rt) {
        return_type = std::string(rt);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Declaration {'return type': " << return_type << ", 'id': " << name << "}" << "\n";
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
    std::string name = "name";
    std::string type = "type";

    Param* next;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    Param(const char* const t, const char* const id) : name(std::string(id)), type(std::string(t)) {};

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
        std::cout << "--Formal Parameter {'type': " << type << ", 'id': " << name << "}" << "\n";
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

class Id : public Exp {
  protected:
    std::string id;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Id(const char* const value) : id(std::string(value)) {}

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

class CondId : public Exp {
  protected:
    std::string id;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    CondId(const char* const value) : id(std::string(value)) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Conditional Id {'name': " << id << "}" << "\n";
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
    std::string type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

  public:
    Compare(const char* const value) : type(std::string(value)) {}

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
        std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Comparator {'type': " << type << " }" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Plus : public Exp {
  protected:
    Exp *left;
    Exp *right;

  public:
    Plus(Exp *L, Exp *R): left(L), right(R) {}
};

class Times : public Exp {
  protected:
    Exp *left;
    Exp *right;

  public:
    Times(Exp *L, Exp *R): left(L), right(R) {}
};

#endif