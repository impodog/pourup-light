//
// Created by impodog on 8/6/2023.
//
#include "ppyl.h"
#include <fstream>
#include "json/json.hpp"


namespace ppyl {
    using nlohmann::json;
    constexpr const char *ppyl_config = "_ppyl";

    void set_value_name(string &result, const string &str, const string &name) {
        if (name.empty()) {
            result.append(str);
        } else {
            result.push_back('<');
            result.append(str);
            result.push_back('>');
            result.append(name);
        }
    }

    string convert_key(const string &key) {
        return "'" + key + "'";
    }

    string convert_json(const json &j, const string &name_raw);

    void convert_json_object(string &result, json j, const string &name) {
        json config;
        if (j.contains(ppyl_config) && (config = j.at(ppyl_config)).is_array()) {
            config = j.at(ppyl_config);
            j.erase(ppyl_config);
        }
        auto add_objects = [&j, &result] {
            for (auto it = j.begin(); it != j.end(); ++it) {
                result.append(convert_json(it.value(), it.key()));
                result.push_back('\n');
            }
        };
        if (name.empty()) {
            add_objects();
        } else {
            result.push_back('{');
            result.append(name).push_back('\n');
            add_objects();
            result.push_back('}');
            result.push_back('\n');
        }
        if (!config.is_null()) {
            result.push_back('\n');
            for (const auto &line: config) {
                result.append(line.get<string>());
                result.push_back('\n');
            }
        }
    }

    void convert_json_array(string &result, const json &j, const string &name) {
        std::string temp = "@ARRAY:";
        for (auto it = j.begin(); it != j.end(); ++it) {
            if (it.value().is_object()) {
                auto ano_name = get_anonymous();
                result.append(convert_json(it.value(), ano_name));
                temp.append(ano_name);
            } else {
                temp.append(convert_json(it.value(), ""));
            }
            temp.push_back(',');
        }
        // The trailing comma is used because the additional name is added to the end of the array.
        result.append(temp);
        result.append(name);
    }

    string convert_json(const json &j, const string &name_raw) {
        string result, name;
        if (name_raw.empty()) {
            name = "";
        } else {
            name = convert_key(name_raw);
        }

        if (j.is_object()) {
            convert_json_object(result, j, name);
        } else if (j.is_array()) {
            convert_json_array(result, j, name);
        } else if (j.is_string() || j.is_number()) {
            set_value_name(result, j.dump(), name);
        } else if (j.is_boolean()) {
            set_value_name(result, j.get<bool>() ? "#T" : "#F", name);
        } else if (j.is_null()) {
            set_value_name(result, "#N", name);
        }
        return result;
    }

    void json_work(const path &src, const path &dst) {
        json j;
        {
            std::ifstream ifs(src);
            ifs >> j;
        }
        {
            std::ofstream ofs(dst);
            ofs << convert_json(j, "");
        }
    }
}