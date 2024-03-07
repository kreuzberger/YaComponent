#pragma once

#include <filesystem>
#include <tinyxml2.h>

class YaCompLayoutParser
{
public:
    YaCompLayoutParser();

public:
    void init(const std::filesystem::path &layout, bool verbose);
    void parse();

private:
    void parseDefinitions(tinyxml2::XMLElement *layout);

    std::filesystem::path mCodePath = {};
    std::filesystem::path mLayoutPath = {};
    std::string mBaseName = {};
    bool mVerbose = false;
};
