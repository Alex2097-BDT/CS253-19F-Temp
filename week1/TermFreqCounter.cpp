//
// Created by alex on 10/1/19.
//
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <string.h>

struct Term{
    char word[100];
    int count;
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


bool fileExists(const std::string &fileName) {
    /**
     * judge whether a file exist
     */
    struct stat buf{};
    return stat(fileName.c_str(), &buf) == 0;
}

int main(int argc, char *argv[]){

    FILE *book, *f_stop_words;
    char stop_words[1000];
    char *stop_words_splitter;
    if(argc != 2){
        cout << "Please enter like this:\n./TermFreqCounter sample.txt" << endl;
        return ERROR_WRONG_ARGUMENTS;
    }
    if(!fileExists(argv[1])){
        cout << "File not found." << endl;
        return ERROR_FILE_NOT_FOUND;
    }

    f_stop_words = fopen("../stop_words.txt", "r");
    if(f_stop_words==NULL) {
        fclose(f_stop_words);
        return ERROR_FILE_NOT_FOUND;
    }

    char *stop_words_split[1000];
    int i;
    int counter = 0;
    fgets(stop_words, 1000, f_stop_words);
    stop_words_splitter = strtok(stop_words, ",\n");
    while(stop_words_splitter != NULL){
        stop_words_split[counter] = stop_words_splitter;
        stop_words_splitter = strtok(NULL, ",");
        counter++;
    }
    counter-=1; //last token not included
    fclose(f_stop_words);

    book = fopen(argv[1], "r");
    char book_line[1000];
    char *term_splitter;
    Term terms[50000];
    int term_counter=0;
    char c,c_end;
    int line_num = 1;
    int term_len = 0;
    int j,k;
    char candidate_term[100];
    bool match = false;
    bool isStopWord;


    while(fgets(book_line, 1000, book)!=NULL){
        for(i=0;i<strlen(book_line);){
            c = book_line[i];

            if(!isAlpha(c)){
                i++;
                continue;
            }else{
                j=i;
                term_len = 1;
                candidate_term[0] = decap(c);
                do{
                    j++;
                    term_len++;
                    c_end = book_line[j];
                    candidate_term[term_len-1] = decap(c_end);
                }while(isAlpha(c_end));
                candidate_term[term_len-1] = '\0';
                term_len--;
                isStopWord = false;
                for(k=0;k<counter;k++){
                    if(strcmp(candidate_term, stop_words_split[k])==0){
                        isStopWord = true;
                        break;
                    }
                }
                if(isStopWord){
                    i+=term_len;
                    continue;
                }else{
                    if(strcmp(candidate_term, "s")==0||strcmp(candidate_term, "t")==0||strcmp(candidate_term, "d")==0){
                        //elizabeth's could be divided into elizabeth and s since ' is a delimiter, but s is not a term!
                        i+=term_len;
                        continue;
                    }
                    match = false;
                    for(k=0;k<term_counter;k++){
                        if(strcmp(candidate_term, terms[k].word)==0){
                            match = true;
                            terms[k].count++;
                            break;
                        }
                    }
                    if(!match){
                        for(k=0;k<100;k++){
                            terms[term_counter].word[k] = candidate_term[k];
                        }
                        terms[term_counter].count = 1;
                        term_counter++;
                    }
                    i+=term_len;
                }
            }

        }
        line_num++;
    }
    fclose(book);
    char tmp_char;
    int tmp_count;
    for(i=0;i<term_counter;i++){
        for(j=i+1;j<term_counter;j++){
            if(terms[i].count < terms[j].count){
                tmp_count = terms[i].count;
                terms[i].count = terms[j].count;
                terms[j].count = tmp_count;
                for(k=0;k<100;k++){
                    tmp_char = terms[i].word[k];
                    terms[i].word[k] = terms[j].word[k];
                    terms[j].word[k] = tmp_char;
                }
            }
        }
    }
    for(i=0;i<25;i++){
        cout << terms[i].word << "  -  " << terms[i].count << endl;
    }
    return SUCCESS;
}
