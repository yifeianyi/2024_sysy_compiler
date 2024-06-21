#include<common.hpp>
#include<ast.hpp>
/*
    CompUnit    = FuncDef
    FuncDef     = FuncType Ident '(' [FuncFParams] ')' Block
    FuncType    = 'void' | 'int' | 'float'
    Block       = '{' { BlockItem } '}' 

    BlockItem   = Stmt
    // Stmt        =  [Expr] ';'| 'return' [Expr]';' 
    // Expr        = AddExpr
    // AddExpr     = MulExpr   | MulExpr ('+' | '-') MulExpr
    // MulExpr     = UnaryExpr | MulExpr ('*' | '/' | '%') UnaryExpr
    // UnaryExpr   = PrimaryExpr | UnaryOp UnaryExpr
    // UnaryOp     = '+' | '-' | '!'
    // PrimaryExpr = '('Expr ')' | Number
    --------------------------------------------------------------------
*/ 

static ASTNode *FuncDef(Token *&Tok);
static void declspec(Token *&Tok);
static ASTNode *Block(Token *&Tok);
static ASTNode *BlockItem(Token *&Tok);
static ASTNode *Stmt(Token *&Tok);
static ASTNode *Expr(Token *&Tok);
static ASTNode *AddExpr(Token *&Tok);
static ASTNode *MulExpr(Token *&Tok);
static ASTNode *UnaryExpr(Token *&Tok);
static ASTNode *PrimaryExpr(Token *&Tok);
//---------------------------------------


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

// 创建多个 Block
static ASTNode *Block(Token *&Tok){
    ASTNode *Nd;
    Nd = BlockItem(Tok);
    skip(Tok,"}");

    return Nd;
}
static ASTNode *BlockItem(Token *&Tok){
    BlockNode *block = new BlockNode(Tok, ND_BLOCK);
    
    ASTNode head = {};
    ASTNode *Cur = &head;
    while(Tok->Name!="}"){
        ASTNode *tmp = nullptr;
        tmp = Stmt(Tok);
        if(tmp){
            Cur->Next = tmp;
            Cur = Cur->Next;
        }
    }
    return head.Next;
}
static ASTNode *Stmt(Token *&Tok){
    if(Tok->Name == ";"){
        skip(Tok,";");
        return nullptr;
    }
    if(Tok->Name == "return"){
        UnaryNode *Nd = new UnaryNode(Tok,ND_RETURN);
        // Nd->LHS = new NumNode(Tok,ND_NUM);
        Nd->LHS =  AddExpr(Tok);
        Assert(Nd->LHS,"'return' left_son is NULL.");
        skip(Tok,";");
        return Nd;
    }

    // Assert(0,"unkonwn the Node in astBuild-Stmt.TokName:%s",Tok->Name.c_str());
    return AddExpr(Tok);
}

static ASTNode *AddExpr(Token *&Tok){
    ASTNode *tmp = MulExpr(Tok);
    Assert(tmp!=NULL,"MulExpr Build error.");
    // BinNode *Nd = new BinNode;

    while(true){
        if(Tok->Name == "+"){
            Tok = Tok->Next;
            Log("In add.");
            BinNode *Nd = new BinNode(ND_ADD, "+", tmp, MulExpr(Tok));
            tmp = Nd;
            Log("Build Add Node successed.__NdName:%s.",Nd->getTokName().c_str());
            continue;
        }

        if(Tok->Name == "-"){
            Tok = Tok->Next;
            // Nd->newBinary(ND_SUB, tmp, MulExpr(Tok));
            BinNode *Nd = new BinNode(ND_SUB, "-", tmp, MulExpr(Tok));
            continue;
        }
        BinNode *Nd = (BinNode*)tmp;
        Assert(Nd!=NULL,"AddExpr return error");
        Log("Create AddExpr  successed.__NdName:%s.",Nd->getTokName().c_str());
        return Nd;
    }
}

static ASTNode *MulExpr(Token *&Tok){
    ASTNode *tmp = UnaryExpr(Tok);
    BinNode *Nd = new BinNode;

    while(true){
        if(Tok->Name == "*"){
            Tok = Tok->Next;
            // Nd->newBinary(ND_MUL,tmp, UnaryExpr(Tok));
            BinNode *Nd = new BinNode(ND_MUL, "*", tmp, UnaryExpr(Tok));
            tmp = Nd;
            continue;
        }

        if(Tok->Name == "/"){
            Tok = Tok->Next;
            // Nd->newBinary(ND_DIV,tmp, UnaryExpr(Tok));
            BinNode *Nd = new BinNode(ND_DIV, "/", tmp, UnaryExpr(Tok));
            tmp = Nd;
            continue;
        }

        if(Tok->Name == "%"){
            Tok = Tok->Next;
            // Nd->newBinary(ND_MOD,tmp, UnaryExpr(Tok));
            BinNode *Nd = new BinNode(ND_MOD, "%", tmp, UnaryExpr(Tok));
            tmp = Nd;
            continue;
        }
        if(Nd->Tok==nullptr) Nd = (BinNode*)tmp;
        Log("In MulExpr");
        return Nd;
    }
}

static ASTNode *UnaryExpr(Token *&Tok){
    ASTNode *Nd = PrimaryExpr(Tok);
    return Nd;
}

static ASTNode *PrimaryExpr(Token *&Tok){
    // Log("In PrimaryExpr.");
    if(Tok->Name == "("){
        skip(Tok,"(");
        Log("In primaryExpr (");
        ASTNode *Nd = AddExpr(Tok);
        skip(Tok,")");
        return Nd;
    }

    if(Tok->Kind == TK_NUM){
        Log("In TK_NUM.TokName:%s.",Tok->Name.c_str());
        NumNode *Nd = new NumNode(Tok,ND_NUM);
        Log("Node value:%d",Nd->getVal());
        Assert(Nd,"Parse primaryExpr TK_NUM error.");
        return Nd;
    }

    Assert(0,"expected an expression.TokName:%s.",Tok->Name.c_str());
    return nullptr;
}
