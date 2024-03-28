#include "common.h"


// static char *CurrentFilename;
// static char *CurrentInput;

// 输出错误信息
void error(const char *Fmt, ...) {
  // 定义一个va_list变量
  va_list VA;
  va_start(VA, Fmt);
  fprintf(stderr, "\n");
  va_end(VA);
  exit(1);
}

bool equal(const Token *Tok,const char *Str){
  return memcmp(Tok->Loc, Str, Tok->Len) == 0 && Str[Tok->Len] == '\0';
}


void printNChars(const char *str, int n) {
    for (int i = 0; i < n; i++) {
        if (str[i] == '\0') {
            break; 
        }
        printf("%c", str[i]);
    }
    printf("\n");
}

void printTokenList(Token *Tok){
  for(;Tok->Kind !=TK_EOF ; Tok = Tok->Next){
    if(Tok->Kind == TK_NUM){
      printf(" %d",Tok->Val);
    }
    else{
      printNChars(Tok->Loc,Tok->Len);
    }
        
  }
}

