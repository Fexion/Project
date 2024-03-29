#include "huffman.h"

namespace codecs {
    size_t before = 0, after = 0;

    std::vector<node*> tree;


    node::node(size_t left_child, size_t right_child){
        num = tree[left_child] -> num + tree[right_child] -> num;
        left = left_child;
        right = right_child;
        is_leaf = false;
    }

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


    size_t root;
    std::vector<bool> existence_of_chars(258, false);
    std::vector <std::vector<bool>> encoded_symbols(258, {0});


    void filling_symbols(size_t nd, std::vector<bool> bits,std::vector <std::vector<bool>>& encoded_symbols1) {
        //std::cout << "start" << std::endl;
        if (tree[nd] -> is_leaf) {
            //std::cout << "leaf start       "<<nd -> c<<std::endl;
            encoded_symbols1[tree[nd] -> c + 128] = bits;
            //encoded_symbols[tree[nd] -> c + 128] = {};

            existence_of_chars[tree[nd] -> c + 128] = true;

            //std::cout << "leaf end" << std::endl;

            return;
        }
        //std::cout << bits << std::endl;

        if (!(tree[nd] -> is_leaf)) {
            //std::cout << "go left" << std::endl;
            bits.push_back(1);
            filling_symbols(tree[nd] -> left, bits,encoded_symbols1);
            bits.erase(bits.end() - 1);

        }
        if (!(tree[nd] -> is_leaf)) {
            //std::cout << "go right" << std::endl;
            bits.push_back(0);
            filling_symbols(tree[nd] -> right, bits, encoded_symbols1);
            bits.erase(bits.end() - 1);
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
                int64_t l = rand() % strings.size();
                for (int i = 0; i < strings[l].size(); ++i) {
                    ++counting[strings[l][i]];
                }
                ++counting['\n'];

            }
        }
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



            size_t left = tree.size();
            tree.push_back(nodes.back());
            nodes.pop_back();

            size_t right = tree.size();
            tree.push_back(nodes.back());
            nodes.pop_back();
            node *tmp = new node(left, right);

            nodes.insert(nodes.begin() + binary(nodes, tmp, nodes.size()), tmp);
        }


        root = tree.size();
        tree.push_back(nodes[0]);
        std::vector<bool> q;
        std::vector <std::vector<bool>> encoded_symbols1(258);
        filling_symbols(root, q, encoded_symbols1);
        encoded_symbols = encoded_symbols1;
        std::cout << "Learning From file time : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';

    }



    std::vector<std::vector <bool> > encoding(const std::string name) {
        std::ifstream f(name);
        int c;


        std::clock_t t1;
        t1 = std::clock();

        std::string tmp;

        std::vector<std::vector<bool>> enc_strings;
        for (std::string str; std::getline(f, str);) {
            enc_strings.push_back({});
            for (int i = 0; i < str.size(); ++i) {
                c = str[i];
                if (existence_of_chars[c + 128]) {
                    //std::cout << "qweasdzxc" << std::endl;
                    for (int i = 0; i < encoded_symbols[c + 128].size(); ++i) {
                        enc_strings[enc_strings.size()-1].push_back(encoded_symbols[c + 128][i]);
                    }
                } else {
                    for (int i = 0; i < encoded_symbols[Esc + 128].size(); ++i) {
                        enc_strings[enc_strings.size()-1].push_back(encoded_symbols[Esc + 128][i]);
                    }
                    tmp = std::bitset<8>(c).to_string();
                    for (int i = 0; i < tmp.size(); ++i) {
                        enc_strings[enc_strings.size()-1].push_back(tmp[i] - '0');
                    }
                }
            }
        }
        //std::cout << strings.size() << '\n';


        std::cout << "Encoding : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';
        f.close();



        return enc_strings;
    }

    void decoding(const std::vector<std::vector<bool>>& enc_strings) {


        std::clock_t t1;
        t1 = std::clock();

        std::ofstream file_out("decoded_encoded");



        size_t nd = root;
        int k = -1;
        int tmp = 0;
        std::string tmp_string;
        for (int j = 0; j < enc_strings.size(); ++j) {
            tmp_string = "";
            std::vector<bool> encoded = enc_strings[j];
            for (auto i = encoded.begin(); i != encoded.end(); ++i) {
                ++after;
                if (k < 0) {
                    if ((*i) == 1) {
                        nd = tree[nd] -> left;
                    }else {
                        nd = tree[nd] -> right;
                    }
                    if (tree[nd] -> is_leaf) {
                        if (tree[nd] -> c != Esc) {
                            //std::cout << char(nd -> c);
                            tmp_string += char(tree[nd] -> c);
                            nd = root;
                        } else {

                            k = 7;
                            tmp = 0;
                            nd = root;
                        }
                    }
                } else if (k > 0) {

                    tmp += (int(*i)) << k;
                    //std::cout << tmp << "   "<< *i << std::endl;
                    --k;
                } else {
                    tmp += int(*i);
                    tmp_string += char(tmp);
                    //std::cout << char(tmp);
                    --k;

                }

            }
            tmp_string += '\n';
            file_out << tmp_string;
        }
        std::cout << "Decoding : " <<(std::clock() - t1)/(double) CLOCKS_PER_SEC << '\n';
        after /= 8;

        std::cout << "Before : " << before << '\n' << "After : " << after << std::endl;
        file_out.close();


    }

    void clear_func() {
        for (size_t i =0; i < tree.size(); ++i) {
            delete tree[i];
        }
    }
    bool f = true;
    void smn_help_me() {
        std::string choice;
        if (f) {
        std::cout << "Hello and welcome to my encoder\nIt's slow, bad, but it works(most of time)\nType \"help\" to find out how to use it or, in case you've been here before, just start working: ";
            f = false;
        } else {
            std::cout << "Hello again, u know what to do and how to do this time: ";
        }

        std::cin >> choice;
        if (choice == "encode") {
            std::cout << "what do u want to encode(Enter path to file): ";
            std::string path;
            std::cin >> path;
            codecs::learning_from_file(path);
            codecs::decoding(codecs::encoding(path));
            clear_func();
        }
        if (choice == "exit") {
            clear_func();
            return;
        }
        if (choice == "help") {
            std::cout << "Type \"encode\" to encode your file(thanks, cap)" << std::endl;
            std::cout << "  also u'll be provided with some information about how fast(slow) works my program" << std::endl;
            std::cout << "  and how hard can it encode your file" << std::endl;
            std::cout << "Type \"exit\" to exit the program" << std::endl;
            smn_help_me();
        }
    }

}
