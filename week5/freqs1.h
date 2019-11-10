//
// Created by alex on 11/2/19.
//

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;
#ifndef WEEK5_FREQS1_H
#define WEEK5_FREQS1_H

char decap_f1(char c){
    if(c<='Z'&&c>='A'){
        return c+32;
    }else return c;
}

struct cmp_f1{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){
        return p1.second > p2.second;
    }
};

string get_book_content_f1(char *fileName){
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

vector<string> get_stop_word(){
    string str = get_book_content_f1("./stop_words.txt");
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<str.length();i++){
        if(str[i]==','){
            if(!subStr.empty()){
                ret.push_back(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap_f1(str[i]));
        }
    }
    if(!subStr.empty()){
        ret.push_back(subStr);
    }
    auto it = ret.end()-1;
    it = ret.erase(it);
    return ret;
}

map<string, int> collect(vector<string> terms){
    map<string, int> ret;
    for(auto pt = terms.begin();pt!=terms.end();pt++){
        auto pt2 = ret.find(*pt);
        if(pt2==ret.end()){
            ret.insert(pair<string, int>(*pt, 1));
        }else{
            pt2->second++;
        }
    }
    return ret;
}

map<string, int> filter_stop_words(map<string, int> collection){
    vector<string> stop_words = get_stop_word();
    for(string sw: stop_words){
        collection.erase(sw);
    }
    return collection;
}


void sort_and_print(map<string, int> mp, int n){
    int i;
    vector<pair<string, int>> vec(mp.begin(), mp.end());
    sort(vec.begin(), vec.end(), cmp_f1());
    auto pt = vec.begin();
    for(i=0;i<n;i++){
        cout << pt->first << "  -  " << pt->second << endl;
        pt++;
    }
}

void sort_and_print(map<string, int> mp){
    sort_and_print(mp, 25);
}

void top25(vector<string> wf){
    sort_and_print(filter_stop_words(collect(wf)));
}
#endif //WEEK5_FREQS1_H
