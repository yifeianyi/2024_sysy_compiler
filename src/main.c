#include "common.h"
int main(int Argc, char **Argv){
    if (Argc != 2) {
        printf("%s\n",*Argv);
        error("%s: invalid number of arguments", Argv[0]);
    }

    Token *Tok = tokenizeFile(Argv[1]);
    
    // printf("\n-----------------------------------------\n");
    // printTokenList(Tok);
    // panic("%s\n",Tok->Loc);
    codegen(Tok);
    return 0;
}