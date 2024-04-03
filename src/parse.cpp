#include<common.hpp>
#include<parse.hpp>
/*
    CompUnit    = FuncDef
    FuncDef     = FuncType Ident '(' [FuncFParams] ')' Block
    FuncType    = 'void' | 'int' | 'float'
    Block       = '{' { BlockItem } '}' 
    --------------------------------------------------------------------
    BlockItem   = Stmt
    Stmt        = 'return' [Number] ';'
*/ 
static void skip(Token *&Tok,const string &s){
    if(Tok->Name == s){
        Tok = Tok->Next;
    }else{
        error("function is error");
    }
}
static ObjNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
static ASTNode *FuncParams(Token *&Tok);
static ASTNode *Block(Token *&Tok);
static ASTNode *BlockItem(Token *&Tok);
static ASTNode *Stmt(Token *&Tok);
ObjNode *parse(TokenList *list){
    Token *Tok = list->head;
    ObjNode *AstTree;
    AstTree = FuncDef(Tok);
    return AstTree;
}

static ObjNode *FuncDef(Token *&Tok){
    declspec(Tok);
    ObjNode *Cur = new FuncNode(Tok);
    skip(Tok,"(");
    Cur->AddParams(FuncParams(Tok));
    skip(Tok,")");
    Cur->AddBody(Block(Tok));
    return Cur;
}

static void declspec(Token *&Tok) {
    if( Tok->Name == "int" ){
        Tok = Tok->Next;
    }

    if( Tok->Name == "void" ){
        Tok = Tok->Next;
    }
    if( Tok->Name == "float"){
        Tok = Tok->Next;
    }
    printf("Type Name error\n");
    assert(0);
    return;
}
static ASTNode *FuncParams(Token *&Tok){
    //过渡方案，暂不支持函数传参
    return NULL;
}
static ASTNode *Block(Token *&Tok){
    ASTNode *Nd = BlockItem(Tok);
    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    skip(Tok,"{");
    ASTNode *Nd = Stmt(Tok);
    skip(Tok,"}");
    return Nd;
}
static ASTNode *Stmt(Token *&Tok){
    if (Tok->Name == "return")
    {
        ASTNode *Nd = new ASTNode(ND_RETURN,Tok);
        Nd->AddLHS(new NumNode(Tok));
        return Nd;
    }
    return NULL;
}

