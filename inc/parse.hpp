#ifndef __PARSE_HPP__
#define __PARSE_HPP__
#include<common.hpp>
#include<tokenize.hpp>
typedef enum{
    ND_RETURN,
    ND_NUM
}NodeKind;

class ASTNode
{
private:
    NodeKind Kind;
    Token *Tok;
    ASTNode *Next;
public:
    ASTNode(/* args */);
    ~ASTNode();
};

#endif