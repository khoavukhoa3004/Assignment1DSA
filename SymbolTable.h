#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"


class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
    ~SymbolTable(){
        
    }
};

class NAME{
public:
    string name;
    NAME(){
        name= "";
    }
    NAME(string name){
        this->name = name;
    }
    bool check(){
        if(name[0]<'a'|| name[0]>'z')
            return false;
        for(int i = 1; i < int(name.size()); i++){
            if((name[i]>='a'&& name[i]<='z')|| (name[i]>='A'&& name[i]<='Z')|| name[i]=='_' || (name[i]>='0'&& name[i]<='9'))
                continue;
            else
                return false;
        }
        return true;
    }
    ~NAME(){
        name ="";
    }
};

class NUMBER{
public:
    bool declade;
    NAME name_num;
    string type = "number";
    int data;
    ~NUMBER(){
        declade = false;
        type ="";
        data = 0;
    }
    
};
class STRING {
public:
    bool declade;
    NAME name_str;
    string type = "string";
    string data;
    ~STRING(){
        declade = false;
        type ="";
        data = "";
    }

};



class manage_type{
public:
    NUMBER num;
    STRING str;
    int level;
    manage_type *next;
    manage_type *prev;
    ~manage_type(){
        level = 0;
        next = NULL;
        prev = NULL;
    }
};
class MANAGE{
public:
    int level;
    manage_type *head;
    manage_type *tail;
    MANAGE(){
        level = 0;
        head = NULL;
        tail = NULL;
    }
    ~MANAGE(){
        level = 0;
        head = NULL;
        tail = NULL;
    }
};


#endif