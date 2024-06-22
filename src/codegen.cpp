#include "common.hpp"
#include "tokenize.hpp"
#include "ast.hpp"

static FILE *OutputFile;
static int Depth = 0;
static void printLn(const char *Fmt,...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(OutputFile, Fmt, VA);
  va_end(VA);
  fprintf(OutputFile, "\n");
}
static void push(void) {
  printLn("  # 压栈，将a0的值存入栈顶");
  printLn("  addi sp, sp, -8");
  printLn("  sd a0, 0(sp)");
  Depth++;
}
static void pop(string Reg) {
  printLn("  # 弹栈，将栈顶的值存入%s", Reg.c_str());
  printLn("  ld %s, 0(sp)", Reg.c_str());
  printLn("  addi sp, sp, 8");
  Depth--;
}

static void genExpr(ASTNode *Nd){
  Log("In genExpr._________NdName:%s.",Nd->getTokName().c_str());
  if(Nd->Kind == ND_NUM){
    printLn(" li a0,  %d",Nd->getVal());
    return ;
  }

  switch (Nd->Kind)
  {
  case ND_NUM:
    printLn(" li a0,  %d",Nd->getVal());
    return ;
  case ND_NEG:
    genExpr(Nd->getLHS());
    // neg a0, a0是sub a0, x0, a0的别名, 即a0=0-a0
    printLn("  # 对a0值进行取反\n");
    printLn("  neg a0, a0\n");
    return;
  default:
    break;
  }

  // if(Nd->getRHS())
  genExpr(Nd->getRHS());
  push();  
  // if(Nd->getLHS()) 
  genExpr(Nd->getLHS());
  pop("a1");

  switch (Nd->Kind)
  {
  case ND_ADD: // + a0=a0+a1
    printLn("  # a0+a1，结果写入a0");
    printLn("  add a0, a0, a1");
    return;
  case ND_SUB: // - a0=a0-a1
    printLn("  # a0-a1，结果写入a0");
    printLn("  sub a0, a0, a1");
    return;
  case ND_MUL: // * a0=a0*a1
    printLn("  # a0×a1，结果写入a0");
    printLn("  mul a0, a0, a1");
    return;
  case ND_DIV: // / a0=a0/a1
    printLn("  # a0÷a1，结果写入a0");
    printLn("  div a0, a0, a1");
    return;
  default:
    break;
  }
}


static void genStmt( ASTNode *Nd){
  if(Nd->getKind()==ND_BLOCK)Nd = Nd->getBody();
  switch (Nd->getKind())
  {
  case ND_RETURN:
    Nd = Nd->getLHS();
    genExpr(Nd);
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