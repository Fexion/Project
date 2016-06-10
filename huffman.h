#include <iostream>
#include <string>
#include <bitset>
#include <map>
#include <fstream>
#include <algorithm>
#include <list>
#include <time.h>
#include <ctime>
#include <stdlib.h>

#define Eof 128
#define Esc 129


namespace codecs {
    struct node {
        size_t num;
        int c;
        bool is_leaf  = true;
        size_t left;
        size_t right;

        node(){}

        node(size_t, size_t);
    };
    int binary(std::vector<node*>&, node*, int);

    void filling_symbols(size_t, std::vector<bool>,std::vector <std::vector<bool>>&);
    void learning_from_file(std::string);
    std::vector<std::vector <bool> > encoding(const std::string);
    void decoding(const std::vector<std::vector<bool>>&);
    void clear_func();

    void smn_help_me();


}
