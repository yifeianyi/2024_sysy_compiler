#include<tokenize.hpp>

string TokenList::fetchName(const char*start,const char *end){
    int len = end - start;
    return string(start,len);
}
string TokenList::fetchName(const char*start,int len){
    return string(start,len);
}

void TokenList::clear() {
    while (head != nullptr) {
        Token* temp = head;
        head = head->Next;
        delete temp;
    }
}

void TokenList::addNode(Token* Node) {
    if(this->head == nullptr){
        head = Node;
        tail = Node;
    }else{
        tail->Next = Node;
        tail = tail->Next;     
    }
}

void TokenList::print() const {
    Token* Cur = head;
    while (Cur != nullptr) {
        // if(Cur-Kind != TK_NUM)
        Cur->print();
        if(Cur->Kind == TK_NUM){
            printf( "(Val:%d) ",Cur->getVal());
        }
        Cur = Cur->Next;
    }
    printf("\n");
}