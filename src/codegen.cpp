#include "common.hpp"
#include "tokenize.hpp"
#include "ast.hpp"

static FILE *OutputFile;
static int Depth = 0;
const 

static void printLn(const char *Fmt,...){
  va_list VA;
  va_start(VA, Fmt);
  vfprintf(OutputFile, Fmt, VA);
  va_end(VA);
  fprintf(OutputFile, "\n");
}
static void push(void) {
  printLn("  # 压栈，将a0的值存入栈顶");
  printLn("  addi sp, sp, -4");
  printLn("  sw a0, 0(sp)");
  Depth++;
}
static void pop(string Reg) {
  printLn("  # 弹栈，将栈顶的值存入%s", Reg.c_str());
  printLn("  lw %s, 0(sp)", Reg.c_str());
  printLn("  addi sp, sp, 4");
  Depth--;
}
static void genAddr(VarNode *Nd){
  switch (Nd->Kind) {
  // 变量
  case ND_VAR:
    printLn("#==== 生成 %s 的地址 ====",Nd->getTokName().c_str());
    printLn("  addi a0, fp, %d", Nd->Offset);
    return;
  default:
    break;
  }

  Assert(Nd->Tok, "not an lvalue");
}
static void load(){
  printLn("  lw a0, 0(a0)");
}
static void store(){
  pop("a1");
  printLn("  sw a0, 0(a1)");
}
static void genExpr(ASTNode *Nd){
  // Log("In genExpr.___NdName:%s.",Nd->getTokName().c_str());
  if(Nd->Kind == ND_NUM){
    printLn("  li a0,  %d",Nd->getVal());
    return ;
  }

  switch (Nd->Kind)
  {
  case ND_NUM:
    printLn("  li a0,  %d",Nd->getVal());
    return ;
  case ND_VAR:
     Log("In ND_VAR.Nd_Name:%s.",Nd->getTokName().c_str());
    genAddr((VarNode*)Nd);
    load();
    return ;
  case ND_NEG:
    genExpr(Nd->getLHS());
    // neg a0, a0是sub a0, x0, a0的别名, 即a0=0-a0
    printLn("  # 对a0值进行取反\n");
    printLn("  neg a0, a0\n");
    return;
  case ND_ASSIGN:
    Log("In assign. Nd_name:%s.",Nd->getTokName().c_str());
    genAddr((VarNode*)Nd->getLHS());
    push();
    genExpr(Nd->getRHS());
    store();
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
    printLn("  mul a0, a0, a1");
    return;
  case ND_DIV: // / a0=a0/a1
    printLn("  div a0, a0, a1");
    return;
  default:
    break;
  }
}


static void genStmt( ASTNode *Nd){
  // if(Nd->getKind()==ND_BLOCK)Nd = Nd->getBody();
  
  switch (Nd->getKind())
  {
  case ND_BLOCK:
    for(auto it = Nd->getBody();it != NULL;it = it->Next){
      // Log("%d..NodeName:%s.",++i,it->getTokName().c_str());
      genStmt(it);
      
    }
    // genStmt(Nd->getBody());
    return ;
  case ND_RETURN:
    Nd = Nd->getLHS();
    genExpr(Nd);
    return ;
  case ND_ASSIGN:
    // static int count = 1;
    // Log("In assign count:%d. Nd_Name:%s.",count++,Nd->getTokName().c_str());
    genExpr(Nd);
    return ;
  default:
    Log("Nothing kind match.TokName:%s, Node_Kind:%d.",Nd->getTokName().c_str(), Nd->getKind());
    break;
  }
}
void codegen(ObjNode *Obj,FILE* Out){
  Log("================================================");
  if(!Obj->IsFunc){
    printf("Obj->Name:%s.\n",Obj->Name.c_str());
    Assert(0,"Semantic error.");
  }
  OutputFile = Out;
//局部变量偏移量计算
  int Offset = 0;
  // 读取所有变量
  VarNode *Locals = (VarNode*)Obj->getLVal();
  for (VarNode *Var = Locals; Var ; Var =(VarNode*) Var->Next) {
    // // 每个变量分配空间
    Offset+= 4;
    // 为每个变量赋一个偏移量，或者说是栈中地址
    Var->Offset = -Offset;
  }
  // 将栈对齐到16字节
  Obj->setStackSize(Offset);


//==============================================
  printLn(" .globl %s", Obj->Name.c_str());
  printLn(" .text");
  printLn("%s:", Obj->Name.c_str());
  if(Obj->Name!="main"){
    printLn("  addi sp, sp, -8");
    printLn("  sw ra, 4(sp)");
    printLn("  sw fp, 0(sp)");
  }

  if(Obj->getStackSize()!=0){
    printLn("  mv fp, sp");
    // 将sp写入fp
    printLn("  # 将sp的值写入fp,存储局部变量");
    printLn("  addi sp, sp, -%d", Obj->getStackSize());// 偏移量为实际变量所用的栈大小
    
  }

//===================================================
  ASTNode *Nd = Obj->getBody();
  int i = 0;
  for(auto it = Nd;it != NULL;it = it->Next){
    Log("%d..NodeName:%s.",++i,it->getTokName().c_str());
    printLn("  #%d ====================================",i);
    genStmt(it);
    
  }

//=============================================================
printLn("# =====%s段结束===============", Obj->Name.c_str());
  if(Obj->Name!="main"){
    printLn("# return段标签");
    printLn(".L.return.%s:", Obj->Name.c_str());
    printLn("  mv sp, fp");
    printLn("  ld fp, 0(sp)");
    printLn("  ld ra, 4(sp)");
    printLn("  addi sp, sp, 8");
    // 返回
    printLn("  # 返回a0值给系统调用");
  }
  printLn("  ret");
}