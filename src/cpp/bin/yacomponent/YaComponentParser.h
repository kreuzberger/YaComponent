
#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <tinyxml2.h>
#include <tuple>

class YaComponentParser
{
public:
    YaComponentParser();

    void init(const std::filesystem::path &code, const std::filesystem::path &doc, bool verbose);
    void parseComponent(const std::filesystem::path &component);
    void parseIfc(const std::filesystem::path &ifc);

private:
    using Element = const tinyxml2::XMLElement;
    using ElementList = std::list<const tinyxml2::XMLElement *>;
    using Entry = std::map<std::string, std::string>;
    using EntryList = std::list<Entry>;

    void parseDefinitions(Element *layout);
    std::tuple<EntryList, EntryList> parseComponentDefinitions(Element *component);
    std::tuple<ElementList, ElementList, ElementList, ElementList> parseIfcDefinitions(Element *ifc);
    //std::map<std::string, std::string> parseAddressInformations(const ElementList &processes);

    std::filesystem::path mComponentPath = {};
    std::filesystem::path mIfcPath = {};
    std::filesystem::path mCodePath = {};
    std::filesystem::path mDocPath = {};
    std::string mBaseName = {};
    bool mVerbose = false;
};
