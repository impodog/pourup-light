#include "ppyl.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include "argparse/include/argparse/argparse.hpp"

int main(int argc, char **argv) {
    argparse::ArgumentParser program("ppyl");
    program.add_argument("src")
            .help("the ppyl source file to compile");
    program.add_argument("-o")
            .help("the output file(in pupl format)");
    program.parse_args(argc, argv);
    std::filesystem::path src = program.get<std::string>("src"),
            output;
    if (auto output_arg = program.present("-o")) {
        output = *output_arg;
    } else {
        output = src.parent_path() / (src.stem().string() + ".pupl");
    }
    ppyl::work(src, output);
    return 0;
}
