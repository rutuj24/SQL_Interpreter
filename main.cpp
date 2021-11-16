#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <cstring>


using namespace std;

void parse(string command);
void help_cmd();
void create_table();
void drop_table();
void describe_table();
void help_tables();

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
    cout << "SQL INTERPRETER" << endl;
    while(a!=0){
        cout << ">>Enter command" << endl;
        getline (cin, command);
        parse(command);
        }
    return 0;
}

void parse(string command){
    cmd_array.clear();
    stringstream ss(command);
    string word;
    while (ss >> word) {
        cmd_array.push_back(word);
    }

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
        cout<<"Insert data function"<<endl;
    }
    else if(cmd_array[0] == "DELETE"){
        cout<<"Delete data function"<<endl;
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
        cout<<">>INSERT INTO table_name VALUES ( val1, val2, … );"<<endl;
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
            tab.keys[index].reference_table = cmd_array[i];
            i = i+2;
            tab.keys[index].reference_key = cmd_array[i];
            i++;
        }
        else{
            k.key_name = cmd_array[i];
            i++;
            if(cmd_array[i].substr(cmd_array[i].length()-1) == ","){
                k.type = cmd_array[i].substr(0,cmd_array[i].length()-1);
                i++;
                tab.keys.push_back(k);
                continue;
            }
            else{
                k.type = cmd_array[i];
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
                    tab.keys.push_back(k);
                }
            }
        }
    }

    fstream myfile;
    myfile.open("schema.txt", ios::out|ios::app);
    if( !myfile.is_open())
    {
          cout << "File Error : Table not created \n";
          return;
    }

    myfile<<tab.table_name<<endl;
    for(int i=0; i<tab.keys.size(); i++){
        myfile<<tab.keys[i].key_name<<"--"<<tab.keys[i].type;
        if(tab.keys[i].primary){
            myfile<<"--primary key";
        }
        if(tab.keys[i].foreign){
            myfile<<"--foreign key references "<<tab.keys[i].reference_table<<"("<<tab.keys[i].reference_key<<")";
        }
        if(tab.keys[i].condition != ""){
            myfile<<"--"<<tab.keys[i].condition;
        }
        myfile<<endl;
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
}

void drop_table(){
    int a=0,check=0;
    string line;
    ifstream in;
    in.open("schema.txt");
    ofstream out;
    out.open("temporary.txt");
    if( !in.is_open() || !out.is_open())
    {
          cout << "File Error : Table not dropped \n";
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
    }
    in.close();
    out.close();
    remove("schema.txt");
    rename("temporary.txt", "schema.txt");
}

void describe_table(){
    int a=0,check=0;
    string line;
    ifstream in;
    in.open("schema.txt");
    if( !in.is_open())
    {
          cout << "File Error\n";
          return;
    }
    while( getline(in,line) )
    {
        if(line == cmd_array[1] || a==1){
            a=1;check=1;
            cout<<">>"<<line<<"\n";
            if(line == ""){
                a=0;
            }
        }
    }
    if(check==0){
        cout<<">>"<<"Table not found"<<endl;
    }
    in.close();
}

void help_tables(){
    int a=0;
    string line, t_name;;
    ifstream in;
    in.open("schema.txt");
    cout<<">>"<<endl;
    if( !in.is_open())
    {
          cout << "File Error \n";
          return;
    }
    if(in.peek() == EOF){
        cout<<"No tables found \n";
        return;
    }
    while( getline(in,line) )
    {
        if(a==1){
            if(line == ""){
                a=0;
                continue;
            }
            int start=0;
            int last = line.find("--");
            line = line.substr(start, last);
            cout<<t_name<<"#"<<line<<"\n";
        }
        if(a==0){
            t_name = line;
            cout<<endl;
            a=1;
        }
    }
    cout<<endl<<">>"<<endl;
    in.close();
}
