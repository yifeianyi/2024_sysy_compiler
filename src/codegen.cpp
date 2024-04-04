#include "common.hpp"
#include "tokenize.hpp"
#include "parse.hpp"

static FILE *OutputFile;
static void printLn(const char *Fmt,...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(OutputFile, Fmt, VA);
  va_end(VA);
  fprintf(OutputFile, "\n");
}
static void genStmt( ASTNode *Nd){
  switch (Nd->getKind())
  {
  case ND_RETURN:
    Nd = Nd->LHS;
    printLn(" li a0,  %d",Nd->getVal());
    break;
  
  default:

    break;
  }
}
void codegen(ObjNode *Obj,FILE* Out){
    if(!Obj->IsFunc){
      printf("Obj->Name:%s.\n",Obj->Name.c_str());
      error("Semantic error.");
    }
    OutputFile = Out;

    ASTNode *Nd = Obj->GetBody();

    printLn(" .globl %s", Obj->Name.c_str());
    printLn(" .text");
    printLn("%s:", Obj->Name.c_str());
    for(auto it = Nd;it != NULL;it = it->Next){
      genStmt(it);
    }
    printLn(" ret");
}