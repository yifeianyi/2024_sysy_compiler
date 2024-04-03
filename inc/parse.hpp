#ifndef __PARSE_HPP__
#define __PARSE_HPP__
#include<common.hpp>
#include<tokenize.hpp>
#include<vector>
typedef enum{
    ND_RETURN,
    ND_NUM
}NodeKind;

class ASTNode
{
protected:
    ASTNode *LHS = NULL,*RHS = NULL;
    NodeKind Kind;
    Token *Tok;

public:
    ~ASTNode(){};
    ASTNode(NodeKind Kind,Token *&Tok):Kind(Kind), Tok(Tok){}
    void AddLHS(ASTNode *LHS){
        this->LHS = LHS;
    }
    void AddRHS(ASTNode *RHS){
        this->RHS = RHS;
    }
};
class NumNode : public ASTNode
{
private:
    union Val
    {
        float f;
        int   i;
    }val;
    
public:
    NumNode(Token *Tok): ASTNode(ND_NUM, Tok){
        this->val.i = Tok->getVal();
    }
    ~NumNode(){

    };
    
};
class ObjNode
{
public:
    ObjNode *Next;
    bool IsFunc;
    // ObjNode(/* args */){

    // }
    // ~ObjNode();
    virtual void AddBody(ASTNode *Body){
        if(!this->IsFunc){
            error("This is not function.");
        }
    }
    virtual void AddParams(ASTNode *Body){
        if(!this->IsFunc){
            error("This is not function.");
        }
    }
};

class FuncNode : public ObjNode
{
private:
    string FuncName;
    ASTNode *Body = NULL;
    ObjNode *Locals = NULL;
    ObjNode *Params = NULL;
    uint32_t StackSize = 0;

public:
    ~FuncNode();
    FuncNode(Token *Tok){
        this->FuncName = Tok->Name;
    }
    void AddBody(ASTNode *Body){
        this->Body = Body;
    }
    void AddParams(ObjNode *Params){
        this->Params = Params;
    }
};
// class VarNode : public ObjNode
// {
// private:
//     string VarName;
//     uint32_t Offset;
//     bool IsLocal;
// public:
//     VarNode(/* args */);
//     ~VarNode();
// };
// class IFNode : public ASTNode
// {
// private:
//     ASTNode *Cond;
//     ASTNode *Then;
//     ASTNode *Els;
// };
// class ForNode : public IFNode
// {
// private:
//     ASTNode *Init;
//     ASTNode *Inc;
// public:
//     ForNode(/* args */);
//     ~ForNode();
// };


ObjNode *parse(TokenList *list);

#endif