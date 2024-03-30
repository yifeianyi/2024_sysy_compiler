#include "common.hpp"
#include "tokenize.hpp"
void codegen(TokenList* list ){
    Token *Tok = list->head;
    while(Tok->Name!="return"){
        Tok = Tok->Next;
    }
    printf("  .globl main\n");
    printf("main:\n");
    if(Tok->Name=="return"){
        Tok = Tok->Next;
        printf("  li a0, %d\n", Tok->getVal());
    }
    printf("  ret\n");
}