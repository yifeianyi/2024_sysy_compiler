#ifndef __AST_HPP__
#define __AST_HPP__
#include<common.hpp>
#include<tokenize.hpp>


class Type
{
private:
    int size;
    Type *Base = nullptr;
public:
};

typedef enum{
    ND_RETURN,
    ND_BLOCK,
    ND_NUM,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
    ND_EXPR_STMT, // 表达式语句
    ND_STMT_EXPR, // 语句表达式
}NodeKind;

//-----------------------------------------------------
class ASTNode
{
protected:
    NodeKind Kind;
    
    Token *Tok = nullptr;
public:
    ASTNode *Next = nullptr;
    ~ASTNode(){};
    ASTNode(){}
    ASTNode(NodeKind Kind,Token *&Tok);

    //================= 函数相关 ===================================
    virtual void addParams(ASTNode* params){error("This ASTNode(TokName: %s) isn't a function node.",this->Tok->Name);}
    virtual void addBody(ASTNode* block){error("This ASTNode(TokName: %s) don't have block.",this->Tok->Name);};


    //================= op相关 ===================================
    virtual void addRHS(ASTNode *RHS){error("This ASTNode(TokName: %s) don't have RHS.",this->Tok->Name);};
    virtual void addLHS(ASTNode *LHS){error("This ASTNode(TokName: %s) don't have LHS.",this->Tok->Name);};

    //---------------------------------------------------
    int getKind();
    virtual int getVal(){
        if(this->Kind != ND_NUM){
            Log("ASTNode:%s",this->Tok->Name.c_str());
            error("This ASTNode don't have val.");
            
        }
        return 0;
    }
};

class ObjNode : public ASTNode
{
public:
    Type *type = nullptr;
    string Name;
    bool IsFunc;
    ObjNode(Token *&Tok){
        this->Name = Tok->Name;
        Tok = Tok->Next;
    }
    // ~ObjNode();
    virtual void AddBody(ASTNode *Body){
        if(!this->IsFunc){
            error("Add function body error.");
        }
    }
    virtual void AddParams(ObjNode *Body){
        if(!this->IsFunc){
            error("Add function Params error.");
        }
    }
    virtual ASTNode *GetBody(){
        if(!this->IsFunc){
            error("This is not function.\nDon't have Body.");
        }
        return NULL;
    }
};
class NumNode : public ASTNode
{
private:
    Type *type;
    union Val
    {
        float f;
        int   i;
    }val;
    
public:
    NumNode(Token *&Tok): ASTNode(ND_NUM, Tok){
        this->val.i = Tok->getVal();
        Tok = Tok->Next;
    }
    ~NumNode(){
    };
    int getVal()override{
        return this->val.i;
    }
};
class BlockNode : public ASTNode
{
public:
    ASTNode *Body = nullptr;
    BlockNode(/* args */){}
    BlockNode(NodeKind Kind,Token *&Tok);
    ~BlockNode();
};
class UnaryNode : public ASTNode
{
private:
    ASTNode *LHS = nullptr;
public:
    UnaryNode(NodeKind kind,Token *Tok);
    ~UnaryNode();
    void addLHS(ASTNode *Nd);
    bool IsBinNode();
};
class IFNode : public ASTNode
{
private:
    ASTNode *Cond;
    ASTNode *Then;
    ASTNode *Els;
};
class ForNode : public ASTNode
{
private:
    ASTNode *Cond;
    ASTNode *Then;
public:
    ForNode(/* args */);
    ~ForNode();
};
//-----------------------------------------------------

class FuncNode : public ObjNode
{
private:
    ASTNode *Body = NULL;
    ObjNode *Locals = NULL;
    ObjNode *Params = NULL;
    uint32_t StackSize = 0;

public:
    ~FuncNode();
    FuncNode(Token *&Tok);
    void addBody(ASTNode *Body);
    void addParams(ObjNode *Params);
    ASTNode *getBody();
};
class VarNode : public ObjNode
{
private:
    uint32_t Offset;
    bool IsLocal;
public:
    VarNode(/* args */);
    ~VarNode();
};



ASTNode *ASTBuild(TokenList *list);

#endif