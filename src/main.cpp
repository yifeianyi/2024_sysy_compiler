#include "common.hpp"
#include "tokenize.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
using std::string;
extern void codegen(TokenList *list);
int main(int Argc, char **Argv){
    if (Argc < 2) {
        printf("%s\n",*Argv);
        // error("%s: invalid number of arguments", Argv[0]);
    }
    TokenList *list = tokenizeFile(Argv[1]);
    codegen(list);
    return 0;
}