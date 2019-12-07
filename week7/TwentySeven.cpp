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
void load_stop_words(vector<string> &);
void increment_count(map<string, int> &, string);

struct _characters{
    string _data;
    function<void(char *)> init = [this](char *fileName){
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
        _data = data;
        ifs.close();
    };
} characters;

struct _all_words{
    vector<string> _data;
    function<void(char *)> init = [this](char *fileName){
        characters.init(fileName);
        std::string subStr;
        for(size_t i=0;i<characters._data.length();i++) if(!isAlpha(characters._data[i])){ if(!subStr.empty()){_data.push_back(subStr); subStr.clear();}}else subStr.push_back(decap(characters._data[i]));
        if(!subStr.empty()) _data.push_back(subStr);
    };
} all_words;

struct _non_stop_words{
    vector<string> _data;
    vector<string> _stop_words;
    function<void(char *)> init = [this](char *fileName){
        load_stop_words(_stop_words);
        all_words.init(fileName);
        for(auto w: all_words._data){
            auto pt = find(_stop_words.begin(), _stop_words.end(), w);
            if(pt==_stop_words.end()){
                _data.push_back(w);
            }
        }
    };
} non_stop_words;

struct _count_and_sort{
    vector<pair<string, int>> _data;
    map<string, int> _map_data;
    function<void(char *)> init = [this](char *fileName){
        non_stop_words.init(fileName);
        for(auto w: non_stop_words._data){
            increment_count(_map_data, w);
        }
        vector<pair<string, int>> vec(_map_data.begin(), _map_data.end());
        sort(vec.begin(), vec.end(), cmp());
        _data = vec;
    };
} count_and_sort;


int main(int argc, char* argv[])
{
    count_and_sort.init(argv[1]);
    int min = 25;
    for(auto pt = count_and_sort._data.begin();pt<count_and_sort._data.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
}

bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap(char c){
    if(c<='Z'&&c>='A') return c+32;
    else return c;
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
    obj.push_back("s");
    obj.push_back("t");
    obj.push_back("d");
}

void increment_count(map<string, int> &obj, string word){
    auto pt = obj.find(word);
    if(pt==obj.end()) obj.insert(pair<string, int>(word, 1));
    else pt->second++;
}