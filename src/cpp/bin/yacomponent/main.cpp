#include "YaComponentParser.h"

#include <core/YaComponentCore.h>
#include <filesystem>
#include <iostream>

void printHelp()
{
    std::cerr << "yacomponent <options>" << std::endl;
    std::cerr << "\n  generates component or interface code for given component" << std::endl;
    std::cerr << "  or given interface." << std::endl;
    std::cerr << "\nOptions:" << std::endl;
    std::cerr << "  --component <xml> component xml file name" << std::endl;
    std::cerr << "  --ifc <xml>       interface xml file name" << std::endl;
    std::cerr << "  --outcode <dir>   generated code directory" << std::endl;
    std::cerr << "  --verbose         output parser info" << std::endl;
    std::cerr << "  --help           print help" << std::endl;
}

int main(int argc, char** argv)
{
    auto componentPath = std::filesystem::path();
    auto ifcPath = std::filesystem::path();
    auto codePath = std::filesystem::path();
    auto verbose = false;

    for (int idx = 1; idx < argc; idx++) {
        if (std::string(argv[idx]) == std::string("--component")) {
            componentPath = (argv[idx + 1]);
            idx++;
        } else if (std::string(argv[idx]).rfind("--component=", 0) == 0) {
            componentPath = std::string(argv[idx]).erase(0, std::string("--component=").length());
        } else if (std::string(argv[idx]) == std::string("--ifc")) {
            ifcPath = (argv[idx + 1]);
            idx++;
        } else if (std::string(argv[idx]).rfind("--ifc=", 0) == 0) {
            ifcPath = std::string(argv[idx]).erase(0, std::string("--ifc=").length());
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

    if (componentPath.empty() && ifcPath.empty()) {
        YaComponentCore::printFatal("missing required argument component or interface xml file");
    }
    if (!componentPath.empty() && !ifcPath.empty()) {
        YaComponentCore::printFatal("only one of --component or --ifc arguments allowed");
    }

    if (!componentPath.empty() && !std::filesystem::exists(componentPath)) {
        YaComponentCore::printFatal(std::string("given component xml file ") + componentPath.c_str()
                                    + std::string(" not found"));
    }
    if (!ifcPath.empty() && !std::filesystem::exists(ifcPath)) {
        YaComponentCore::printFatal(std::string("given ifc xml file ") + ifcPath.c_str()
                                    + std::string(" not found"));
    }

    YaComponentParser parser;
    parser.init(codePath, verbose);

    if (!componentPath.empty()) {
        parser.parseComponent(componentPath);
    } else if (!ifcPath.empty()) {
        parser.parseIfc(ifcPath);
    }

    return 0;
}
