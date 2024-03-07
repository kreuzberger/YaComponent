
#include <filesystem>
#include <iostream>

#include "YaCompLayoutParser.h"
#include "core/YaComponentCore.h"

void printHelp()
{
    std::cerr << "yacomplayout <options>" << std::endl;
    std::cerr << "\nOptions:" << std::endl;
    std::cerr << "  --layout <xml>   layout xml file name" << std::endl;
    std::cerr << "  --outcode <dir>  generated code directory" << std::endl;
    std::cerr << "  --verbose        output parser info" << std::endl;
}

int main(int argc, char** argv)
{
    std::cout << "yacomplayout - Code Generator for YaComponent Layout" << std::endl;

    auto layout = std::filesystem::path();
    auto verbose = false;

    bool ret = true;

    for (int idx = 1; idx < argc; idx++) {
        if (std::string(argv[idx]) == std::string("--layout")) {
            layout = (argv[idx + 1]);
            idx++;
        } else if (std::string(argv[idx]) == std::string("--outcode")) {
            std::string codePath = argv[idx + 1];
            idx++;
        } else if (std::string(argv[idx]) == std::string("--verbose")) {
            YaComponentCore::VERBOSE = true;
        } else if (std::string(argv[idx]) == std::string("--help")) {
            printHelp();
            exit(0);
        }
    }

    if (layout.empty()) {
        std::cerr << "error: missing required argument layout xml file" << std::endl;
        ret = false;

    } else {
        if (!std::filesystem::exists(layout)) {
            std::cerr << "error: given layout xml file" << layout << "not found" << std::endl;
        } else {
            YaCompLayoutParser parser;
            parser.init(layout, verbose);
            parser.parse();
        }
    }

    return (ret) ? 0 : 1;
}
