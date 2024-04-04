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
        error("error! Tok:%s s:%s ",Tok->Name.c_str(),s.c_str());
    }
}
static ObjNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
static ObjNode *FuncParams(Token *&Tok);
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
    ObjNode *Func = new FuncNode(Tok);
    skip(Tok,"(");
    Func->AddParams(FuncParams(Tok));
    skip(Tok,")");
    Func->AddBody(Block(Tok));
    return Func;
}

static void declspec(Token *&Tok) {
    if( Tok->Name == "int" ){
        Tok = Tok->Next;
        return ;
    }

    if( Tok->Name == "void" ){
        Tok = Tok->Next;
        return ;
    }
    if( Tok->Name == "float"){
        Tok = Tok->Next;
        return ;
    }
    printf("Type Name error\n");
    assert(0);
    return;
}
static ObjNode *FuncParams(Token *&Tok){
    //过渡方案，暂不支持函数传参
    return NULL;
}
static ASTNode *Block(Token *&Tok){
    ASTNode *Nd = BlockItem(Tok);
    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    ASTNode head;
    ASTNode *Cur = &head;
    skip(Tok,"{");
    while(Tok->Name != "}"){
        Cur->Next = Stmt(Tok);
        Cur = Cur->Next;
    }
#ifdef __DEBUG_BLOCKITEM__
    Log("TokenName:%s",Tok->Name.c_str());
#endif
    skip(Tok,"}");
    return head.Next;
}
static ASTNode *Stmt(Token *&Tok){
    if (Tok->Name == "return")
    {
        ASTNode *Nd = new ASTNode(ND_RETURN,Tok);
#ifdef __DEBUG_PARSE_STMT__
        if(Tok->Kind == TK_NUM){
            printf("TokenKind is TK_NUM\n");
            printf("Val:%d\n",Tok->getVal());
        }
#endif        
        Nd->AddLHS(new NumNode(Tok));
        skip(Tok,";");
        return Nd;
    }
    return NULL;
}

