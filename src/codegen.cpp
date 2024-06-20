#include "common.hpp"
#include "tokenize.hpp"
#include "ast.hpp"

static FILE *OutputFile;
static void printLn(const char *Fmt,...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(OutputFile, Fmt, VA);
  va_end(VA);
  fprintf(OutputFile, "\n");
}
static void genStmt( ASTNode *Nd){
  if(Nd->getKind()==ND_BLOCK)Nd = Nd->getBody();
  switch (Nd->getKind())
  {
  case ND_RETURN:
    Nd = Nd->getLHS();
    Log("In ND_RETURN.");
    printLn(" li a0,  %d",Nd->getVal());
    break;
  
  default:
    Log("Nothing kind match.TokName:%s, Node_Kind:%d.",Nd->getTokName().c_str(), Nd->getKind());
    break;
  }
}
void codegen(ObjNode *Obj,FILE* Out){
    if(!Obj->IsFunc){
      printf("Obj->Name:%s.\n",Obj->Name.c_str());
      error("Semantic error.");
    }
    OutputFile = Out;

    ASTNode *Nd = Obj->getBody();
    
    printLn(" .globl %s", Obj->Name.c_str());
    printLn(" .text");
    printLn("%s:", Obj->Name.c_str());
    int i = 1;
    for(auto it = Nd;it != NULL;it = it->Next){
      Log("%d..NodeName:%s.",i++,it->getTokName().c_str());
      genStmt(it);
    }
    printLn(" ret");
}