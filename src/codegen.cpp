#include "common.hpp"
#include "tokenize.hpp"
static FILE *OutputFile;
static void printLn(const char *Fmt,...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(OutputFile, Fmt, VA);
  va_end(VA);
  fprintf(OutputFile, "\n");
}

void codegen(TokenList* list ,FILE* Out){
    OutputFile = Out;
    Token *Tok = list->head;
    while(Tok->Name!="return"){
        Tok = Tok->Next;
    }
    printLn("  .globl main");
    printLn("main:");
    if(Tok->Name=="return"){
        Tok = Tok->Next;
        printLn("  li a0, %d", Tok->getVal());
    }
    printLn("  ret");
}