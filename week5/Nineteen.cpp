//
// Created by alex on 11/2/19.
//
#include "config.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <map>

#ifdef WORD_1
#include "words1.h"
#else
#include "words2.h"
#endif

#ifdef FREQ_1
#include "freqs1.h"
#else
#include "freqs2.h"
#endif

using namespace std;

int main(int argc, char *argv[]){
    top25(extract_words(argv[1]));
    return 0;
}
