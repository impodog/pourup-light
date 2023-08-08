//
// Created by impodog on 8/6/2023.
//

#ifndef PPYL_PPYL_H
#define PPYL_PPYL_H

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

#define PPYL_RESERVED "__"

namespace ppyl {
    using std::string;
    using std::filesystem::path;
    using string_vec = std::vector<string>;
    constexpr const char *anonymous = PPYL_RESERVED "anonymous_";

    bool is_symbol(char c);

    bool is_space(char c);

    bool is_spaces(const std::string &str);

    string to_unescaped(char c);

    string_vec split(const string &str, char c);

    string join(const string_vec &vec, char c);

    string join(const string_vec &vec, char c, std::string begin);

    string rjoin(const string_vec &vec, char c);

    string rjoin(const string_vec &vec, char c, std::string begin);

    string get_anonymous();

    void format(string &str);

    void macros(string &str);

    void work(const path &src, const path &dst);

    void json_work(const path &src, const path &dst);
}

#endif //PPYL_PPYL_H
