//
// Created by impodog on 8/6/2023.
//

#include "ppyl.h"
#include <iostream>
#include <functional>

namespace ppyl {
    using macro_func_type = std::function<void(const string_vec &, string_vec &)>;
    using macro_func_map = std::unordered_map<string, macro_func_type>;

    void push_args(const string_vec &args, string_vec &result) {
        for (const auto &arg: args) {
            result.push_back("<" + arg);
        }
    }

    void rpush_args(const string_vec &args, string_vec &result) {
        for (auto iter = args.rbegin(); iter != args.rend(); ++iter) {
            result.push_back("<" + *iter);
        }
    }

    void macro_push(const string_vec &args, string_vec &result) {
        push_args(args, result);
    }

    void macro_rpush(const string_vec &args, string_vec &result) {
        rpush_args(args, result);
    }

    void macro_array(string_vec args, string_vec &result) {
        if (args.size() < 2) {
            std::cerr << "macro ARRAY need at least 2 args, but got " << args.size() << std::endl;
            return;
        }
        auto array_name = args.back();
        args.pop_back();
        rpush_args(args, result);
        result.push_back("[" + std::to_string(args.size()));
        result.push_back("]" + array_name);
    }

    void macro_call(string_vec args, string_vec &result) {
        auto func_name = args.back();
        args.pop_back();
        rpush_args(args, result);
        result.push_back("." + func_name);
    }

    void macro_show(const string_vec &args, string_vec &result) {
        for (const auto &arg: args) {
            result.push_back("S" + arg);
        }
    }

    void macro_copy(const string_vec &args, string_vec &result) {
        if (args.size() != 2) {
            std::cerr << "macro COPY need 2 args, but got " << args.size() << std::endl;
            return;
        }
        result.push_back("<" + args[0]);
        result.push_back(">" + args[1]);
    }

    macro_func_map macro_map = {
            {"PUSH",  macro_push},
            {"QUEUE", macro_rpush},
            {"ARRAY", macro_array},
            {"CALL",  macro_call},
            {"SHOW",  macro_show},
            {"COPY",  macro_copy},
    };

    void macros(string &str) {
        auto lines = split(str, '\n');
        string_vec result_vec;
        for (auto &line: lines) {
            if (line.front() == '@') {
                size_t colon_pos = line.find(':');
                try {
                    auto &func = macro_map.at(line.substr(1, colon_pos - 1));
                    auto args = split(line.substr(colon_pos + 1), ',');
                    func(args, result_vec);
                } catch (const std::out_of_range &) {
                    std::cerr << "macro " << line.substr(1, colon_pos - 1) << " not found in line \""
                              << line << "\"" << std::endl;
                }
            } else {
                result_vec.push_back(line);
            }
        }
        str = join(result_vec, '\n');
    }
}