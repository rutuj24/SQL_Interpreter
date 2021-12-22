#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <cstring>
#include<sstream>

using namespace std;

void parse(string command);
void schema_parser();
void help_cmd();
void create_table();
void drop_table();
void describe_table();
void help_tables();
void insert_data();
void delete_data();

class key{
    public:
    string key_name;
    string type;
    string condition;
    bool primary = false;
    bool foreign = false;
    string reference_table;
    string reference_key;
};

class table{
    public:
    string table_name;
    vector<key> keys;
};

#define MAX_LEN 250
vector<string> cmd_array;
vector<table> schema;
int a=1;

int main()
{
    string command;
    cout << "           DBMS CP" << endl;
    schema_parser();
    while(a!=0){
        /*for(int i=0; i<schema.size(); i++){
            for(int j=0; j<schema[i].keys.size(); j++){
                cout<<schema[i].table_name<<"#"<<schema[i].keys[j].key_name<<"#"<<schema[i].keys[j].type;
                if(schema[i].keys[j].primary == true){
                    cout<<"#PK";
                }
                if(schema[i].keys[j].foreign == true){
                    cout<<"#FK "<<schema[i].keys[j].reference_table<<" "<<schema[i].keys[j].reference_key;
                }
                if(schema[i].keys[j].condition != ""){
                    cout<<"#"<<schema[i].keys[j].condition;
                }
                cout<<endl;
            }
        cout<<endl;
        }*/
        cout << ">>Enter command" << endl;
        getline (cin, command);
        parse(command);
        }
    return 0;
}

void parse(string command){
    cmd_array.clear();
    int check;
    stringstream ss(command);
    string word;
    while (ss >> word) {
        int n = word.length();
        check=1;
        char char_array[n + 1];
        strcpy(char_array, word.c_str());
        for(int j=0; j<n; j++){
            if(char_array[j]=='"'){
                check=0;
            }
        }
        if(check==1){
            transform(word.begin(), word.end(), word.begin(), ::toupper);
        }
        //cout<<word;
        cmd_array.push_back(word);
    }

//    transform(cmd_array[0].begin(), cmd_array[0].end(), cmd_array[0].begin(), ::toupper);
    if(cmd_array[0] == "CREATE"){
        create_table();
    }
    else if(cmd_array[0] == "DROP"){
        drop_table();
    }
    else if(cmd_array[0] == "DESCRIBE"){
        describe_table();
    }
    else if(cmd_array[0] == "INSERT"){
        insert_data();
    }
    else if(cmd_array[0] == "DELETE"){
        delete_data();
    }
    else if(cmd_array[0] == "UPDATE"){
        cout<<"Update data function"<<endl;
    }
    else if(cmd_array[0] == "SELECT"){
        cout<<"Select data function"<<endl;
    }
    else if(cmd_array[0] == "HELP" && cmd_array[1] == "TABLES"){
        help_tables();
    }
    else if(cmd_array[0] == "HELP"){
        help_cmd();
    }
    else if(cmd_array[0] == "QUIT"){
        cout<<">>Closing program"<<endl;
        a=0;
    }
    else{
        cout<<">>Invalid command"<<endl;
    }
}

void help_cmd(){
    if(cmd_array[1] == "CREATE"){
        cout<<">>CREATE TABLE table_name ( attribute_1 attribute1_type CHECK (constraint1),"<<endl;
        cout<<">>attribute_2 attribute2_type, …, PRIMARY KEY ( attribute_1, attribute_2 ),"<<endl;
        cout<<">>FOREIGN KEY ( attribute_y ) REFERENCES table_x ( attribute_t ), FOREIGN"<<endl;
        cout<<">>KEY ( attribute_w ) REFERENCES table_y ( attribute_z )… );"<<endl;
    }
    else if(cmd_array[1] == "DROP"){
        cout<<">>DROP TABLE table_name;"<<endl;
    }
    else if(cmd_array[1] == "SELECT"){
        cout<<">>SELECT attribute_list FROM table_list WHERE condition_list;"<<endl;
    }
    else if(cmd_array[1] == "INSERT"){
        cout<<">>INSERT INTO table_name VALUES ( INT, STRING, DEC … );"<<endl;
    }
    else if(cmd_array[1] == "DELETE"){
        cout<<">>DELETE FROM table_name WHERE condition_list;"<<endl;
    }
    else if(cmd_array[1] == "UPDATE"){
        cout<<">>UPDATE table_name SET attr1 = val1, attr2 = val2… WHERE condition_list;"<<endl;
    }
    else{
        cout<<">>Command you are searching for doesn't exist"<<endl;
    }
}

void create_table(){
    table tab;
    tab.table_name = cmd_array[2];
    string filename = tab.table_name + ".csv";
    fstream myfile;
    for(int i=0; i<schema.size(); i++){
        if(tab.table_name == schema[i].table_name){
            cout<<">>Table already created"<<endl;
            return;
        }
    }
    int i=3;
    while(cmd_array[i] != ");"){
        key k;
        if(cmd_array[i] == "(" || cmd_array[i] == ")"){
            i++;
        }
        else if(cmd_array[i] == "PRIMARY"){
            i = i+3;
            while(cmd_array[i] != ")," && cmd_array[i] != ")"){
                for(int j=0; j<tab.keys.size(); j++){
                    if(cmd_array[i] == tab.keys[j].key_name){
                        tab.keys[j].primary = true;
                        break;
                    }
                }
                i++;
            }
            i++;
        }
        else if(cmd_array[i] == "FOREIGN"){
            int index;
            i = i+3;
            for(int j=0; j<tab.keys.size(); j++){
                if(cmd_array[i] == tab.keys[j].key_name){
                    tab.keys[j].foreign = true;
                    index=j;
                    break;
                }
            }

            i = i+3;
            int foreign_check=0, foreign_index;
            for(int k=0; k<schema.size(); k++){
                if(cmd_array[i] == schema[k].table_name){
                    foreign_check=1;
                    foreign_index=k;
                }
            }
            if(foreign_check==0){
                cout<<">>Foreign key reference table error\n";
                return;
            }
            tab.keys[index].reference_table = cmd_array[i];

            i = i+2;
            foreign_check=0;
            for(int k=0; k<schema[foreign_index].keys.size(); k++){
                if(cmd_array[i] == schema[foreign_index].keys[k].key_name && tab.keys[index].type==schema[foreign_index].keys[k].type){
                    foreign_check=1;
                }
            }
            if(foreign_check==0){
                cout<<">>Foreign key reference key error\n";
                return;
            }
            tab.keys[index].reference_key = cmd_array[i];
            i++;
        }
        else{
            k.key_name = cmd_array[i];
            i++;
            if(cmd_array[i].substr(cmd_array[i].length()-1) == ","){
                k.type = cmd_array[i].substr(0,cmd_array[i].length()-1);
                if(k.type != "INT" && k.type != "CHAR" && k.type != "DEC"){
                    cout<<">>Invalid attribute type\n";
                    return;
                }
                i++;
                tab.keys.push_back(k);
                continue;
            }
            else{
                k.type = cmd_array[i];
                if(k.type != "INT" && k.type != "CHAR" && k.type != "DEC"){
                    cout<<">>Invalid attribute type\n";
                    return;
                }
                i++;
                if(cmd_array[i] == ");"){
                    tab.keys.push_back(k);
                    break;
                }
                else{
                    i++;
                    if(cmd_array[i+1] == ");"){
                        k.condition = cmd_array[i].substr(1,cmd_array[i].length()-2);
                    }
                    else{
                        k.condition = cmd_array[i].substr(1,cmd_array[i].length()-3);
                    }
                    i++;
                    if(k.condition != "NOT_NULL" && k.condition != "UNIQUE"){
                        cout<<">>Invalid condition for key\n";
                        return;
                    }
                    tab.keys.push_back(k);
                }
            }
        }
    }
    myfile.open("schema.txt", ios::out|ios::app);
    if( !myfile.is_open())
    {
          cout << ">>File Error : Table not created \n";
          return;
    }

    myfile<<tab.table_name<<endl;
    for(int i=0; i<tab.keys.size(); i++){
        myfile<<tab.keys[i].key_name<<"--"<<tab.keys[i].type;
        if(tab.keys[i].primary){
            myfile<<"--PK";
        }
        else{
            myfile<<"-- ";
        }

        if(tab.keys[i].foreign){
            myfile<<"--FK "<<tab.keys[i].reference_table<<" "<<tab.keys[i].reference_key;
        }
        else{
            myfile<<"-- ";
        }

        if(tab.keys[i].condition != ""){
            myfile<<"--"<<tab.keys[i].condition;
        }
        else{
            myfile<<"-- ";
        }
        myfile<<"--"<<endl;
    }
    myfile<<endl;
    myfile.close();
    string fname = tab.table_name + ".csv";
    myfile.open(fname.c_str(), ios::out);
    for(int i=0; i<tab.keys.size(); i++){
        myfile<<tab.keys[i].key_name<<",";
    }
    myfile<<"\n";
    myfile.close();
    cout<<">>Table created successfully"<<endl;
    schema.push_back(tab);
}

void drop_table(){
    int a=0,check=0;
    string line;
    if(cmd_array.size()<3){
        cout<<">>Syntax Error\n";
        return;
    }
    ifstream in;
    in.open("schema.txt");
    ofstream out;
    out.open("temporary.txt");
    if( !in.is_open() || !out.is_open())
    {
          cout << ">>File Error : Table not dropped \n";
          return;
    }
    while( getline(in,line) )
    {
        if(line == cmd_array[2] || a==1){
            a=1;check=1;
            if(line == ""){
                a=0;
            }
        }
        else{
            out<<line<<"\n";
        }
    }
    line = cmd_array[2] + ".csv";
    if(check==0){
        cout<<">>Table not found"<<endl;
    }
    else{
        remove(line.c_str());
        cout<<">>Table removed successfully"<<endl;
        /*for (auto it = schema.begin(); it != schema.end(); ++it)
            if(*it.table_name == cmd_array[2]){
                schema.erase(it);
            }
        }*/
        for(int i=0; i<schema.size(); i++){
            if(schema[i].table_name == cmd_array[2]){
                schema.erase(schema.begin()+i);
            }
        }
    }
    in.close();
    out.close();
    remove("schema.txt");
    rename("temporary.txt", "schema.txt");
}

void describe_table(){
    int check=0;
    for(int i=0; i<schema.size(); i++){
        if(cmd_array[1] == schema[i].table_name){
            check=1;
            cout<<">>"<<cmd_array[1]<<endl;
            for(int j=0; j<schema[i].keys.size(); j++){
                cout<<">>"<<schema[i].keys[j].key_name;
                cout<<"--"<<schema[i].keys[j].type;
                if(schema[i].keys[j].primary == true){
                    cout<<"--Primary Key";
                }
                if(schema[i].keys[j].foreign == true){
                    cout<<"--Foreign Key   References   "<<schema[i].keys[j].reference_table<<"("<<schema[i].keys[j].reference_key<<")";
                }
                if(schema[i].keys[j].condition != ""){
                    cout<<"--"<<schema[i].keys[j].condition<<endl;
                }
                cout<<endl;
            }
        }
    }
}

void help_tables(){
    for(int i=0; i<schema.size(); i++)
    {
        cout<<schema[i].table_name;
        cout<<endl;
    }
    cout<<">>"<<endl;
}

void schema_parser(){
    int a=0;
    int check=0;
    string line,Line;
    ifstream in;
    table table_temp;
    in.open("schema.txt");
    if( !in.is_open())
    {
          cout << ">>File Error \n";
          return;
    }
    while( getline(in,line) )
    {
        if(check==1){
            a=1;
            check=0;
        }
        if(a==0){
            table_temp.table_name = line;
            check=1;
        }
        if(a==1){
            key k;
            if(line == ""){
                a=0;
                schema.push_back(table_temp);
                table_temp.keys.clear();
                continue;
            }
            int start=0;
            int last = line.find("--");
            Line = line.substr(start, last);
            k.key_name = Line;
            start = last + 2;
            last = line.find("--", start);
            Line = line.substr(start, last-start);
            k.type = Line;
            start = last + 2;
            last = line.find("--", start);
            Line = line.substr(start, last-start);
            if(Line == "PK"){
                k.primary = true;
            }
            start = last + 2;
            last = line.find("--", start);
            Line = line.substr(start, last-start);
            if(Line != " "){
                istringstream ss(Line);
                string word;
                vector<string> words;
                while (ss >> word)
                {
                    words.push_back(word);
                }
                if(words[0]=="FK"){
                    k.foreign = true;
                    k.reference_table = words[1];
                    k.reference_key = words[2];
                }
            }
            start = last + 2;
            last = line.find("--", start);
            Line = line.substr(start, last-start);
            if(Line != " "){
                k.condition = Line;
            }
            table_temp.keys.push_back(k);
        }
    }
    /*for(int i=0; i<schema.size(); i++){
        for(int j=0; j<schema[i].keys.size(); j++){
            cout<<schema[i].table_name<<"#"<<schema[i].keys[j].key_name<<"#"<<schema[i].keys[j].type<<"\n";
        }
        cout<<endl;
    }
    cout<<">>"<<endl;*/
}

void insert_data(){
    int i,index,check=0,length=0;
    for(i=0; i<schema.size(); i++){
        if(cmd_array[2] == schema[i].table_name){
            index=i;
            check=1;
            break;
        }
    }
    if(check==0){
        cout<<">>Table does not exist"<<endl;
        return;
    }
    length = schema[index].keys.size();
    i=5;
    vector<string> values;
    while(cmd_array[i] != ");"){
        if(cmd_array[i+1] == ");"){
            values.push_back(cmd_array[i]);
        }
        else{
            values.push_back(cmd_array[i].substr(0, cmd_array[i].length()-1));
        }
        i++;
    }
    /*cout<<values.size();
    for(int i=0; i<values.size(); i++){
        cout<<values[i];
    }*/
    if(values.size()!=length){
        cout<<">>No. of arguments don't match"<<endl;           //CHECKING LENGTH
        return;
    }

    for(int i=0; i<length; i++){                                //CHECKING VALUES
        if(schema[index].keys[i].type == "INT"){
            int n = values[i].length();
            char char_array[n + 1];
            strcpy(char_array, values[i].c_str());
            for(int j=0; j<n; j++){
                if(!isdigit(char_array[j]) || char_array[j]=='"'){
                    cout<<">>Value type error\n";
                    return;
                }
            }
        }
        if(schema[index].keys[i].type == "DEC"){
            int n = values[i].length();
            char char_array[n + 1];
            strcpy(char_array, values[i].c_str());
            for(int j=0; j<n; j++){
                if(!(isdigit(char_array[j])) && char_array[j]!='.'  || char_array[j]=='"'){
                    cout<<">>Value type error\n";
                    return;
                }
            }
        }

        if(schema[index].keys[i].type == "CHAR"){
            int n = values[i].length(), x=0;
            char char_array[n + 1];
            strcpy(char_array, values[i].c_str());
            for(int j=0; j<n; j++){
                if(char_array[j]=='"'){
                    x++;
                }
            }
            //cout<<x;
            if(x!=2){
                cout<<">>Value type error\n";
                return;
            }
        }

        if(schema[index].keys[i].primary == true || schema[index].keys[i].condition == "UNIQUE"){
            ifstream in;
            string tab_nam = schema[index].table_name+".csv";
            in.open(tab_nam.c_str());
            vector<string> vals;
            string line;
            while( getline(in,line) ){
                stringstream ss(line);
                string word;
                vals.clear();
                while (getline(ss, word, ',')){
                    vals.push_back(word);
                }
                //cout<<vals[i]<<endl<<values[i];
                if(values[i] == vals[i]){
                    cout<<">>Primary Key or Constraint error\n";
                    in.close();
                    return;
                }
            }
            in.close();
        }

        if(schema[index].keys[i].foreign == true){
            string foreign_tab = schema[index].keys[i].reference_table;
            string foreign_k = schema[index].keys[i].reference_key;
            int foreign_id, foreign_key_id;
            for(int j=0; j<schema.size(); j++){
                if(schema[j].table_name == foreign_tab){
                    foreign_id = j;
                    break;
                }
            }
            for(int j=0; j<schema[foreign_id].keys.size(); j++){
                if(schema[foreign_id].keys[j].key_name == foreign_k){
                    foreign_key_id = j;
                    break;
                }
            }
            foreign_tab = foreign_tab + ".csv";
            cout<<foreign_tab;
            ifstream in;
            int check_for_foreign=0;
            in.open(foreign_tab.c_str());
            vector<string> vals;
            string line;
            while( getline(in,line) ){
                stringstream ss(line);
                string word;
                vals.clear();
                while (getline(ss, word, ',')){
                    vals.push_back(word);
                }
                //cout<<vals[foreign_key_id]<<endl<<values[i];
                if(values[i] == vals[foreign_key_id]){
                    check_for_foreign=1;
                    break;
                }
            }
            if(check_for_foreign==0){
                cout<<">>Foreign key error\n";
                return;
            }
            in.close();
        }

        if(schema[index].keys[i].condition == "NOT_NULL"){
            if(values[i] == "0" || values[i] == ""){
                cout<<">>Constraint error\n";
                return;
            }
        }
    }
    fstream myfile;
    string fname = schema[index].table_name + ".csv";
    myfile.open(fname.c_str(), ios::out|ios::app);
    for(i=0; i<values.size(); i++){
        myfile<<values[i]<<",";
    }
    myfile<<"\n";
    cout<<">>Data added successfully\n";
    myfile.close();
}

void delete_data(){
    int tab_index, check=0;
    for(int j=0; j<schema.size(); j++){
        if(schema[j].table_name==cmd_array[2]){
            tab_index = j;
            check=1;
            break;
        }
    }
    if(check==0){
        cout<<"Table not found\n";
        return;
    }

    vector<string> command;
    int i=4;
    while(cmd_array[i] != ";"){

    }
}

//CREATE TABLE a1 ( a INT, b INT CHECK (UNIQUE), PRIMARY KEY ( a ) );
//CREATE TABLE a2 ( c int, d int CHECK (N_NULL), PRIMARY KEY ( c ), FOREIGN KEY ( d ) REFERENCES a1 ( a ) );
//INSERT INTO a1 ( 2223, 4356 );
