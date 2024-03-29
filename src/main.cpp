#include "common.hpp"
#include "tokenize.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using std::string;
extern void codegen(TokenList *list);
int main(int Argc, char **Argv){
    if (Argc < 2) {
        printf("%s\n",*Argv);
        // error("%s: invalid number of arguments", Argv[0]);
    }
    
    // printf("%s\n",readFile(string(Argv[1])));
    TokenList *list = tokenizeFile(std::string(Argv[1]));
    // codegen(list);
    return 0;
}