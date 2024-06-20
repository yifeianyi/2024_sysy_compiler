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

static ASTNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
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
    ASTNode astTree = {};
    ASTNode *Cur = &astTree;
    Token *Tok = list->head;
    while(Tok->Kind!=TK_EOF){
        // Log("TokName:%s.",Tok->Name.c_str());
        Cur->Next = FuncDef(Tok);
        Cur = Cur->Next;
    }
    return astTree.Next;
}

static ASTNode *FuncDef(Token *&Tok){
    declspec(Tok);
    FuncNode *Nd = new FuncNode(Tok,ND_FUN);
    Nd->addParams(Tok);
    Nd->addBody(Block(Tok));
    return Nd;
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
    Assert(0,"Type Name error.TokName:%s ,TokenKind:%d.",Tok->Name.c_str(),Tok->Kind);
    return;
}
static ASTNode *Block(Token *&Tok){
    BlockNode *Nd = new BlockNode(Tok, ND_BLOCK);
    Nd->Body = BlockItem(Tok);
    Log("NodeName:%s.",Nd->getTokName().c_str());
    skip(Tok,"}");
    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    ASTNode head = {};
    ASTNode *Cur = &head;
    while(Tok->Name!="}"){
        ASTNode *tmp = Stmt(Tok);
        
        if(tmp){
            Log("NodeName:%s.",tmp->getTokName().c_str());
            Cur->Next = tmp;
            Cur = Cur->Next;
        }
    }
    Log("NodeName:%s.",head.Next->getTokName().c_str());
    return head.Next;
}
static ASTNode *Stmt(Token *&Tok){
    if(Tok->Name == "return"){
        UnaryNode *Nd = new UnaryNode(Tok,ND_RETURN);
        Nd->LHS = new NumNode(Tok,ND_NUM);
        skip(Tok,";");
        return Nd;
    }
    if(Tok->Name == ";"){
        skip(Tok,";");
        return nullptr;
    }


    Assert(0,"unkonwn the Node in astBuild-Stmt.TokName:%s",Tok->Name.c_str());
    return nullptr;
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
