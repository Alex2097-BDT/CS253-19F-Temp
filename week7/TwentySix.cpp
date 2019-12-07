//
// Created by alex on 11/16/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){ return p1.second > p2.second;}
};

bool isAlpha(char);
char decap(char);
void extract_word(vector<string> &, char *);
void load_stop_words(vector<string> &);
void increment_count(map<string, int> &, string);

struct _all_words{
    vector<string> data;
} all_words;

struct _stop_words{
    vector<string> data;
} stop_words;

struct _unique_words{
    vector<string> data;
    function<void()> formula = [this](){
        for(auto w: all_words.data){
            auto pt = find(stop_words.data.begin(), stop_words.data.end(), w);
            if(pt==stop_words.data.end()){
                data.push_back(w);
            }
        }
    };
} unique_words;

struct _counts{
    map<string, int> data;
    function<void()> formula = [this](){
        for(auto w: unique_words.data){
            increment_count(data, w);
        }
    };
} counts;

struct _sorts{
    vector<pair<string, int>> data;
    function<void()> formula = [this](){
        vector<pair<string, int>> vec(counts.data.begin(), counts.data.end());
        sort(vec.begin(), vec.end(), cmp());
        data = vec;
    };
} sorts;

void update(){
    unique_words.formula();
    counts.formula();
    sorts.formula();
}

int main(int argc, char *argv[]){
    //practice 12.1
    extract_word(all_words.data, argv[1]);
    load_stop_words(stop_words.data);
    update();
    int min = 25;
    for(auto pt = sorts.data.begin();pt<sorts.data.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
    return 0;
}


bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap(char c){
    if(c<='Z'&&c>='A') return c+32;
    else return c;
}

void extract_word(vector<string> &obj, char *fileName){
    string str;
    ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);
    if(!ifs){
        cout<<"file not found."<<endl;
        return;
    }
    //read file
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    str = data;
    ifs.close();
    //segmentation
    std::string subStr;
    for(size_t i=0;i<str.length();i++) if(!isAlpha(str[i])){ if(!subStr.empty()){ obj.push_back(subStr); subStr.clear();}}else subStr.push_back(decap(str[i]));
    if(!subStr.empty()) obj.push_back(subStr);
    //filter out 's, 't, 'd
    string s = "s", t = "t", d = "d";
    for(auto pt = obj.begin();pt!=obj.end();pt++) if(strcmp(pt->c_str(), s.c_str())==0 || strcmp(pt->c_str(), t.c_str())==0 || strcmp(pt->c_str(), d.c_str())==0) pt = obj.erase(pt);
}

void load_stop_words(vector<string> &obj){
    string str;
    ifstream ifs;
    ifs.open("./stop_words.txt", std::ifstream::binary);
    if(!ifs) {
        cout<<"file not found."<<endl;
        return;
    }
    //read file
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    str = data;
    ifs.close();
    //segmentation
    std::string subStr;
    for(size_t i=0;i<str.length();i++){
        if(str[i]==','){
            if(!subStr.empty()){
                obj.push_back(subStr);
                subStr.clear();
            }
        }else subStr.push_back(decap(str[i]));
    }
    if(!subStr.empty()) obj.push_back(subStr);
    //remove the last element in obj
    auto it = obj.end()-1;
    it = obj.erase(it);
}

void increment_count(map<string, int> &obj, string word){
    auto pt = obj.find(word);
    if(pt==obj.end()) obj.insert(pair<string, int>(word, 1));
    else pt->second++;
}
