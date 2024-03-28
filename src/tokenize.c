#include "common.h"

/*----------------------------------组件声明---------------------------------------------*/
static bool isIdent1(const char C);
static bool isIdent2(const char C);
static int readPunct(char *Ptr);
static void TagKeywords(Token *Tok);
static bool isKeywords(Token *Tok);

//词链表相关
static Token *newToken(TokenKind Kind, char *Start, char *End);

/*----------------------------------核心声明---------------------------------------------*/
Token *tokenize(char *Filename, char *P);
char *readFile(char *Path);
Token *tokenizeFile(char *Path);

static char *CurrentFilename; // 输入的文件名
static char *CurrentInput; // 输入的字符串

/*----------------------------------组件定义---------------------------------------------*/
static bool isIdent1(const char C){
  return ('a' <= C && C <= 'z') || ('A' <= C && C <= 'Z') || C == '_';
}

static bool isIdent2(const char C){
  return isIdent1(C) || ('0' <= C && C <= '9');
}

static void TagKeywords(Token *Tok){
  for(Token *T=Tok; T->Kind !=TK_EOF; T = T->Next ){
    if(isKeywords(T))T->Kind = TK_KEYWORD;
  }
}
static bool isKeywords(Token *Tok){
  static char *keywords[]={( char*)"return"};

  for(int i = 0;i < (int)(sizeof(keywords)/sizeof(*keywords));i++){
    if(equal(Tok,keywords[i]))return true;
  }
  return false;
}

static int readPunct(char *Ptr){
  return ispunct(*Ptr) ? 1 : 0 ;
}

//词链表相关
static Token *newToken(TokenKind Kind, char *Start, char *End){
  Token *Tok = (Token*)calloc(1,sizeof(Token));
  Tok->Kind = Kind;
  Tok->Loc  = Start;
  Tok->Len  = End - Start;
  return Tok;
}


/*----------------------------------核心定义---------------------------------------------*/
// 终结符解析，文件名，文件内容
Token *tokenize(char *Filename, char *P){
  Assert(P!=NULL,"Input File is NULL!");
  CurrentFilename = Filename;
  CurrentInput = P;
  Token Head = {};
  Token *Cur = &Head;

  while(*P){
    if(isspace(*P)){
      ++P;
      continue;
    }

    //解析数字
    if(isdigit(*P)){
      Cur->Next = newToken(TK_NUM, P, P);
      Cur = Cur->Next;
      const char *OldPtr = P;
      Cur->Val = strtoul(P, &P, 10);
      Cur->Len = P-OldPtr;

    #ifdef __log__
      printf("%d\n",Cur->Val);
    #endif
      continue;
    }

    // 解析标记符或关键字
    if (isIdent1(*P)) {
      char *Start = P;
      do {
          ++P;
      } while (isIdent2(*P));
      
      Cur->Next = newToken(TK_IDENT, Start, P);
      Cur = Cur->Next;
    #ifdef __log__
      printNChars(Cur->Loc,Cur->Len);//debug
    #endif
      continue;
    }


      // 解析操作符
    int PunctLen = readPunct(P);
    if (PunctLen) {
      Cur->Next = newToken(TK_PUNCT, P, P + PunctLen);
      Cur = Cur->Next;
      // 指针前进Punct的长度位
      P += PunctLen;
    #ifdef __log__
      printNChars(Cur->Loc,Cur->Len);//debug
    #endif
      continue;
    }

      //非法字符处理
      error("invalid token");
  }

  
  Cur->Next = newToken(TK_EOF, P, P); //给 token 链表 加个 ending
  TagKeywords(Head.Next);             //标注Token中的所有 Keywords
  return Head.Next;
}

char *readFile(char *Path){

  FILE *FP;
    //过渡方案
  if (strcmp(Path, "-") == 0) {
    FP = stdin;
  } else {
    FP = fopen(Path, "r");
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

Token *tokenizeFile(char *Path){
    return tokenize(Path,readFile(Path));
}