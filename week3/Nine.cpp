//
// Created by alex on 10/18/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <any>

using namespace std;

typedef string (* PF_GET_BOOK_CONTENT)(char *);
typedef vector<string>(* PF_GET_CANDIDATE_TERMS)(const std::string);
typedef vector<string>(* PF_FILTER_STD)(vector<string>);
typedef map<string, int>(* PF_COLLECT)(vector<string>);
typedef map<string, int>(* PF_FILTER_STOP_WORDS)(map<string, int>);

string get_book_content(char *);
vector<string> get_candidate_terms(const std::string);
vector<string> filter_std(vector<string>);
map<string, int> collect(vector<string>);
map<string, int> filter_stop_words(map<string, int>);

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){ return p1.second > p2.second;}
};

class TFTheOne{
public:
    TFTheOne(std::any v){
        value = v;
    }

    void bind(PF_GET_BOOK_CONTENT pf){
        value = pf(std::any_cast<char*>(value));
    }

    void bind(PF_GET_CANDIDATE_TERMS pf){
        value = pf(std::any_cast<string>(value));
    }

    void bind(PF_FILTER_STD pf){
        value = pf(std::any_cast<vector<string>>(value));
    }

    void bind(PF_COLLECT pf){
        value = pf(std::any_cast<vector<string>>(value));
    }

    void bind(PF_FILTER_STOP_WORDS pf){
        value = pf(std::any_cast<map<string, int>>(value));
    }

    void print_me(){
        std::any tmp;
        char *v1;
        tmp = v1;
        if(strcmp(value.type().name(), tmp.type().name())==0){
            cout<<std::any_cast<char *>(value)<<endl;
            return;
        }
        string v2;
        tmp = v2;
        if(strcmp(value.type().name(), tmp.type().name())==0){
            cout<<std::any_cast<string>(value)<<endl;
            return;
        }
        vector<string> v3;
        tmp = v3;
        if(strcmp(value.type().name(), tmp.type().name())==0){
            auto casted = std::any_cast<vector<string>>(value);
            for(auto pt: casted) cout<<pt<<endl;
            return;
        }
        map<string, int> v4;
        tmp = v4;
        if(strcmp(value.type().name(), tmp.type().name())==0){
            auto casted = std::any_cast<map<string, int>>(value);
            vector<pair<string, int>> vec(casted.begin(), casted.end());
            sort(vec.begin(), vec.end(), cmp());
            int min = 25;
            if(vec.size()<min){
                min = (int)vec.size();
            }
            for(auto pt = vec.begin();pt<vec.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
            return;
        }
    }

private:
    std::any value;
};

bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap(char c){
    if(c<='Z'&&c>='A') return c+32;
    else return c;
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
        }else subStr.push_back(decap(str[i]));
    }
    if(!subStr.empty()) ret.push_back(subStr);
    auto it = ret.end()-1;
    it = ret.erase(it);
    return ret;
}

vector<string> get_candidate_terms(const std::string str){
    vector<string> ret;
    std::string subStr;
    for(size_t i=0;i<str.length();i++) if(!isAlpha(str[i])){ if(!subStr.empty()){ ret.push_back(subStr); subStr.clear();}}else subStr.push_back(decap(str[i]));
    if(!subStr.empty()) ret.push_back(subStr);
    return ret;
}

vector<string> filter_std(vector<string> candidate_terms){
    string s = "s", t = "t", d = "d";
    for(auto pt = candidate_terms.begin();pt!=candidate_terms.end();pt++) if(strcmp(pt->c_str(), s.c_str())==0 || strcmp(pt->c_str(), t.c_str())==0 || strcmp(pt->c_str(), d.c_str())==0) pt = candidate_terms.erase(pt);
    return candidate_terms;
}

map<string, int> collect(vector<string> terms){
    map<string, int> ret;
    for(auto pt = terms.begin();pt!=terms.end();pt++){
        auto pt2 = ret.find(*pt);
        if(pt2==ret.end()) ret.insert(pair<string, int>(*pt, 1));
        else pt2->second++;
    }
    return ret;
}

map<string, int> filter_stop_words(map<string, int> collection){
    vector<string> stop_words = get_stop_word();
    for(string sw: stop_words) collection.erase(sw);
    return collection;
}

int main(int argc, char *argv[]){
    TFTheOne tfTheOne = TFTheOne(argv[1]);
    tfTheOne.bind(get_book_content);
    tfTheOne.bind(get_candidate_terms);
    tfTheOne.bind(filter_std);
    tfTheOne.bind(collect);
    tfTheOne.bind(filter_stop_words);
    tfTheOne.print_me();
    return 0;
}
