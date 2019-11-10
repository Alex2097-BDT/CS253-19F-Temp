//
// Created by alex on 10/11/19.
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

enum RET {SUCCESS, ERROR_WRONG_ARGUMENTS, ERROR_FILE_NOT_FOUND};

using namespace std;

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

vector<string> get_stop_word(){
    string str = get_book_content("./stop_words.txt");
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<str.length();i++){
        if(str[i]==','){
            if(!subStr.empty()){
                ret.push_back(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap(str[i]));
        }
    }
    if(!subStr.empty()){
        ret.push_back(subStr);
    }
    auto it = ret.end()-1;
    it = ret.erase(it);
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
            subStr.push_back(decap(str[i]));
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
    sort(vec.begin(), vec.end(), cmp());
    auto pt = vec.begin();
    for(i=0;i<n;i++){
        cout << pt->first << "  -  " << pt->second << endl;
        pt++;
    }
}

void sort_and_print(map<string, int> mp){
    sort_and_print(mp, 25);
}


int main(int argc, char *argv[]){
    sort_and_print(filter_stop_words(collect(filter_std(get_candidate_terms(get_book_content(argv[1]))))));
    return SUCCESS;
}
