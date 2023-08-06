//
// Created by impodog on 8/6/2023.
//
#include "ppyl.h"

namespace ppyl {
    void format(string &str) {
        bool in_string = false, is_escape = false;
        string result;
        for (auto c: str) {
            if (is_escape) {
                result.push_back(to_escaped(c));
                is_escape = false;
                continue;
            }
            switch (c) {
                case '\\':
                    is_escape = true;
                    break;
                case '"':
                    if (in_string) {
                        result.push_back('$');
                    }
                case '\'': //fallthrough
                    in_string = !in_string;
                    break;
                default:
                    if (in_string) {
                        result.append(to_unescaped(c));
                    } else {
                        if (is_space(c)) {
                            continue;
                        }
                        if (is_symbol(c)) {
                            result.push_back('\n');
                        }
                        result.push_back(c);
                    }
                    break;
            }
        }
        auto lines = split(result, '\n');
        string_vec result_vec;
        for (auto &line: lines) {
            if (!is_spaces(line)) {
                result_vec.push_back(line);
            }
        }
        result = join(result_vec, '\n');
        str = result;
    }
}