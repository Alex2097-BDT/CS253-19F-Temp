//
// Created by alex on 11/9/19.
//
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

using namespace std;

enum Actor{_main, _novelManager, _stopWordManager, _wordFreqManager};

struct Message{
    Actor sender;
    Actor receiver;
    string type;
    string data;
};

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){
        return p1.second > p2.second;
    }
};

void sendMessage(Message);
bool isAlpha(char);
char decap(char);


template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;
public:
    threadsafe_queue(){}
    threadsafe_queue(threadsafe_queue const& other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue=other.data_queue;
    }
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T& value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return false;
        value=data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        if(data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

class NovelManager{
public:
    NovelManager(){
        _stop = false;
    }

    void run(){
        while(!_stop){
            shared_ptr<Message> ptr = _queue.wait_and_pop();
            _dispatch(*ptr);
            if(ptr->type=="end") _stop=true;
        }
    }

    void _dispatch(Message message){
        if(message.type=="init"){
            //do read file
            shared_ptr<Message> ptr = _queue.wait_and_pop();
            string filePath = ptr->data;
            ifstream ifs;
            ifs.open(filePath.c_str(), std::ifstream::binary);
            if(!ifs) return;
            ifs.seekg(0, ifs.end);
            int len = ifs.tellg();
            ifs.seekg(0, ifs.beg);
            char *data = new char[len+1];
            ifs.read(data, len);
            data[len] = '\0';
            content = data;
            ifs.close();
            //split words
            std::string subStr;
            for(size_t i=0;i<content.length();i++){
                if(!isAlpha(content[i])){
                    if(!subStr.empty()){
                        //wrap message and send to StopWordManager
                        Message msg_filterWord;
                        msg_filterWord.sender = _novelManager;
                        msg_filterWord.receiver = _stopWordManager;
                        msg_filterWord.type = "filter";
                        msg_filterWord.data = subStr;
                        sendMessage(msg_filterWord);
                        subStr.clear();
                    }
                }else{
                    subStr.push_back(decap(content[i]));
                }
            }
            if(!subStr.empty()){
                //wrap message and send to StopWordManager
                Message msg_filterWord;
                msg_filterWord.sender = _novelManager;
                msg_filterWord.receiver = _stopWordManager;
                msg_filterWord.type = "filter";
                msg_filterWord.data = subStr;
                sendMessage(msg_filterWord);
            }
            Message msg_top25;
            msg_top25.sender = _novelManager;
            msg_top25.receiver = _stopWordManager;
            msg_top25.type = "top25";
            sendMessage(msg_top25);
            Message msg_die;
            msg_die.sender = _novelManager;
            msg_die.receiver = _novelManager;
            msg_die.type = "end";
            sendMessage(msg_die);
        }else{
            message.sender = _novelManager;
            message.receiver = _stopWordManager;
            sendMessage(message);
        }
    }

public:
    threadsafe_queue<Message> _queue;
private:
    bool _stop;
    string content;
};

class StopWordManager{
public:
    StopWordManager(){
        _stop = false;
        //do read file
        string filePath = "./stop_words.txt";
        ifstream ifs;
        ifs.open(filePath.c_str(), std::ifstream::binary);
        if(!ifs) return;
        ifs.seekg(0, ifs.end);
        int len = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        char *data = new char[len+1];
        ifs.read(data, len);
        data[len] = '\0';
        string str = data;
        ifs.close();
        //split words
        std::string subStr;
        for(size_t i=0;i<str.length();i++){
            if(str[i]==','){
                if(!subStr.empty()){
                    stop_words.push_back(subStr);
                    subStr.clear();
                }
            }else{
                subStr.push_back(decap(str[i]));
            }
        }
        if(!subStr.empty()){
            stop_words.push_back(subStr);
        }
        auto it = stop_words.end()-1;
        it = stop_words.erase(it);
        stop_words.push_back("s");
        stop_words.push_back("t");
        stop_words.push_back("d");
    }

    void run(){
        while(!_stop){
            shared_ptr<Message> ptr = _queue.wait_and_pop();
            _dispatch(*ptr);
            if(ptr->type=="end") _stop=true;
        }
    }

    void _dispatch(Message message){
        if(message.type=="filter"){
            auto pt = find(stop_words.begin(), stop_words.end(), message.data);
            if(pt==stop_words.end()){
                message.sender = _stopWordManager;
                message.receiver = _wordFreqManager;
                message.type="word";
                sendMessage(message);
            }
        }else{
            message.sender = _stopWordManager;
            message.receiver = _wordFreqManager;
            sendMessage(message);
            Message msg_die;
            msg_die.sender = _stopWordManager;
            msg_die.receiver = _stopWordManager;
            msg_die.type = "end";
            sendMessage(msg_die);
        }
    }
public:
    threadsafe_queue<Message> _queue;
private:
    bool _stop;
    vector<string> stop_words;
};


class WordFreqManager{
public:
    WordFreqManager(){
        _stop = false;
    }

    void run(){
        while(!_stop){
            shared_ptr<Message> ptr = _queue.wait_and_pop();
            _dispatch(*ptr);
            if(ptr->type=="end") _stop=true;
        }
    }

    void _dispatch(Message message){
        if(message.type=="word"){
            auto pt2 = word_freq.find(message.data);
            if(pt2==word_freq.end()){
                word_freq.insert(pair<string, int>(message.data, 1));
            }else{
                pt2->second++;
            }
        }else if(message.type=="top25"){
            int i;
            vector<pair<string, int>> vec(word_freq.begin(), word_freq.end());
            sort(vec.begin(), vec.end(), cmp());
            auto pt = vec.begin();
            for(i=0;i<25;i++){
                cout << pt->first << "  -  " << pt->second << endl;
                pt++;
            }
            Message msg_end_novel;
            msg_end_novel.sender = _wordFreqManager;
            msg_end_novel.receiver = _novelManager;
            msg_end_novel.type = "end";
            msg_end_novel.data = "";
            sendMessage(msg_end_novel);
        }else{

        }
    }
public:
    threadsafe_queue<Message> _queue;
private:
    bool _stop;
    map<string, int> word_freq;
};

NovelManager *novelManager = new NovelManager();
StopWordManager *stopWordManager = new StopWordManager();
WordFreqManager *wordFreqManager = new WordFreqManager();


int main(int argc, char *args[]){

    Message msg_init_novel;
    msg_init_novel.sender = _main;
    msg_init_novel.receiver = _novelManager;
    msg_init_novel.type = "init";
    msg_init_novel.data = "";
    sendMessage(msg_init_novel);

    Message msg_novelPath;
    msg_novelPath.sender = _main;
    msg_novelPath.receiver = _novelManager;
    msg_novelPath.type = "data";
    msg_novelPath.data = args[1];
    sendMessage(msg_novelPath);

    Message msg_init_wf;
    msg_init_wf.sender = _main;
    msg_init_wf.receiver = _wordFreqManager;
    msg_init_wf.type = "run";
    msg_init_wf.data = "";
    sendMessage(msg_init_wf);

    thread novel_th(mem_fun(&NovelManager::run), novelManager);
    thread stopwd_th(mem_fun(&StopWordManager::run), stopWordManager);
    thread wordfq_th(mem_fun(&WordFreqManager::run), wordFreqManager);
    novel_th.join();
    stopwd_th.join();
    wordfq_th.join();
    return 0;
}

bool isAlpha(char c){
    if((c<='Z'&&c>='A')||(c<='z'&&c>='a')) return true;
    else return false;
}

char decap(char c){
    if(c<='Z'&&c>='A'){
        return c+32;
    }else return c;
}

void sendMessage(Message message){
    switch(message.receiver){
        case _novelManager:{
            novelManager->_queue.push(message);
            break;
        }
        case _stopWordManager:{
            stopWordManager->_queue.push(message);
            break;
        }
        case _wordFreqManager:{
            wordFreqManager->_queue.push(message);
            break;
        }
        default:
            return;
    }
}