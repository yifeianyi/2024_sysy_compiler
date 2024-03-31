#include "common.hpp"
#include "tokenize.hpp"
using std::vector;
extern void codegen(TokenList* list ,FILE* Out);

static bool OptS;
static string OpTo;
static string InputPath;
void error(char const *Fmt, ...) {
  // 定义一个va_list变量
  va_list VA;
  va_start(VA, Fmt);
  fprintf(stderr, "\n");
  va_end(VA);
  exit(1);
}

static void parseArgs(int Argc, vector<string> &args){
    for(int i = 0; i < Argc-1; i++){
        if( args[i] == "--help"){
            exit(0);
        }  

        if( args[i] == "-o" ){
            if(i + 1 >= Argc){
                error("'-o' 选项后缺少参数");
            }
            OpTo = args[++i];
            continue;
        }
        
        if (args[i].rfind("-o", 0) == 0) { // 如果参数以 "-o" 开头
            OpTo = args[i].substr(2); // 获取 "-o" 之后的部分作为目标路径
            continue;
        }

        // 解析以 "-" 开头但不是上述参数的情况
        if (args[i].front() == '-' && args[i].size() > 1) {
            error("未知的参数: ", args[i]);
        }

        //暂时形同虚设
        if( args[i] == "-S"){
            OptS = true;
            continue;
        }

        InputPath = args[i];
    }
    
        // 检查是否指定了输入文件路径
    if (InputPath.empty()) {
        error("没有指定输入文件");
    }
}

static FILE *openFile(string &Path){
    //过渡方案
    if(Path.empty() || Path =="-"){
        return stdout;
    }

    FILE *Out = fopen(Path.c_str(), "w");
    if(!Out){
        error("cannot open output file: %s: %s", Path, strerror(errno));
    }

    return Out;
}

int main(int Argc, char **Argv){
    if (Argc < 2) {
        printf("%s\n",*Argv);
        // error("%s: invalid number of arguments", Argv[0]);
    }

    vector<string> args(Argv + 1,Argv + Argc);
    for(int i=1; i < Argc; i++) 
        args.push_back(string(Argv[i]));
    printf("test:%s\n",args[0].c_str());
    parseArgs(Argc, args);

    TokenList *list = tokenizeFile(InputPath.c_str());

    FILE *Out = openFile(OpTo);
    codegen(list, Out);

    return 0;
}