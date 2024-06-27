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
    ND_VAR,
    ND_ASSIGN,

    ND_FUN,
    ND_BLOCK,
    ND_NUM,
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_MOD,
    ND_NEG,
    ND_EXPR_STMT, // 表达式语句
}NodeKind;

//-----------------------------------------------------
class ASTNode
{
public:
    NodeKind Kind;
    Token *Tok = nullptr;
public:
    ASTNode *Next = nullptr;
    ~ASTNode(){};
    ASTNode(){}
    ASTNode(NodeKind Kind, string op);
    ASTNode(Token *&Tok, NodeKind Kind);
    string getTokName();
    
    // void astTokenPrint();
    //================= 函数相关 ===================================
    virtual void addParams(Token *&Tok){error("This ASTNode(TokName: %s) isn't a function node.",this->Tok->Name.c_str());}
    virtual void addBody(ASTNode* block){error("This ASTNode(TokName: %s) don't have block.",this->Tok->Name.c_str());};
    virtual ASTNode *getBody(){
        error("This Node is not function.Don't have Body.TokName:%s.",this->Tok->Name.c_str());
        return NULL;
    }

    //================= op相关 ===================================
    virtual ASTNode *getRHS(){return nullptr;};
    virtual ASTNode *getLHS(){return nullptr;};
    // virtual void newBinary(NodeKind kind, ASTNode *LHS, ASTNode *RHS){error("This ASTNode(TokName: %s) isn't OpNode.",LHS->Tok->Name.c_str());};

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
    bool IsFunc = false;
    ObjNode();
    ObjNode(Token *&Tok,NodeKind Kind);
    // ~ObjNode();
    virtual void addBody(ASTNode *Body){
        if(!this->IsFunc){
            error("Add function body error.");
        }
    }
    virtual void addParams(Token *&Tok){
        if(!this->IsFunc){
            error("Add function Params error.");
        }
    }
    virtual ASTNode *getLVal(){
        if(!this->IsFunc){
            error("This node isn't a function.TokName:%s.",Tok->Name.c_str());
        }
        return nullptr;
    }
    virtual void setStackSize(int32_t size){
        if(!this->IsFunc){
            error("This node isn't a function.Not stack.TokName:%s.",Tok->Name.c_str());
        }
    }
    virtual int32_t getStackSize(){
        if(!this->IsFunc){
            error("This node isn't a function.Not stack.TokName:%s.",Tok->Name.c_str());
        }
        return -1;
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
    NumNode(Token *&Tok, NodeKind kind);
    ~NumNode(){};
    int getVal()override{
        return this->val.i;
    }
};
class BlockNode : public ASTNode
{
public:
    ASTNode *Body = nullptr;
    BlockNode(/* args */){}
    BlockNode(Token *&Tok,NodeKind Kind);
    ~BlockNode();
    ASTNode *getBody();
    void addBody(ASTNode *body){this->Body = body;}
    
};
class UnaryNode : public ASTNode
{

public:
    ASTNode *LHS = nullptr;

public:
    UnaryNode(){}
    UnaryNode(NodeKind kind,string op);
    UnaryNode(Token *&Tok, NodeKind kind);
    UnaryNode(NodeKind Kind,string op,ASTNode *LHS);
    ~UnaryNode();
    ASTNode *getLHS();
};
class BinNode : public UnaryNode
{
public:
    ASTNode *RHS = nullptr;
public:
    BinNode(/* args */){}
    BinNode(NodeKind kind,string op);
    BinNode(NodeKind Kind,string op,ASTNode *LHS, ASTNode *RHS);
    ~BinNode();
    ASTNode *getRHS();
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
    ASTNode *Body = nullptr;
    ObjNode *Params = nullptr;
    uint32_t StackSize = 0;

public:
    ObjNode *Locals = nullptr;

public:
    ~FuncNode();
    FuncNode(Token *&Tok,NodeKind kind);
    void addBody(ASTNode *Body);
    void addParams(Token *&Tok);
    ASTNode *getBody();
    ASTNode *getLVal(){
        return Locals;
    }
    void setStackSize(int32_t size){
        this->StackSize = size;
    }
    int32_t getStackSize(){return this->StackSize;}
};

class VarNode : public ObjNode
{
private:
    
    bool IsLocal;
public:
    int32_t Offset;
public:
    ~VarNode();
    VarNode(/* args */);
    VarNode(Token *&Tok,NodeKind kind):ObjNode(Tok,kind){
        Tok = Tok->Next;
    }
    
};



ASTNode *ASTBuild(TokenList *list);
void skip(Token *&Tok,const string &s);
#endif