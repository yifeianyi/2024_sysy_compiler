#include<common.hpp>
/*

    CompUnit    = FuncDef
    FuncDef     = FuncType Ident '(' [FuncFParams] ')' Block
    FuncType    = 'void' | 'int' | 'float'
    Block       = '{' { BlockItem } '}' 
    --------------------------------------------------------------------
    BlockItem   = Stmt
    Stmt        = 'return' [Exp] ';'

*/ 
