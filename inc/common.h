#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// #define __log__
typedef struct Token Token; //词法分析相关
typedef struct Node Node;   //语法分析相关

/*------------------------------------------------------------------*/
typedef enum{
    TK_NUM,
    TK_KEYWORD,
    TK_IDENT,
    TK_PUNCT,
    TK_EOF,
}TokenKind;

struct Token
{
    TokenKind Kind;
    Token *Next;
    int Val;
    char *Loc;
    int Len;
};


/*------------------------------------------------------------------*/
typedef enum{
    ND_BLOCK,
    ND_NUM,
    ND_FUNCALL,
}NodeKind;

struct Node
{
    NodeKind Kind;
};

typedef struct Function Function;
struct Function
{
    Node *Body;
};



/*----------------------------------------------------------*/
Token *tokenizeFile(char *Path);
char *readFile(char *Path);
/*----------------------------------------------------------*/

/*----------------------------------------------------------*/
// 去除了static用以在多个文件间访问
// 报错函数


/*----------------------------------------------------------*/
bool equal(const Token *Tok,const char *Str);
/*----------------------------------------------------------*/
void codegen(Token *Tok);
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/




#include "debug.h"

#endif
