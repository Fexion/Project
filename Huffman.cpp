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

size_t before = 0, after = 0;


struct node {
    size_t num;
    int c;
    bool is_leaf  = true;
    node *left = nullptr;
    node *right = nullptr;

    node(){}

    node(node *left_child, node *right_child){
        num = left_child -> num + right_child -> num;
        left = left_child;
        right = right_child;
        is_leaf = false;
    }
};

struct {
	bool operator()(node *first, node *second) {
		return first-> num > second -> num;
	}
} compare;

int binary(std::vector<node*>& it, node *nd, int size) {
    int left_corner = 0, right_corner = size, mid;

    for (;left_corner != right_corner;) {
        mid = (left_corner + right_corner) / 2;
        //std::cout << mid << std::endl;
        if ( it[mid]->num > nd -> num) {
            left_corner = mid + 1;
        }else {
            right_corner = mid;

        }
    }

    return left_corner;
}

node *root = new node;

std::vector<bool> existence_of_chars(258, false);

std::vector <std::string> encoded_symbols(258, "");

void filling_symbols(node *nd, std::string bits) {
    //std::cout << "start" << std::endl;

    if (nd -> is_leaf) {
        //std::cout << "leaf start       "<<nd -> c<<std::endl;
        encoded_symbols[nd -> c + 128] = bits;
        existence_of_chars[nd -> c + 128] = true;

        //std::cout << "leaf end" << std::endl;

        return;
    }
    //std::cout << bits << std::endl;

    if (nd -> left) {
        //std::cout << "go left" << std::endl;
        filling_symbols(nd -> left, bits + std::to_string(1));
    }
    if (nd -> right) {
        //std::cout << "go right" << std::endl;
        filling_symbols(nd -> right, bits + std::to_string(0));
    }
}


void learning_from_file(std::string name) {
    encoded_symbols = {};
    std::ifstream f(name);
    std::map<int, size_t> counting;



    counting[Eof] = 1;
    counting[Esc] = 1;

    std::vector<std::string> strings;
    for (std::string str; std::getline(f, str);) {
        strings.push_back(str);
        before += str.size();
    }
    //std::cout << strings.size() << '\n';

    std::clock_t t1;
    f.close();

    srand(time(0));

    t1 = std::clock();

    if (strings.size() <10000) {
        //std::cout << "less";
        for (auto str = strings.begin(); str != strings.end(); ++str) {
            for (int i = 0; i <  str -> size(); ++i) {
                ++counting[str -> at(i)];
            }
            ++counting['\n'];
        }
    } else {
        for (int k = 0; k < 10000; ++k) {
            size_t l = rand() % strings.size();
            for (int i = 0; i < strings[l].size(); ++i) {
                ++counting[strings[l][i]];
            }
            ++counting['\n'];

        }
    }


    //std::cout << "qweqwe" << std::endl;


    std::vector<node*> nodes;
    for (auto it = counting.begin(); it != counting.end(); ++it) {

        node* n = new node;
        n -> c = it -> first;
        n -> num = it -> second;
        nodes.push_back(n);
    }
    //std::cout << '\n' << "nodes   " << nodes.size() << '\n';

    std::sort(nodes.begin(), nodes.end(), compare);

    for (;nodes.size() != 1;) {



        node *left = nodes.back();
        nodes.pop_back();

        node *right = nodes.back();
        nodes.pop_back();
        node *tmp = new node(left, right);

        nodes.insert(nodes.begin() + binary(nodes, tmp, nodes.size()), tmp);
    }


    root = nodes[0];
    filling_symbols(root, "");

    std::cout << "Learning From file time : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';

}




std::vector<std::string> encoding(std::string name) {
    std::ifstream f(name);
    int c;


    std::clock_t t1;
    t1 = std::clock();


    std::vector<std::string> enc_strings;
    for (std::string str; std::getline(f, str);) {
        enc_strings.push_back("");
        for (int i = 0; i < str.size(); ++i) {
            c = str[i];
            if (existence_of_chars[c + 128]) {
                //std::cout << "qweasdzxc" << std::endl;
                enc_strings[enc_strings.size()-1] += encoded_symbols[c + 128];
            } else {
                enc_strings[enc_strings.size()-1] += encoded_symbols[Esc + 128];
                enc_strings[enc_strings.size()-1] += std::bitset<8>(c).to_string();
            }
        }
    }
    //std::cout << strings.size() << '\n';


    std::cout << "Encoding : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';
    f.close();

    return enc_strings;
}

void decoding(std::vector<std::string> enc_strings) {


    std::clock_t t1;
    t1 = std::clock();

    std::ofstream file_out("decoded_encoded");



    node *nd = root;
    int k = -1;
    int tmp = 0;
    std::string tmp_string;
    for (int j = 0; j < enc_strings.size(); ++j) {
        tmp_string = "";
        std::string encoded = enc_strings[j];
        for (auto i = encoded.begin(); i != encoded.end(); ++i) {
            ++after;
            if (k < 0) {
                if ((*i) == '1') {
                    nd = nd -> left;
                }else {
                    nd = nd -> right;{}
                }
                if (nd -> is_leaf == true) {
                    if (nd -> c != Esc) {
                        //std::cout << char(nd -> c);
                        tmp_string += char(nd -> c);
                        nd = root;
                    } else {

                        k = 7;
                        tmp = 0;
                        nd = root;
                    }
                }
            } else if (k > 0) {

                tmp += (int(*i) - int('0')) << k;
                //std::cout << tmp << "   "<< *i << std::endl;
                --k;
            } else {
                tmp += int(*i) - int('0');
                tmp_string += char(tmp);
                //std::cout << char(tmp);
                --k;

            }

        }
        //std::cout << '\n';
        tmp_string += '\n';
        file_out << tmp_string;
    }
    std::cout << "Decoding : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';
    after /= 8;

    std::cout << "Before : " << before << '\n' << "After : " << after << std::endl;
    file_out.close();


}






int main() {
    std::string input;

    std::cout << "Enter File Path : ";
    std::cin >>  input;
    //input = "data3";
    learning_from_file(input);
    decoding(encoding(input));



    //std::cout << std::bitset<8>(')').to_string();

    return 0;
}
