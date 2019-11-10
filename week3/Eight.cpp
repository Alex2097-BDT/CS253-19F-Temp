//
// Created by alex on 10/18/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

typedef void (* PF_SORT_AND_PRINT)(map<string, int>, int);
typedef void (* PF_FILTER_STOP_WORDS)(map<string, int>, PF_SORT_AND_PRINT);
typedef void (* PF_COLLECT)(vector<string>, PF_FILTER_STOP_WORDS);
typedef void (* PF_FILTER_STD)(vector<string>, PF_COLLECT);
typedef void (* PF_GET_CANDIDATE_TERMS)(const std::string, PF_FILTER_STD);

void get_book_content(char *, PF_GET_CANDIDATE_TERMS);
void get_candidate_terms(const std::string, PF_FILTER_STD);
void filter_std(vector<string>, PF_COLLECT);
void collect(vector<string>, PF_FILTER_STOP_WORDS);
void filter_stop_words(map<string, int>, PF_SORT_AND_PRINT);
void sort_and_print(map<string, int>, int);

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){ return p1.second > p2.second;}
};

//auxiliary function, not included in the pipeline, so do not have to follow constraint 8.1
bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

//auxiliary function, not included in the pipeline, so do not have to follow constraint 8.1
char decap(char c){
    if(c<='Z'&&c>='A') return c+32;
    else return c;
}

//auxiliary function, not included in the pipeline, so do not have to follow constraint 8.1
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

//auxiliary function, not included in the pipeline, so do not have to follow constraint 8.1
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
        }else subStr.push_back(decap(str[i]));
    }
    if(!subStr.empty()) ret.push_back(subStr);
    auto it = ret.end()-1;
    it = ret.erase(it);
    return ret;
}

void get_book_content(char *fileName, PF_GET_CANDIDATE_TERMS pf){
    string ret;
    ifstream ifs;
    ifs.open(fileName, std::ifstream::binary);
    if(!ifs) return;
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    ret = data;
    ifs.close();
    PF_FILTER_STD pf_next = filter_std;
    pf(ret, pf_next);
}

void get_candidate_terms(const std::string str, PF_FILTER_STD pf){
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<str.length();i++) if(!isAlpha(str[i])){ if(!subStr.empty()){ ret.push_back(subStr); subStr.clear();}}else subStr.push_back(decap(str[i]));
    if(!subStr.empty()) ret.push_back(subStr);
    PF_COLLECT pf_next = collect;
    pf(ret, pf_next);
}

void filter_std(vector<string> candidate_terms, PF_COLLECT pf){
    string s = "s", t = "t", d = "d";
    for(auto pt = candidate_terms.begin();pt!=candidate_terms.end();pt++) if(strcmp(pt->c_str(), s.c_str())==0 || strcmp(pt->c_str(), t.c_str())==0 || strcmp(pt->c_str(), d.c_str())==0) pt = candidate_terms.erase(pt);
    PF_FILTER_STOP_WORDS pf_next = filter_stop_words;
    pf(candidate_terms, pf_next);
}


void collect(vector<string> terms, PF_FILTER_STOP_WORDS pf){
    map<string, int> ret;
    for(auto pt = terms.begin();pt!=terms.end();pt++){
        auto pt2 = ret.find(*pt);
        if(pt2==ret.end()) ret.insert(pair<string, int>(*pt, 1));
        else pt2->second++;
    }
    PF_SORT_AND_PRINT pf_next = sort_and_print;
    pf(ret, pf_next);
}

void filter_stop_words(map<string, int> collection, PF_SORT_AND_PRINT pf){
    vector<string> stop_words = get_stop_word();
    for(string sw: stop_words) collection.erase(sw);
    pf(collection, 25);
}

void sort_and_print(map<string, int> mp, int n){
    vector<pair<string, int>> vec(mp.begin(), mp.end());
    sort(vec.begin(), vec.end(), cmp());
    for(auto pt = vec.begin();pt<vec.begin()+n;pt++) cout << pt->first << "  -  " << pt->second << endl;
}

//int main(int argc, char *argv[]){
//    PF_GET_CANDIDATE_TERMS pf = get_candidate_terms;
//    get_book_content(argv[1], pf);
//    return 0;
//}
