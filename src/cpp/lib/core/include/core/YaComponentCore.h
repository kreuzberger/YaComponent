#pragma once

#include <vector>
#include <string>
class YaComponentCore
{
public:
    YaComponentCore();

    static void printDbg(const std::string &);
    static void printWarn(const std::string &);
    static void printFatal(const std::string &);

    static std::vector<std::string> split(const std::string &s, char delim);
    static std::string SEP;

    static bool VERBOSE;

private:
    static std::vector<std::string> &split(const std::string &s,
                                           char delim,
                                           std::vector<std::string> &elems);
};

