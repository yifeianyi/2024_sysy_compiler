#include<ast.hpp>

/*
    ASTNode Member function
*/
ASTNode::ASTNode(NodeKind Kind,Token *&Tok):Kind(Kind), Tok(Tok){
    if(Kind != ND_NUM)Tok = Tok->Next;
}

void ASTNode::addParams(ASTNode* params){
    error("This ASTNode isn't a function node.");
}
void ASTNode::addBody(ASTNode* block){

}


void ASTNode::addRHS(ASTNode *RHS){

}
void ASTNode::addLHS(ASTNode *LHS){

}
int ASTNode::getKind(){
    return this->Kind;
}

//==========================================================================================
FuncNode::FuncNode(Token *&Tok):ObjNode(Tok){
    this->IsFunc = true;
}
void FuncNode::addBody(ASTNode *Body){
    this->Body = Body;
}
void FuncNode::addParams(ObjNode *Params) {
    this->Params = Params;
}
ASTNode *FuncNode::getBody(){
    return this->Body;
}