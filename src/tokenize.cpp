#include "common.hpp"
#include "tokenize.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
using std::string;

static bool isKeywords(Token *Tok);
/*----------------------------------组件定义---------------------------------------------*/
static bool isIdent1(const char C){
  return ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_';
}
static bool isIdent2(const char C){
  return isIdent1(C) || ('0' <= C && C <= '9');
}
static void TagKeywords(Token *head){

  for(Token *T=head; T->Kind !=TK_EOF; T = T->Next ){
    if(isKeywords(T))T->Kind = TK_KEYWORD;
  }
}
static bool isKeywords(Token *Tok){
  static string keywords[]={"return","int","float"};
  int len =  keywords->size();
  for(int i = 0;i < len;i++){
    if(Tok->Name == keywords[i])return true;
  }
  return false;
}
static int readPunct(char *Ptr){
  return ispunct(*Ptr) ? 1 : 0 ;
}
/*----------------------------------核心定义---------------------------------------------*/
TokenList *tokenize(char* P){
  Assert(P!=NULL,"Input File is NULL!");
  TokenList* list = new TokenList;

  while(*P){
    if(isspace(*P)){
      ++P;
      continue;
    }

    //解析数字
    if(isdigit(*P)){
      const char *Start = P;
      int val = strtoul(P, &P, 10);
      string name = list->fetchName(Start,P);
      list->addNode(new DigitalTok(TK_NUM, name, val));
      continue;
    }

    // 解析标记符或关键字
    if (isIdent1(*P)) {
      char *Start = P;
      do {
          ++P;
      } while (isIdent2(*P));
      string name = list->fetchName(Start,P);
      list->addNode(new OtherTok(TK_IDENT, name));
      continue;
    }
    
    // 解析操作符
    int PunctLen = readPunct(P);
    if (PunctLen) {
      string name = list->fetchName(P,PunctLen);
      list->addNode(new OtherTok(TK_PUNCT,name));
      P += PunctLen;
      continue;
    }

      // 非法字符处理
      error("invalid token");
  }

  list->addNode(new OtherTok(TK_EOF,"ending"));
#ifdef __Debug_Token_List__
  list->print();
#endif
  TagKeywords(list->head);     
  return list;
}

char *readFile(const char *Path){
  assert(Path!=NULL);
  FILE *FP;
    //过渡方案
  if (strcmp(Path,"-")==0) {
    FP = stdin;
  } else {
    FP = fopen(Path, "r");
    assert(FP);
    if (!FP)
      error("cannot open %s: %s", Path, strerror(errno));
  }

  // 要返回的字符串
  char *Buf;
  size_t BufLen;
  FILE *Out = open_memstream(&Buf, &BufLen);

  // 读取整个文件
  while(true) {
    char Buf2[4096];
    // 数组指针Buf2，数组元素大小1，数组元素个数4096，文件流指针
    int N = fread(Buf2, 1, sizeof(Buf2), FP);
    if (N == 0)
      break;
    // 数组指针Buf2，数组元素大小1，实际元素个数N，文件流指针
    fwrite(Buf2, 1, N, Out);
  }

  // 对文件完成了读取
  if (FP != stdin)
    fclose(FP);

  fflush(Out);
  // 确保最后一行以'\n'结尾
  if (BufLen == 0 || Buf[BufLen - 1] != '\n')
    fputc('\n', Out);
  fputc('\0', Out);
  fclose(Out);
  return Buf;
}

TokenList *tokenizeFile(const char *Path){
    // printf("In tokenizeFile\n");
    return tokenize(readFile(Path));
}