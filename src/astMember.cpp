#include<ast.hpp>

/*
    ASTNode Member function
*/
ASTNode::ASTNode(NodeKind Kind,string op):Kind(Kind){
    Token *Tok = new Token(TK_PUNCT,op);
    this->Tok = Tok;
}
ASTNode::ASTNode(Token *&Tok,NodeKind Kind):Kind(Kind), Tok(Tok){}

int ASTNode::getKind(){
    return this->Kind;
}
string ASTNode::getTokName(){
    return this->Tok->Name;
}
// void ASTNode::astTokenPrint(){
//     Log("NodeName:%s.",getTokName().c_str());
// }

ObjNode::ObjNode(Token *&Tok,NodeKind Kind):ASTNode(Tok,Kind){
    this->Name = Tok->Name;
}


//==========================================================================================
FuncNode::FuncNode(Token *&Tok, NodeKind kind):ObjNode(Tok, kind){
    this->IsFunc = true;
    Tok = Tok->Next;
}
void FuncNode::addBody(ASTNode *Body){
    this->Body = Body;
}
void FuncNode::addParams(Token *&Tok){
    skip(Tok,"(");
    //暂时无参,不处理.
    skip(Tok,")");
    return ;
}

ASTNode *FuncNode::getBody(){
    return this->Body;
}
ASTNode *BlockNode::getBody(){
    return this->Body;
}


BlockNode::BlockNode(Token *&Tok, NodeKind kind):ASTNode(Tok, kind){skip(Tok,"{");}

//====================== UnaryNode | BinNode =========================
UnaryNode::UnaryNode(NodeKind kind,string op):ASTNode(kind,op){}
UnaryNode::UnaryNode(Token *&Tok, NodeKind kind):ASTNode(Tok, kind){Tok = Tok->Next;}
ASTNode *UnaryNode::getLHS(){
    return this->LHS;
}
BinNode::BinNode(NodeKind Kind,string op):UnaryNode(Kind,op){}
BinNode::BinNode(NodeKind Kind,string op,ASTNode *LHS, ASTNode *RHS):UnaryNode(Kind,op){
    this->LHS = LHS;
    this->RHS = RHS;
}
ASTNode *BinNode::getRHS(){
    return this->RHS;
}
NumNode::NumNode(Token *&Tok, NodeKind kind):ASTNode(Tok, kind){
    this->val.i = Tok->getVal();
    Tok = Tok->Next;
    // Log("Num value: %d.",this->val.i);
}


void skip(Token *&Tok,const string &s){
    if(Tok->Name == s){
        Tok = Tok->Next;
    }else{
        error("error! Tok:%s s:%s ",Tok->Name.c_str(),s.c_str());
    }
}