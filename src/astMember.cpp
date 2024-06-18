#include<ast.hpp>

ASTNode::ASTNode(NodeKind Kind,Token *&Tok):Kind(Kind), Tok(Tok){
    if(Kind != ND_NUM)Tok = Tok->Next;
}
void ASTNode::newBinary(NodeKind Kind, ASTNode *LHS, ASTNode *RHS){

}
void ASTNode::newUnary(NodeKind Kind, ASTNode *LHS){

}
void ASTNode::AddLHS(ASTNode *LHS){
    this->LHS = LHS;
}

void ASTNode::AddRHS(ASTNode *RHS){
    this->RHS = RHS;
}
int ASTNode::getKind(){
    return this->Kind;
}
//==========================================================================================
FuncNode::FuncNode(Token *&Tok):ObjNode(Tok){
    this->IsFunc = true;
}
void FuncNode::AddBody(ASTNode *Body){
    this->Body = Body;
}
void FuncNode::AddParams(ObjNode *Params) {
    this->Params = Params;
}
ASTNode *FuncNode::GetBody(){
    return this->Body;
}