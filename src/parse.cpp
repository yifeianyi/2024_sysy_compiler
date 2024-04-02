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
static ASTNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
static ASTNode *FuncParams(Token *&Tok);
static ASTNode *Block(Token *&Tok);
static ASTNode *BlockItem(Token *&Tok);
static ASTNode *Stmt(Token *&Tok);

ASTNode *parse(TokenList *list){
    Token *Tok = list->head;
    ASTNode *AstTree;
    // declspec(Tok);
    while(Tok->Kind != TK_EOF){
        AstTree = FuncDef(Tok);
    }
}

static ASTNode *FuncDef(Token *&Tok){
    declspec(Tok);
    ASTNode *Cur = new FuncNode();
    Cur->GetParams(FuncParams(Tok));
    Cur->GetBody(Block(Tok));
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
static ASTNode *Block(Token *&Tok){
    ASTNode *Nd = BlockItem(Tok);
    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    ASTNode *Nd = Stmt(Tok);
    return Nd;
}
static ASTNode *Stmt(Token *&Tok){
    if (Tok->Name == "return")
    {
        ASTNode *Nd = new OpNode();
        ASTNode *Num= new NumNode();
        Nd->SetLHS(Num);
    }
}

