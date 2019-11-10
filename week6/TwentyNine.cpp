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

#define WORKER_NUM 5

using namespace std;

struct cmp{
    bool operator()(const pair<string, int> &p1, const pair<string, int> &p2){
        return p1.second > p2.second;
    }
};

bool isAlpha(char);
char decap(char);
string get_book_content(char *);


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

threadsafe_queue<string> word_space;
threadsafe_queue<map<string, int>> freq_space;
threadsafe_queue<pair<string, int>> merged_freq_space;

void process_words(){
    map<string, int> word_freq;
    while(1){
        if(!word_space.empty()){
            shared_ptr<string> ptr = word_space.wait_and_pop();
            auto pt = find(stop_words.stop_word_list.begin(), stop_words.stop_word_list.end(), *ptr);
            if(pt==stop_words.stop_word_list.end()){
                auto pt2 = word_freq.find(*ptr);
                if(pt2==word_freq.end()){
                    word_freq.insert(pair<string, int>(*ptr, 1));
                }else{
                    pt2->second++;
                }
            }
        }else{
            break;
        }
    }
    freq_space.push(word_freq);
}

void process_freqs(){
    while(!freq_space.empty()){
        shared_ptr<map<string, int>> ptr = freq_space.wait_and_pop();
        vector<pair<string, int>> vec(ptr->begin(), ptr->end());
        for(auto pt = vec.begin();pt!=vec.end();pt++){
            bool seenm1 = false;
            bool find = false;
            while(1){
                shared_ptr<pair<string, int>> ptr2 = merged_freq_space.wait_and_pop();
                if(ptr2->second==-1){
                    if(seenm1){
                        merged_freq_space.push(*ptr2);
                        break;
                    }else{
                        seenm1 = true;
                        merged_freq_space.push(*ptr2);
                        continue;
                    }
                }else{
                    if(pt->first==ptr2->first){
                        find = true;
                        pair<string, int> new_pair;
                        new_pair.first = pt->first;
                        new_pair.second = pt->second + ptr2->second;
                        merged_freq_space.push(new_pair);
                    }else{
                        merged_freq_space.push(*ptr2);
                    }
                }
            }
            if(!find){
                merged_freq_space.push(*pt);
            }
        }
    }
}


int main(int argc, char *args[]){
    //read file
    string content;
    ifstream ifs;
    ifs.open(args[1], std::ifstream::binary);
    ifs.seekg(0, ifs.end);
    int len = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char *data = new char[len+1];
    ifs.read(data, len);
    data[len] = '\0';
    content = data;
    ifs.close();
    //parse words
    std::string subStr;
    for(size_t i=0;i<content.length();i++){
        if(!isAlpha(content[i])){
            if(!subStr.empty()){
                word_space.push(subStr);
                subStr.clear();
            }
        }else{
            subStr.push_back(decap(content[i]));
        }
    }
    if(!subStr.empty()){
        word_space.push(subStr);
    }

    //generate 5 threads(workers)
    for(int i=0;i<5;i++){
        thread worker(process_words);
        worker.join();
    }

    //init: insert minus 1 to merged freq space to avoid infinite checking
    pair<string, int> m1;
    m1.first="";
    m1.second=-1;
    merged_freq_space.push(m1);

    //generate 5 threads(workers)
    for(int i=0;i<5;i++){
        thread worker(process_freqs);
        worker.join();
    }

    //normalize merged_freq_space
    vector<pair<string, int>> normalized_merged_freq_space;
    while(!merged_freq_space.empty()){
        shared_ptr<pair<string, int>> ptr = merged_freq_space.try_pop();
        normalized_merged_freq_space.push_back(*ptr);
    }

    //sort and print
    sort(normalized_merged_freq_space.begin(), normalized_merged_freq_space.end(), cmp());
    auto pt = normalized_merged_freq_space.begin();
    for(int i=0;i<25;i++){
        cout << pt->first << "  -  " << pt->second << endl;
        pt++;
    }
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