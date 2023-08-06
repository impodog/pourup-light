//
// Created by impodog on 8/6/2023.
//

#include "ppyl.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

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
        return join(vec, c, "");
    }

    string join(const string_vec &vec, char c, std::string begin) {
        if (vec.empty()) {
            return begin;
        }
        begin.append(vec.front());
        for (auto iter = ++vec.begin(); iter != vec.end(); ++iter) {
            begin.push_back(c);
            begin.append(*iter);
        }
        return begin;
    }

    string rjoin(const string_vec &vec, char c) {
        return rjoin(vec, c, "");
    }

    string rjoin(const string_vec &vec, char c, std::string begin) {
        if (vec.empty()) {
            return begin;
        }
        begin.append(vec.back());
        for (auto iter = ++vec.rbegin(); iter != vec.rend(); ++iter) {
            begin.push_back(c);
            begin.append(*iter);
        }
        return begin;
    }

    string get_anonymous() {
        static size_t count = 0;
        size_t value = count++;
        string result = anonymous;
        while (value) {
            result.push_back(static_cast<char>(value % 26 + 'a'));
            value /= 26;
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
        macros(str);
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