#pragma once

#include <string>
#include <tinyxml2.h>
#include <vector>

class YaComponentCore
{
public:
    YaComponentCore();

    static void printDbg(const std::string &);
    static void printWarn(const std::string &);
    static void printFatal(const std::string &);
    static void printDbgXml(const tinyxml2::XMLElement *);

    static std::vector<std::string> split(const std::string &s, char delim);
    static std::string to_lower(const std::string &str);
    static std::string to_upper(const std::string &str);

    static std::string SEP;

    static bool VERBOSE;

private:
    static std::vector<std::string> &split(const std::string &s,
                                           char delim,
                                           std::vector<std::string> &elems);
};

