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
    virtual uint32_t getVal() = 0;
    void print(){
        printf("->%s",Name.c_str());
    }

    Token(TokenKind Kind,string Name){
        this->Kind = Kind;
        this->Name = Name;
    }
    void clear();
};
class DigitalTok: public Token{
    int val;
public:
    DigitalTok(TokenKind Kind,string Name,int value):Token(Kind,Name){
        this->val = value;
    }
    uint32_t getVal() override{
        return this->val;
    }
    
};

class OtherTok: public Token{
    public:
    ~OtherTok() {};
    OtherTok(TokenKind Kind,string Name):Token(Kind,Name){};
    uint32_t getVal(){
        printf("don't have val\n");
        return 0;
    }
};


//---------------------------------------------------------------------------------
class TokenList {
    

public:
    Token* head = nullptr;
    Token* tail = nullptr;
    Token* cur  = nullptr;
    ~TokenList() {
        clear();
    }
    void resetCur(){
        this->cur = head->Next;
    }
    string fetchName(const char*start,const char *end){
        int len = end - start;
        return string(start,len);
    }
    string fetchName(const char*start,int len){
        return string(start,len);
    }

    void clear() {
        while (head != nullptr) {
            Token* temp = head;
            head = head->Next;
            delete temp;
        }
    }

    void addNode(Token* Node) {
        if(this->head == nullptr){
            head = Node;
            tail = Node;
        }else{
            tail->Next = Node;
            tail = tail->Next;     
        }
    }

    void print() const {
        Token* Cur = head;
        while (Cur != nullptr) {
            Cur->print();
            Cur = Cur->Next;
        }
    }
};

TokenList *tokenizeFile(const char *Path);
char *readFile(const char *Path);

#endif