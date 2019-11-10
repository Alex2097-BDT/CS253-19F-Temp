//
// Created by alex on 10/26/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;
using namespace std::placeholders;

typedef function<void(string)> STRING_FUN;
typedef function<void()> EMPTY_FUN;

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

class WordFrequencyFramework{
public:
    void register_for_load_event(STRING_FUN handler){
        _load_event_handlers.push_back(handler);
    }

    void register_for_dowork_event(EMPTY_FUN handler){
        _dowork_event_handlers.push_back(handler);
    }

    void register_for_end_event(EMPTY_FUN handler){
        _end_event_handlers.push_back(handler);
    }

    void run(string strData){
        for(auto handler: _load_event_handlers){
            handler(strData);
        }
        for(auto handler: _dowork_event_handlers){
            handler();
        }
        for(auto handler: _end_event_handlers){
            handler();
        }
    }

private:
    vector<STRING_FUN> _load_event_handlers;
    vector<EMPTY_FUN> _dowork_event_handlers;
    vector<EMPTY_FUN> _end_event_handlers;
};

class StopWordFilter{
public:
    void _load(string ignore){
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
                    _stop_words->push_back(subStr);
                    subStr.clear();
                }
            }else subStr.push_back(decap(str[i]));
        }
        if(!subStr.empty()) _stop_words->push_back(subStr);
        //remove the last element in obj
        auto it = _stop_words->end()-1;
        it = _stop_words->erase(it);
        //adding "s", "t", "d"
        _stop_words->push_back("s");
        _stop_words->push_back("t");
        _stop_words->push_back("d");
    }

    bool is_stop_word(string word){
        return (find(_stop_words->begin(),_stop_words->end(),word)!=_stop_words->end());
    }

    StopWordFilter(WordFrequencyFramework *wfapp){
        _stop_words = new vector<string>();
        STRING_FUN sfun = bind(&StopWordFilter::_load, this, _1);
        wfapp->register_for_load_event(sfun);
    }

private:
    vector<string> *_stop_words;
};

class DataStorage{
public:
    DataStorage(WordFrequencyFramework *wfapp, StopWordFilter *sw_filter){
        _data = new vector<string>();
        _stop_word_filter = sw_filter;
        STRING_FUN sfun = bind(&DataStorage::_load, this, _1);
        wfapp->register_for_load_event(sfun);
        EMPTY_FUN efun = bind(&DataStorage::_produce_words, this);
        wfapp->register_for_dowork_event(efun);
    }
    void _load(string fileName){
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
        for(size_t i=0;i<str.length();i++) if(!isAlpha(str[i])){ if(!subStr.empty()){ _data->push_back(subStr); subStr.clear();}}else subStr.push_back(decap(str[i]));
        if(!subStr.empty()) _data->push_back(subStr);
    }

    void _produce_words(){
        for(auto w: *_data){
            if(!_stop_word_filter->is_stop_word(w)) {
                for (auto handlers: _word_event_handlers) {
                    handlers(w);
                }
            }
        }
    }

    void register_for_word_event(STRING_FUN handler){
        _word_event_handlers.push_back(handler);
    }



private:
    vector<string> *_data;
    StopWordFilter *_stop_word_filter;
    vector<STRING_FUN> _word_event_handlers;
};

class WordFrequencyCounter{
public:
    WordFrequencyCounter(WordFrequencyFramework *wfapp, DataStorage *data_storage){
        STRING_FUN sfun = bind(&WordFrequencyCounter::_increment_count, this, _1);
        EMPTY_FUN efun = bind(&WordFrequencyCounter::_print_freqs, this);
        data_storage->register_for_word_event(sfun);
        wfapp->register_for_end_event(efun);
    }

    void _increment_count(string word){
        auto pt = _word_freqs.find(word);
        if(pt==_word_freqs.end()) _word_freqs.insert(pair<string, int>(word, 1));
        else pt->second++;
    }

    void _print_freqs(){
        vector<pair<string, int>> vec(_word_freqs.begin(), _word_freqs.end());
        sort(vec.begin(), vec.end(), cmp());
        int min = 25;
        for(auto pt = vec.begin();pt<vec.begin()+min;pt++) cout << pt->first << "  -  " << pt->second << endl;
    }

private:
    map<string, int> _word_freqs;
};

int main(int argc, char *argv[]){
    int practice = 1;
    if(argc==3){
        practice = *argv[2] - '0';
    }
    switch(practice){
        case 1:{
            WordFrequencyFramework *wfapp = new WordFrequencyFramework();
            StopWordFilter *stop_word_filter = new StopWordFilter(wfapp);
            DataStorage  *data_storage = new DataStorage(wfapp, stop_word_filter);
            WordFrequencyCounter *word_freq_counter = new WordFrequencyCounter(wfapp, data_storage);
            wfapp->run(argv[1]);
            break;
        }
        case 2:{
            //Here I define another counter like "WordFreqCounter" and register it after registering WordFreqCounter
            //Other parts remain the same.
            class ZCounter{
            public:
                ZCounter(WordFrequencyFramework *wfapp, DataStorage *data_storage){
                    zcount = 0;
                    STRING_FUN sfun = bind(&ZCounter::_increment_count, this, _1);
                    EMPTY_FUN efun = bind(&ZCounter::_print_zcount, this);
                    data_storage->register_for_word_event(sfun);
                    wfapp->register_for_end_event(efun);
                }

                void _increment_count(string word){
                    const char *tmp = strchr(word.c_str(), 'z');
                    if(tmp!=NULL) zcount++;
                }

                void _print_zcount(){
                    cout<<"Number of non-stop words that contain z: "<<zcount<<endl;
                }

            private:
                int zcount;
            };
            WordFrequencyFramework *wfapp = new WordFrequencyFramework();
            StopWordFilter *stop_word_filter = new StopWordFilter(wfapp);
            DataStorage  *data_storage = new DataStorage(wfapp, stop_word_filter);
            WordFrequencyCounter *word_freq_counter = new WordFrequencyCounter(wfapp, data_storage);
            //here is the additional function
            ZCounter *zcounter = new ZCounter(wfapp, data_storage);
            wfapp->run(argv[1]);
            break;
        }
        default:
            cout<<"invalid third input argument"<<endl;
            return -1;
    }
}