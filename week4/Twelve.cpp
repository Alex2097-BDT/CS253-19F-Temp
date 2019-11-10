//
// Created by alex on 10/26/19.
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

typedef struct _data_storage_obj{
    vector<string> data;
    function<void(char *)> init = [this](char * fileName){
        extract_word(data, fileName);
    };
    function<vector<string>()> words = [this](){
        return data;
    };
} data_storage_obj;

typedef struct _stop_words_obj{
    vector<string> stop_words;
    function<void()> init = [this](){
        load_stop_words(stop_words);
    };
    function<bool(string)> is_stop_word = [this](string word){
        return (find(stop_words.begin(),stop_words.end(),word)!=stop_words.end());
    };
} stop_words_obj;

typedef struct _word_freq_obj{
    map<string, int> freqs;
    function<void(string)> incre_count = [this](string word){
        increment_count(freqs, word);
    };
    function<vector<pair<string, int>>()> sorted = [this](){
        vector<pair<string, int>> vec(freqs.begin(), freqs.end());
        sort(vec.begin(), vec.end(), cmp());
        return vec;
    };
} word_freq_obj;

typedef struct _selfref_data_storage_obj{
    vector<string> data;
    struct _selfref_data_storage_obj *me;
    function<void(struct _selfref_data_storage_obj *, char *)> init = [this](struct _selfref_data_storage_obj *p, char * fileName){
        me = p;
        extract_word(me->data, fileName);
    };
    function<vector<string>()> words = [this](){
        return me->data;
    };

} sf_data_storage_obj;

typedef struct _selfref_stop_words_obj{
    vector<string> stop_words;
    struct _selfref_stop_words_obj *me;
    function<void(struct _selfref_stop_words_obj *)> init = [this](struct _selfref_stop_words_obj *p){
        me = p;
        load_stop_words(me->stop_words);
    };
    function<bool(string)> is_stop_word = [this](string word){
        return (find(me->stop_words.begin(),me->stop_words.end(),word)!=me->stop_words.end());
    };
} sf_stop_words_obj;

typedef struct _selfref_word_freq_obj{
    map<string, int> freqs;
    struct _selfref_word_freq_obj *me;
    function<void(struct _selfref_word_freq_obj *)> init = [this](struct _selfref_word_freq_obj *p){
        me = p;
    };
    function<void(string)> incre_count = [this](string word){
        increment_count(me->freqs, word);
    };
    function<vector<pair<string, int>>()> sorted = [this](){
        vector<pair<string, int>> vec(me->freqs.begin(), me->freqs.end());
        sort(vec.begin(), vec.end(), cmp());
        return vec;
    };
} sf_word_freq_obj;

//int main(int argc, char *argv[]){
//    int practice = 1;
//    if(argc==3){
//        practice = *argv[2]-'0';
//    }
//
//    switch(practice){
//        case 1:{
//            //practice 12.1
//            data_storage_obj dso;
//            dso.init(argv[1]);
//            stop_words_obj swo;
//            swo.init();
//            word_freq_obj wfo;
//            for(auto w: dso.words()){
//                if(!swo.is_stop_word(w)){
//                    wfo.incre_count(w);
//                }
//            }
//            auto word_freqs = wfo.sorted();
//            int min = 25;
//            for(auto pt = word_freqs.begin();pt<word_freqs.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
//            break;
//        }
//
//        case 2:{
//            //practice 12.2
//            data_storage_obj dso;
//            dso.init(argv[1]);
//            stop_words_obj swo;
//            swo.init();
//
//            //here is the "end" of my program. New function(member) top25 is added by
//            //inheriting word_freq_obj to new_word_freq_obj.
//            struct new_word_freq_obj:word_freq_obj{
//                function<void()> top25 = [this](){
//                    vector<pair<string, int>> vec(freqs.begin(), freqs.end());
//                    sort(vec.begin(), vec.end(), cmp());
//                    for(auto pt = vec.begin();pt<vec.begin()+25;pt++) cout << pt->first << "  -  " << pt->second << endl;
//                };
//            } n_wfo;
//            for(auto w: dso.words()){
//                if(!swo.is_stop_word(w)){
//                    n_wfo.incre_count(w);
//                }
//            }
//            n_wfo.top25();
//            break;
//        }
//
//        case 3:{
//            //practice 12.3
//            sf_data_storage_obj sf_dso;
//            sf_dso.init(&sf_dso, argv[1]);
//            sf_stop_words_obj sf_swo;
//            sf_swo.init(&sf_swo);
//            sf_word_freq_obj sf_wfo;
//            sf_wfo.init(&sf_wfo);   //necessary, this initiates pointer me in sf_wfo
//            for(auto w: sf_dso.words()){
//                if(!sf_swo.is_stop_word(w)){
//                    sf_wfo.incre_count(w);
//                }
//            }
//            auto word_freqs = sf_wfo.sorted();
//            int min = 25;
//            for(auto pt = word_freqs.begin();pt<word_freqs.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
//            break;
//        }
//        default:
//            cout<<"invalid third input argument"<<endl;
//            return -1;
//    }
//    return 0;
//}