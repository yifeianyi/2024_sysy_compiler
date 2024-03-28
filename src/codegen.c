#include "common.h"

void codegen(Token* Tok ){
    
    while(!equal(Tok, "return")){
        Tok = Tok->Next;
    }
    printf("  .globl main\n");
    printf("main:\n");
    if(equal(Tok,"return")){
        Tok = Tok->Next;
        printf("  li a0, %d\n", Tok->Val);
    }
    printf("  ret\n");
}