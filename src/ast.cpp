#include<common.hpp>
#include<ast.hpp>
/*
    CompUnit    = FuncDef
    FuncDef     = FuncType Ident '(' [FuncFParams] ')' Block
    FuncType    = 'void' | 'int' | 'float'
    Block       = '{' { BlockItem } '}' 
    BlockItem   = { Stmt }
    Stmt        = 'return' [Expr]';' | [ Expr ]';'
    --------------------------------------------------------------------
    Expr        = AddExpr
    AddExpr     = MulExpr | MulExpr ('+' | '-') MulExpr
    MulExpr     = UnaryExpr | MulExpr ('*' | '/' | '%') UnaryExp r
    UnaryExpr   = PrimaryExpr
    PrimaryExpr = '('Expr ')' | Number
*/ 
static void skip(Token *&Tok,const string &s){
    if(Tok->Name == s){
        Tok = Tok->Next;
    }else{
        error("error! Tok:%s s:%s ",Tok->Name.c_str(),s.c_str());
    }
}
static ASTNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
static ObjNode *FuncParams(Token *&Tok);
static ASTNode *Block(Token *&Tok);
static ASTNode *BlockItem(Token *&Tok);
static ASTNode *Stmt(Token *&Tok);
//---------------------------------------
// static ASTNode *Expr(Token *&Tok);
// static ASTNode *AddExpr(Token *&Tok);
// static ASTNode *MulExpr(Token *&Tok);
// static ASTNode *UnaryExpr(Token *&Tok);
// static ASTNode *PrimaryExpr(Token *&Tok);

ASTNode *ASTBuild(TokenList *list){
    Token *Tok = list->head;
    ASTNode *AstTree;
    AstTree = FuncDef(Tok);
    return AstTree;
}

static ASTNode *FuncDef(Token *&Tok){
    declspec(Tok);
    FuncNode *Func = new FuncNode(Tok);
    skip(Tok,"(");
    Func->addParams(FuncParams(Tok));
    skip(Tok,")");
    Func->addBody(Block(Tok));
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
    skip(Tok,"{");
    ASTNode *Nd = BlockItem(Tok);
    skip(Tok,"}");
    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    BlockNode *Nd = new BlockNode(ND_BLOCK,Tok);

    ASTNode head;
    ASTNode *Cur = &head;
    while(Tok->Name != "}"){
        ASTNode *tmp = Stmt(Tok);
        if(tmp==NULL)continue;
        else{
            Cur->Next = tmp;
            Cur = Cur->Next;
        }
    }
    Nd->addBody(head.Next);
    return Nd;
}
static ASTNode *Stmt(Token *&Tok){
    
    if(Tok->Name == ";"){
        skip(Tok,";");
        return NULL;
    }

    if (Tok->Name == "return")
    {
        UnaryNode *Nd = new UnaryNode(ND_RETURN,Tok);
#ifdef __DEBUG_PARSE_STMT__
        if(Tok->Kind == TK_NUM){
            printf("TokenKind is TK_NUM\n");
            printf("Val:%d\n",Tok->getVal());
        }
#endif        
        Nd->addLHS(new NumNode(Tok));
        skip(Tok,";");
        return Nd;
    }
    return NULL;
}


// static ASTNode *Expr(Token *&Tok){
//     ASTNode *Nd = AddExpr(Tok);
//     return Nd;
// }
// static ASTNode *AddExpr(Token *&Tok){
//     ASTNode *Nd = MulExpr(Tok);

//     while(true){
//         if(Tok->Name == "+"){
//             Tok = Tok->Next;
//             Nd->newBinary(ND_ADD, Nd, MulExpr(Tok));
//             continue;
//         }

//         if(Tok->Name == "-"){
//             Tok = Tok->Next;
//             Nd->newBinary(ND_SUB, Nd, MulExpr(Tok));
//             continue;
//         }
//         return Nd;
//     }
// }

// static ASTNode *MulExpr(Token *&Tok){
//     ASTNode *Nd = UnaryExpr(Tok);

//     while(true){
//         if(Tok->Name == "*"){
//             Tok = Tok->Next;
//             Nd->newBinary(ND_MUL,Nd, UnaryExpr(Tok));
//             continue;
//         }

//         if(Tok->Name == "/"){
//             Tok = Tok->Next;
//             Nd->newBinary(ND_DIV,Nd, UnaryExpr(Tok));
//             continue;
//         }

//         if(Tok->Name == "%"){
//             Tok = Tok->Next;
//             Nd->newBinary(ND_MOD,Nd, UnaryExpr(Tok));
//             continue;
//         }
//         return Nd;
//     }
// }

// static ASTNode *UnaryExpr(Token *&Tok){
//     ASTNode *Nd = PrimaryExpr(Tok);
//     return Nd;
// }

// static ASTNode *PrimaryExpr(Token *&Tok){
//     if(Tok->Name == "("){
//         ASTNode *Nd = new ASTNode(ND_STMT_EXPR, Tok);

        
//     }
// }
