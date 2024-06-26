#ifndef __TOKENIZE_HPP__
#define __TOKENIZE_HPP__
#include<common.hpp>


typedef enum{
    TK_NUM,
    TK_KEYWORD,
    TK_IDENT,
    TK_PUNCT,
    TK_EOF,
}TokenKind;

class Token
{
public:
    Token *Next = nullptr;
    TokenKind Kind;
    string Name;
public:
    virtual ~Token() {};
    // virtual uint32_t getVal() = 0;
    virtual uint32_t getVal(){
        Assert(0,"Don't have val. TokName:%s.",this->Name.c_str());
        return 0;
    }
    void print(){
        printf("->%s",Name.c_str());
    }

    Token(TokenKind Kind,string Name){
        this->Kind = Kind;
        this->Name = Name;
    }
};
class DigitalTok: public Token{
    int val;
public:
    DigitalTok(TokenKind Kind,string Name,int value):Token(Kind,Name){
#ifdef __Debug_TokenList__        
        printf("In DigitalTok.\nTokenKind:%d TokenName:%s Val:%d\n",Kind,Name.c_str(),value);
#endif
        this->val = value;
    }
    uint32_t getVal() override{
        return this->val;
    }
};

//---------------------------------------------------------------------------------
class TokenList {
public:
    Token* head = nullptr;
    Token* tail = nullptr;
    // Token* cur  = nullptr;
    ~TokenList() {
        clear();
    }
    string fetchName(const char*start,const char *end);
    string fetchName(const char*start,int len);
    void clear();
    void addNode(Token* Node);
    void print() const;
};
TokenList *tokenizeFile(const char *Path);
char *readFile(const char *Path);

//=============================================================================


#endif