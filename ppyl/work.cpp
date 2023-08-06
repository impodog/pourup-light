//
// Created by impodog on 8/6/2023.
//

#include "ppyl.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace ppyl {
    bool is_symbol(char c) {
        switch (c) {
            case '<':
            case '>':
            case '=':
            case '[':
            case ']':
            case '{':
            case '}':
                return true;
            default:
                return false;
        }
    }

    bool is_space(char c) {
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                return true;
            default:
                return false;
        }
    }

    bool is_spaces(const string &str) {
        return std::all_of(str.begin(), str.end(), is_space);
    }

    char to_escaped(char c) {
        switch (c) {
            case '\n':
                return 'n';
            case '\r':
                return 'r';
            case '\t':
                return 't';
            default:
                return c;
        }
    }

    string to_unescaped(char c) {
        switch (c) {
            case '\n':
                return "\\n";
            case '\r':
                return "\\r";
            case '\t':
                return "\\t";
            default:
                string result;
                result.push_back(c);
                return result;
        }
    }

    string_vec split(const string &str, char c) {
        string_vec result;
        string tmp;
        for (auto ch: str) {
            if (ch == c) {
                result.push_back(tmp);
                tmp.clear();
            } else {
                tmp.push_back(ch);
            }
        }
        if (!tmp.empty()) {
            result.push_back(tmp);
        }
        return result;
    }

    string join(const string_vec &vec, char c) {
        string result;
        for (auto &str: vec) {
            result += str;
            result.push_back(c);
        }
        return result;
    }

    void work(const path &src, const path &dst) {
       string str;
        {
            std::ifstream ifs(src);
            if (!ifs.is_open()) {
                std::cerr << "Cannot open file: " << src << std::endl;
                return;
            }
            str = string((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        }
        format(str);
        {
            std::ofstream ofs(dst);
            if (!ofs.is_open()) {
                std::cerr << "Cannot open file: " << dst << std::endl;
                return;
            }
            ofs << str;
        }
    }


}