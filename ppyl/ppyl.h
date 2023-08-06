//
// Created by impodog on 8/6/2023.
//

#ifndef PPYL_PPYL_H
#define PPYL_PPYL_H

#include <string>
#include <filesystem>
#include <vector>

namespace ppyl {
    using std::string;
    using std::filesystem::path;
    using string_vec = std::vector<string>;

    bool is_symbol(char c);

    bool is_space(char c);

    bool is_spaces(const std::string &str);

    char to_escaped(char c);

    std::string to_unescaped(char c);

    string_vec split(const string &str, char c);

    std::string join(const string_vec &vec, char c);


    void format(string &str);

    void work(const path &src, const path &dst);
}

#endif //PPYL_PPYL_H
