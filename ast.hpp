#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <vector>
#define INDENT_CHAR ' '
extern int INDENTS;

class AST;
    class Prog;

    class Stmt;
        //class Block;
        //class IfStmt;
        class AssnStmt;
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
        for (auto child : children)
        {   
            std::cout << "before seg fault" << std::endl;
            child->Print();
            std::cout << "after seg fault" << std::endl;
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

class AssnStmt : public Stmt {

    protected:

    std::string identifier;
    std::string assignment;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:

    AssnStmt(const char* const id, const char* const assn) : identifier(id), assignment(assn) {};

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
        std::cout << "--Main Function Declaration: {'Name': " << name << "}" << "\n";
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
    std::string name;
    std::string return_type;

    void AddChild(AST *child) override
    {
        children.push_back(child);

    }

    public:
    FuncDecl(const char* const id, const char* const rt) : name(id), return_type(rt) {};

    void AddNode(AST *node) override
    {
        AddChild(node);
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

class Exp : public AST {

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