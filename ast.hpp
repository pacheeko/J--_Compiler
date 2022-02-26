#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#define INDENT_CHAR ' '
extern int INDENTS;

class AST;
    class Prog;

    class Stmt;
        //class Block;
        //class IfStmt;
        //class AssnStmt;
        //class NullStmt;
        //class RetStmt;
        //class BreakStmt;
        //class WhileStmt;

    class Exp;
        class Id;
        class Num;
        class Times;
        class Plus;
        //class Actuals;
        //class FuncCall;

    class Decl;
        class MainDecl;
        class FuncDecl;
        class VarDecl;
        //class Param;

class AST
{
    protected:

    std::vector<AST*> children;

    virtual void AddChild(AST *child) = 0;

    public:

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
        std::cout << "--Program: " << prog_name << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;

    }
};

class Stmt {};

class Exp {};
    class Num;
    class Id;
    class Plus;
    class Minus;
    class Times;

class Decl : public AST{
    protected:
    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

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
        std::cout << "--Main Function Declaration" << "\n";
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

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    VarDecl(const char* const str) : name(str) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
          std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Variable Declaration" << "\n";
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

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    FuncDecl(const char* const str) : name(str) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
    }

    void Print() override {
          std::cout << std::string(INDENTS*2, INDENT_CHAR);
        std::cout << "--Function Declaration" << "\n";
        INDENTS++;
        for (auto child : children)
        {
            child->Print();
        }
        INDENTS--;
    }
};

class Num : public Exp {
  protected:
    int num; 

  public:
    Num(float value) {
      num = value;
    }
};

class Id : public Exp {
  protected:
    std::string id;

  public:
    Id(std::string value) : id(value) {}
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