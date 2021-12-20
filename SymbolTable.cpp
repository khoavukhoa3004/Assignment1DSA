#include "SymbolTable.h"

bool split_line(string s, string *ar){
    ar[0] ="";
    ar[1] ="";
    ar[2] ="";
    if(s=="")
        return false;
    int pre_count = 0;
    for(int j = 0; j < 2; j++){
        for(int i = pre_count; i < int(s.size()); i++){
            if(s[i]==' ' || i == int(s.size())-1){
                if(i == int(s.size())-1){
                    ar[j] = s.substr(pre_count, i+1);
                    return true;
                }
                else{
                    ar[j] = s.substr(pre_count, i - pre_count);
                    pre_count = i + 1;
                    break;
                }
            }
        } 
    }
    ar[2] = s.substr(pre_count,int(s.size()) - pre_count);
    return true;
}

void InsertAt(MANAGE &table, NAME name, string type){
    manage_type *temp = new manage_type();
    temp->level = table.level;
    if(type=="number"){
        temp->num.declade = true;
        temp->str.declade = false;
        temp->num.name_num = name.name;
    }     
    else{
        temp->str.declade = true;
        temp->num.declade = false;
        temp->str.name_str = name.name;
    }
    
    if(table.tail ==NULL){
        table.head = temp;
        table.tail = temp;
    }
    else{
        table.tail->next = temp;
        temp->prev = table.tail;
        table.tail = table.tail->next;
    }
        
}

bool ConvertToInt(string s, int &ans){
    ans = 0;
    bool positive =true;
    if(s[0]=='-')
        positive = false;
    if(s[0]=='+' || s[0]=='-'){
        for(int i = int(s.size())-1; i >=1; i--){
            if(s[i]>='0' && s[i]<='9'){
                continue;
            }
            else return false;
                
        }

        ans = stoi(s.substr(1, int(s.size())-1));
    }
    else if(s[0]>='0' && s[0]<='9'){
        for(int i = s.size()-1; i>=1; i--){
            if(s[i]>='0' && s[i]<='9'){
                continue;
            }
            else return false;           
        }
        ans = stoi(s);
        if(!positive)
            ans = - ans;
    }
    else 
        return false;
    return true;
}
bool CheckString(string s, string &ans){
    if(s=="''"){
        ans = s;
        return true;
    }
    if(s[0]!='\'' || s[int(s.size())-1]!='\'')
        return false;
    if(int(s.size()) < 3)
        return false;
    for(int i = 1; i < int(s.size())-1; i++){
        if((s[i]>='0'&&s[i]<='9') || (s[i]>='a'&&s[i]<='z')||(s[i]>='A'&&s[i]<='Z')||s[i]==' ')
            continue;
        else 
            return false;
    }
    ans = s;
    return true;
}
int FindAt(MANAGE &table, string *s){
    // return 1: success
    // return 0: typeMismatch
    // return -1: Undeclared
    // s[0]: variable that need value;
    // s[1]: value that can be used as: string value, number value or variable

    // First Check if *s have name correctly or have string's type correctly. If not then return -2;
    // if return -2 then it have InvalidInstruction
    NAME a(s[0]);
    if(!a.check())
        return -2;
    string ans ="";
    NAME b(s[1]);
    if(!b.check()){
        if(!CheckString(b.name,ans)){
            int t;
            if(!ConvertToInt(b.name,t)){
                return -2;
            }
        }
        
    } 
    manage_type *temp = table.tail;
    manage_type *s0 = NULL; // variable of s[0]
    while(temp){
        if(!s0){
            if(temp->num.declade){
                if(temp->num.name_num.name == s[0]){
                    s0 = temp;
                }
            }
            else if(temp->str.declade){
                if(temp->str.name_str.name == s[0]){
                    s0 = temp;
                }
            }
        }
        else break;
        temp = temp->prev;
    }
    if(!s0) 
        return -1;
    if(s0->num.declade){
        if(ConvertToInt(s[1],s0->num.data)){
            return 1;
        }
        else if(CheckString(s[1],s0->str.data)){
            return 0;
        }
        else{
            manage_type *t= table.tail;
            while(t){
                if(t->num.declade){
                    if(t->num.name_num.name == s[1]){
                        return 1;
                    }
                }
                else if(t->str.declade){
                    if(t->str.name_str.name == s[1]){
                        return 0;
                    }
                }
                t = t->prev;
            }
        }
    }
    else if(s0->str.declade){
        if(ConvertToInt(s[1],s0->num.data)){
            return 0;
        }
        else if(CheckString(s[1],s0->str.data)){
            return 1;
        }
        else{
            manage_type *t= table.tail;
            while(t){
                if(t->num.declade){
                    if(t->num.name_num.name == s[1]){
                        return 0;
                    }
                }
                else if(t->str.declade){
                    if(t->str.name_str.name == s[1]){
                        return 1;
                    }
                }
                t = t->prev;
            }
        }
    }
    return -1;

}
bool CheckInsertAt(MANAGE &table, NAME &object){
    if(!table.tail)
        return true;
    manage_type *temp = table.tail;
    while(temp){
        if(temp->num.declade){
            if(temp->num.name_num.name == object.name){
                if(temp->level== table.level)
                    return false;
            }
        }
        else if(temp->str.declade){
            if(temp->str.name_str.name == object.name){
                if(temp->level == table.level)
                    return false;   
            }

        }
        temp = temp->prev;
        
    }
    return true;
}

void ClearAllTable(MANAGE &table){
    while(table.head){
        manage_type *temp = table.head;
        table.head = table.head->next;
        if(table.head)
            table.head->prev = NULL;
        temp->next = NULL;
        delete temp;
    }
    table.tail = NULL;
}
bool LookUp(MANAGE &table, string s, string &lineoffile){
    NAME t(s);
    if(!t.check()){
        ClearAllTable(table);
        throw InvalidInstruction(lineoffile);
    }
    if(!table.tail)
        return false;
    manage_type *temp = table.tail;
    while(temp){
        if(temp->num.declade){
            if(temp->num.name_num.name ==s){
                cout<<temp->level<<endl;
                return true;
            }
        }
        else{
            if(temp->str.name_str.name == s){
            cout<<temp->level<<endl;
            return true;
            }
        }
        temp = temp->prev;
    }
    return false;
}
void PRINT(MANAGE &table){
    string ans ="";
    bool InitSpace = false;
    manage_type *temp = table.head;
    bool repeat = false;
    while(temp){
        manage_type *t = table.tail;
        while(temp!=t){
            if(temp->num.declade){
                if(t->num.declade){
                    if(temp->num.name_num.name == t->num.name_num.name){
                        repeat = true;
                        break;
                    }
                }
                else{
                    if(temp->num.name_num.name == t->str.name_str.name){
                        repeat = true;
                        break;
                    }
                }
            }
            else{
                if(t->num.declade){
                    if(temp->str.name_str.name == t->num.name_num.name){
                        repeat = true;
                        break;
                    }
                }
                else{
                    if(temp->str.name_str.name == t->str.name_str.name){
                        repeat = true;
                        break;
                    }
                }                
            }
            t = t->prev;
        }
        if(!repeat){
            if(!InitSpace){
                if(temp->num.declade){
                    ans = ans + (temp->num.name_num.name+"//"+to_string(temp->level));
                }
                else{
                    ans = ans +(temp->str.name_str.name+"//"+to_string(temp->level));
                }
                InitSpace = true;
            }
            else{
                if(temp->num.declade){
                    ans = ans +" "+ (temp->num.name_num.name+"//"+to_string(temp->level));
                }
                else{
                    ans = ans +" "+ (temp->str.name_str.name+"//"+to_string(temp->level));
                }
            }
        }
        else{
            repeat = false;
        }
        temp = temp->next;
    }
    if(ans!="")
        cout<<ans<<endl;
}
void RPRINT(MANAGE &table){
    string ans ="";
    bool InitSpace = false;
    manage_type *temp = table.tail;
    bool repeat = false;
    while(temp){
        manage_type *t = table.tail;
        while(temp!=t){
            if(temp->num.declade){
                if(t->num.declade){
                    if(temp->num.name_num.name == t->num.name_num.name){
                        repeat = true;
                        break;
                    }
                }
                else{
                    if(temp->num.name_num.name == t->str.name_str.name){
                        repeat = true;
                        break;
                    }
                }
            }
            else{
                if(t->num.declade){
                    if(temp->str.name_str.name == t->num.name_num.name){
                        repeat = true;
                        break;
                    }
                }
                else{
                    if(temp->str.name_str.name == t->str.name_str.name){
                        repeat = true;
                        break;
                    }
                }                
            }
            t = t->prev;
        }
        if(!repeat){
            if(!InitSpace){
                if(temp->num.declade){
                    ans = ans + (temp->num.name_num.name+"//"+to_string(temp->level));
                }
                else{
                    ans = ans +(temp->str.name_str.name+"//"+to_string(temp->level));
                }
                InitSpace = true;
            }
            else{
                if(temp->num.declade){
                    ans = ans +" "+ (temp->num.name_num.name+"//"+to_string(temp->level));
                }
                else{
                    ans = ans +" "+ (temp->str.name_str.name+"//"+to_string(temp->level));
                }
            }
        }
        else{
            repeat = false;
        }
        temp = temp->prev;
    }
    if(ans!="")
        cout<<ans<<endl;
}


bool CheckStatement(MANAGE &table, string &GetLineofFile, string *split){
    if(split[0]=="INSERT"){
        if(split[1]=="" || split[2]=="")
            return false;
        if(split[2]!= "number" && split[2]!="string")
            return false;
        NAME object(split[1]);
        if(!object.check())
            return false;
        if(!CheckInsertAt(table,object)){
            // throw Redeclared
            ClearAllTable(table);
            throw Redeclared(GetLineofFile);
        }
        else{
            InsertAt(table,object,split[2]);
            cout<<"success"<<endl;
        }
    }
    else if(split[0]=="ASSIGN"){
        if(split[1]=="" || split[2]=="")
            return false;
        int ans =FindAt(table, split+1);
        if(ans == 0){
            // throw TypeMismatch
            ClearAllTable(table);
            throw TypeMismatch(GetLineofFile);
        }
        else if(ans == -1){
            ClearAllTable(table);
            throw Undeclared(GetLineofFile);
        }
        else if(ans ==-2){
            ClearAllTable(table);
            throw InvalidInstruction(GetLineofFile);
        }
        else 
            cout<<"success"<<endl;
    }
    else if(split[0]=="BEGIN" || split[0]=="END"){
        if(split[1]!=""||split[2]!="")
            return false;
        if(split[0] =="BEGIN"){
            table.level++;
        }
        else{
            if(table.level == 0){
                ClearAllTable(table);
                throw UnknownBlock();
            }
            while(table.tail){
                if(table.tail->level == table.level){
                    manage_type *clear = table.tail;
                    table.tail = table.tail->prev;
                    if(table.tail)
                        table.tail->next = NULL;
                    clear->prev = NULL;
                    delete clear;
                }
                else
                    break;
            }
            if(!table.tail)
                table.head = NULL;
            table.level--;
        }
    }
    else if(split[0]=="LOOKUP"){
        if(split[1]=="" || split[2]!=""){
            ClearAllTable(table);
            throw InvalidInstruction(GetLineofFile);
        }
        if(!LookUp(table,split[1], GetLineofFile)){
            ClearAllTable(table);
            throw Undeclared(GetLineofFile);
        }
    }
    else if(split[0]=="PRINT"){ 
        if(split[1]!="" || split[2]!=""){
            ClearAllTable(table);
            throw InvalidInstruction(GetLineofFile);
        }
        PRINT(table);
    }
    else if(split[0]=="RPRINT"){
        if(split[1]!="" || split[2]!=""){
            ClearAllTable(table);
            throw InvalidInstruction(GetLineofFile);
        }
        RPRINT(table);
    }
    else return false;
    return true;
}

void SymbolTable::run(string filename) {
    // Quan ly level block
    MANAGE table;
    table.head = NULL;
    table.tail = NULL;
    table.level = 0;
    //
    ifstream ifs(filename);
    string GetLineofFile;
    string split[3];
    while(ifs.good()){
        getline(ifs,GetLineofFile);
        if(!split_line(GetLineofFile,split)){
            // throw InvalidInstruction
            throw InvalidInstruction(GetLineofFile);
        }
        if(!CheckStatement(table, GetLineofFile, split)){
            ClearAllTable(table);
            throw InvalidInstruction(GetLineofFile);
        }
    }
    ifs.close();
    if(table.level != 0){
        ClearAllTable(table);
        throw UnclosedBlock(table.level);
    }
    else
        ClearAllTable(table);
    
}