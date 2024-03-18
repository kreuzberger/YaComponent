#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <string>
#include <tinyxml2.h>

class YaCompLayoutCodeGen
{
public:
    YaCompLayoutCodeGen();
    using ElementList = std::list<const tinyxml2::XMLElement *>;
    using AddressMap = std::map<std::string, std::string>;

    void write(const std::filesystem::path &codePath,
               const std::string &componentName,
               const ElementList &processes,
               const AddressMap &adresses);

private:
    using Element = const tinyxml2::XMLElement;
    void writeComponentParts(std::ofstream &fhSource, Element *component, const AddressMap &adresses);
};
