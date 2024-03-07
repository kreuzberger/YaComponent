#include "YaCompLayoutParser.h"

#include <core/YaComponentCore.h>

YaCompLayoutParser::YaCompLayoutParser() {}

void YaCompLayoutParser::init(const std::filesystem::path &layout, bool verbose)
{
    mLayoutPath = std::filesystem::absolute(layout);
    mBaseName = std::filesystem::absolute(mLayoutPath).stem();
    mCodePath = std::filesystem::current_path() / mBaseName / std::filesystem::path("code");
    std::filesystem::create_directories(mCodePath);
    mVerbose = verbose;
}

void YaCompLayoutParser::parse()
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError load_state = doc.LoadFile(mLayoutPath.c_str());
    if (tinyxml2::XML_SUCCESS == load_state) {
    } else {
        YaComponentCore::printFatal("cannot read xml file " + mLayoutPath.string());
    }

    tinyxml2::XMLElement *layout = doc.FirstChildElement("componentlayout");
    if (layout) {
        parseDefinitions(layout);
    }
    //writeCodeFiles();
}

void YaCompLayoutParser::parseDefinitions(tinyxml2::XMLElement *layout)
{
    const tinyxml2::XMLElement *include = layout->FirstChildElement("include");
    while (include) {
        const char *file = include->Attribute("file");
        if (file) {
            YaComponentCore::printDbg(file);
        }
        include = layout->NextSiblingElement("include");
    }

    const tinyxml2::XMLElement *processes = layout->FirstChildElement("processes");
    while (processes) {
        const tinyxml2::XMLElement *process = processes->FirstChildElement("process");
        while (process) {
            const char *name = process->Attribute("name");
            if (name) {
                YaComponentCore::printDbg(name);
            }
            process = processes->NextSiblingElement("process");
        }
        processes = layout->NextSiblingElement("processes");
    }
}
