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

#ifndef WEEK5_WORDS1_H
#define WEEK5_WORDS1_H

bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap_w1(char c){
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


vector<string> get_candidate_terms(const std::string str){
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<str.length();i++){
        if(!isAlpha(str[i])){
            if(!subStr.empty()){
                ret.push_back(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap_w1(str[i]));
        }
    }
    if(!subStr.empty()){
        ret.push_back(subStr);
    }
    return ret;
}

vector<string> filter_std(vector<string> candidate_terms){
    string s = "s";
    string t = "t";
    string d = "d";
    for(auto pt = candidate_terms.begin();pt!=candidate_terms.end();pt++){
        if(strcmp(pt->c_str(), s.c_str())==0 || strcmp(pt->c_str(), t.c_str())==0 || strcmp(pt->c_str(), d.c_str())==0){
            pt = candidate_terms.erase(pt);
            continue;
        }
    }
    return candidate_terms;
}

vector<string> extract_words(char *fileName){
    return filter_std(get_candidate_terms(get_book_content(fileName)));
}
#endif //WEEK5_WORDS1_H
