#include "YaCompLayoutParser.h"
#include "core/YaComponentCore.h"

#include <filesystem>
#include <iostream>


void printHelp()
{
    std::cerr << "yacomplayout <options>" << std::endl;
    std::cerr << "\nOptions:" << std::endl;
    std::cerr << "  --layout <xml>   layout xml file name" << std::endl;
    std::cerr << "  --outcode <dir>  generated code directory" << std::endl;
    std::cerr << "  --verbose        output parser info" << std::endl;
    std::cerr << "  --help           print help" << std::endl;
}

int main(int argc, char** argv)
{
    auto layoutPath = std::filesystem::path();
    auto codePath = std::filesystem::path();
    auto verbose = false;

    for (int idx = 1; idx < argc; idx++) {
        if (std::string(argv[idx]) == std::string("--layout")) {
            layoutPath = (argv[idx + 1]);
            idx++;
        } else if (std::string(argv[idx]).rfind("--layout=", 0) == 0) {
            layoutPath = std::string(argv[idx]).erase(0, std::string("--layout=").length());
        } else if (std::string(argv[idx]) == std::string("--outcode")) {
            codePath = argv[idx + 1];
            idx++;
        } else if (std::string(argv[idx]).rfind("--outcode=", 0) == 0) {
            codePath = std::string(argv[idx]).erase(0, std::string("--outcode=").length());
        } else if (std::string(argv[idx]) == std::string("--verbose")) {
            YaComponentCore::VERBOSE = true;
        } else if (std::string(argv[idx]) == std::string("--help")) {
            printHelp();
            exit(0);
        }
    }

    if (layoutPath.empty()) {
        YaComponentCore::printFatal("missing required argument layout xml file");

    } else {
        if (!std::filesystem::exists(layoutPath)) {
            YaComponentCore::printFatal(std::string("given layout xml file ") + layoutPath.c_str()
                                        + std::string(" not found"));
        } else {
            YaCompLayoutParser parser;
            parser.init(layoutPath, codePath, verbose);
            parser.parse();
        }
    }

    return 0;
}
