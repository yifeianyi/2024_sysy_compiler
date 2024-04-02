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
private:
    ASTNode *LHS,*RHS;
    NodeKind Kind;
    Token *Tok;

public:
    virtual ~ASTNode();
    virtual void CreateNode() = 0;
    ASTNode *GetParams(ASTNode *Params);
    ASTNode *GetBody(ASTNode *Body);
    ASTNode *SetLHS(ASTNode *LHS);
};
class NumNode : public ASTNode
{
private:
    union Val
    {
        float f;
        int   i;
    };
    
public:
    NumNode(/* args */);
    ~NumNode();
    void CreateNode();
};


class ObjNode : public ASTNode
{
private:
    // Type *Ty;
    uint32_t Offset;
    bool IsFunc;
public:
    ObjNode(/* args */);
    ~ObjNode();
    void CreateNode();
};

class VarNode : public ObjNode
{
private:
    string VarName;
    bool IsLocal;
public:
    VarNode(/* args */);
    ~VarNode();
    void CreateNode();
};

class FuncNode : public ObjNode
{
private:
    string FuncName;
    ASTNode *Body;
    std::vector<ObjNode> Params;
    uint32_t StackSize;

public:
    FuncNode(/* args */);
    ~FuncNode();
    void CreateNode();
};

class OpNode : public ASTNode
{
private:
    
public:
    OpNode(/* args */);
    ~OpNode();
    void CreateNode();
};

class IFNode : public ASTNode
{
private:
    ASTNode *Cond;
    ASTNode *Then;
    ASTNode *Els;
};

class ForNode : public IFNode
{
private:
    ASTNode *Init;
    ASTNode *Inc;
public:
    ForNode(/* args */);
    ~ForNode();
};

#endif