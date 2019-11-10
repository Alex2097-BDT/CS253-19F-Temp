//
// Created by alex on 11/9/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){
        return p1.second > p2.second;
    }
};

bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap(char c){
    if(c<='Z'&&c>='A'){
        return c+32;
    }else return c;
}

string get_book_content(char *fileName){
    string ret;
    ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);
    if(!ifs) return NULL;
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    ret = data;
    ifs.close();
    return ret;
}

struct Stop_words{
    Stop_words(){
        string str = get_book_content("./stop_words.txt");
        std::string subStr;
        for(size_t i=0;i<str.length();i++){
            if(str[i]==','){
                if(!subStr.empty()){
                    stop_word_list.push_back(subStr);
                    subStr.clear();
                }
            }else{
                subStr.push_back(decap(str[i]));
            }
        }
        if(!subStr.empty()){
            stop_word_list.push_back(subStr);
        }
        auto it = stop_word_list.end()-1;
        it = stop_word_list.erase(it);
        stop_word_list.push_back("s");
        stop_word_list.push_back("t");
        stop_word_list.push_back("d");
    }
    vector<string> stop_word_list;
};

struct Stop_words stop_words = Stop_words();


vector<string> partition(string data_str, int nlines){
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<data_str.length();i++){
        if(data_str[i]=='\n'){
            if(!subStr.empty()){
                ret.push_back(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap(data_str[i]));
        }
    }
    if(!subStr.empty()){
        ret.push_back(subStr);
    }
    vector<string> tmp;
    int count = 0;
    string tmpStr="";
    for(auto pt=ret.begin();pt!=ret.end();pt++){
        tmpStr = tmpStr + *pt + "\n";
        count++;
        if(count==nlines){
            tmp.push_back(tmpStr.substr(0, strlen(tmpStr.c_str())-1));
            count = 0;
            tmpStr = "";
        }
    }
    if(count>0){
        tmp.push_back(tmpStr.substr(0, strlen(tmpStr.c_str())-1));
    }
    return tmp;
}

vector<pair<string, int>> split_words(string data_str){
    //scan
    vector<string> candidate_words;
    std::string subStr;
    for(size_t i=0;i<data_str.length();i++){
        if(!isAlpha(data_str[i])){
            if(!subStr.empty()){
                candidate_words.push_back(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap(data_str[i]));
        }
    }
    if(!subStr.empty()){
        candidate_words.push_back(subStr);
    }
    //remove stop_words
    vector<string> go_words;
    for(auto pt=candidate_words.begin();pt!=candidate_words.end();pt++){
        auto pt2 = find(stop_words.stop_word_list.begin(), stop_words.stop_word_list.end(), *pt);
        if(pt2==stop_words.stop_word_list.end()){
            go_words.push_back(*pt);
        }
    }
    vector<pair<string, int>> ret;
    for(auto pt=go_words.begin();pt!=go_words.end();pt++){
        pair<string, int> pr;
        pr.first = *pt;
        pr.second = 1;
        ret.push_back(pr);
    }
    return ret;
}

map<string, vector<pair<string, int>>> regroup(vector<vector<pair<string, int>>> pairs_list){
    map<string, vector<pair<string, int>>> mapping;
    for(auto pairs: pairs_list){
        for(auto p:pairs){
            auto pt = mapping.find(p.first);
            if(pt!=mapping.end()){
                mapping[p.first].push_back(p);
            }else{
                mapping[p.first]=vector<pair<string, int>>{p};
            }
        }
    }
    return mapping;
}

pair<string, int> count_words(pair<string, vector<pair<string, int>>> mapping){
    pair<string, int> ret;
    ret.first = mapping. first;
    ret.second = mapping.second.size();
    return ret;
}



int main(int argc, char *argv[]){
    //read file
    string content;
    ifstream ifs;
    ifs.open(argv[1], std::ifstream::binary);
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    content = data;
    ifs.close();
    vector<string> parts = partition(content, 200);
    vector<vector<pair<string, int>>> pairs_list;
    for(auto pt:parts){
        pairs_list.push_back(split_words(pt));
    }
    map<string, vector<pair<string, int>>> splits_per_word = regroup(pairs_list);
    vector<pair<string, vector<pair<string, int>>>> splits_list(splits_per_word.begin(), splits_per_word.end());
    vector<pair<string, int>> word_freqs;
    for(auto p: splits_list){
        word_freqs.push_back(count_words(p));
    }
    sort(word_freqs.begin(), word_freqs.end(), cmp());
    int count=0;
    for(auto p: word_freqs){
        count++;
        cout<<p.first<<" - "<<p.second<<endl;
        if(count==25) break;
    }
    return 0;
}