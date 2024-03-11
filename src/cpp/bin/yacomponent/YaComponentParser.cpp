
#include "YaComponentParser.h"
#include "YaComponentCodeGen.h"
#include "YaComponentDocGen.h"

#include <core/YaComponentCore.h>
YaComponentParser::YaComponentParser() {}

void YaComponentParser::init(

    const std::filesystem::path &code, const std::filesystem::path &doc, bool verbose)
{
    if (!code.empty()) {
        mCodePath = std::filesystem::absolute(code);
    } else {
        mCodePath = std::filesystem::current_path() / mBaseName / std::filesystem::path("code");
    }
    if (!doc.empty()) {
        mDocPath = std::filesystem::absolute(doc);
    } else {
        mDocPath = std::filesystem::current_path() / mBaseName / std::filesystem::path("doc");
    }
    std::filesystem::create_directories(mCodePath);
    std::filesystem::create_directories(mDocPath);
    mVerbose = verbose;
}

void YaComponentParser::parseComponent(const std::filesystem::path &componentPath)
{
    mComponentPath = std::filesystem::absolute(componentPath);
    mBaseName = std::filesystem::absolute(mComponentPath).stem();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError load_state = doc.LoadFile(mComponentPath.c_str());
    if (tinyxml2::XML_SUCCESS == load_state) {
    } else {
        YaComponentCore::printFatal("cannot read xml file " + mComponentPath.string());
    }

    YaComponentCore::printDbg("parsing component file " + mComponentPath.string());

    auto *component = doc.RootElement();
    auto [providedIfc, usedIfc] = parseComponentDefinitions(component);

    YaComponentCodeGen code_writer;
    code_writer.writeComponent(mCodePath, mBaseName, providedIfc, usedIfc);
    YaComponentDocGen doc_writer;
    doc_writer.writeComponent(mDocPath, mBaseName, providedIfc, usedIfc);
}

void YaComponentParser::parseIfc(const std::filesystem::path &ifcPath)
{
    mIfcPath = std::filesystem::absolute(ifcPath);
    auto ifcBaseName = std::filesystem::absolute(mIfcPath).stem();

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError load_state = doc.LoadFile(mIfcPath.c_str());
    if (tinyxml2::XML_SUCCESS == load_state) {
    } else {
        YaComponentCore::printFatal("cannot read xml file " + mIfcPath.string());
    }

    YaComponentCore::printDbg("parsing interface file " + mIfcPath.string());

    auto *ifc = doc.RootElement();
    auto [properties, requests, responses, includes] = parseIfcDefinitions(ifc);

    YaComponentCodeGen code_writer;
    code_writer.writeIfc(mCodePath, ifcBaseName, properties, requests, responses, includes);
    YaComponentDocGen doc_writer;
    doc_writer.writeIfc(mDocPath, ifcBaseName, properties, requests, responses, includes);
}

std::tuple<YaComponentParser::EntryList, YaComponentParser::EntryList>
YaComponentParser::parseComponentDefinitions(Element *component)
{
    auto ifcProvided = EntryList();
    auto ifcUsed = EntryList();

    auto *provides = component->FirstChildElement("provides");
    while (provides) {
        auto *ifc = provides->FirstChildElement("interface");
        while (ifc) {
            if (!ifc->Attribute("id") || !ifc->Attribute("xml")) {
                YaComponentCore::printFatal(
                    "YaComponentParser:: missing definition of interface id or xml file");
            }
            auto id = std::string(ifc->Attribute("id"));
            auto xml = std::string(ifc->Attribute("xml"));

            YaComponentCore::printDbg(std::string("YaComponentParser: id:") + id + ", xml:" + xml);
            Entry entry;
            entry.insert_or_assign("id", id);
            entry.insert_or_assign("classname", std::filesystem::path(xml).stem());
            ifcProvided.push_back(entry);
            ifc = ifc->NextSiblingElement("interface");
        }

        provides = provides->NextSiblingElement("provides");
    }

    auto *uses = component->FirstChildElement("uses");
    while (uses) {
        auto *ifc = uses->FirstChildElement("interface");
        while (ifc) {
            if (!ifc->Attribute("id") || !ifc->Attribute("xml")) {
                YaComponentCore::printFatal(
                    "YaComponentParser:: missing definition of interface id or xml file");
            }
            auto id = std::string(ifc->Attribute("id"));
            auto xml = std::string(ifc->Attribute("xml"));

            YaComponentCore::printDbg(std::string("YaComponentParser: id:") + id + ", xml:" + xml);
            Entry entry;
            entry.insert_or_assign("id", id);
            entry.insert_or_assign("classname", std::filesystem::path(xml).stem());
            ifcUsed.push_back(entry);
            ifc = ifc->NextSiblingElement("interface");
        }

        uses = uses->NextSiblingElement("uses");
    }

    return std::make_tuple(ifcProvided, ifcUsed);
}

std::tuple<YaComponentParser::ElementList,
           YaComponentParser::ElementList,
           YaComponentParser::ElementList,
           YaComponentParser::ElementList>
YaComponentParser::parseIfcDefinitions(Element *ifc)
{
    auto properties = ElementList();
    auto requests = ElementList();
    auto responses = ElementList();
    auto includes = ElementList();

    auto *prop = ifc->FirstChildElement("properties")->FirstChildElement("prop");
    while (prop) {
        if (!prop->Attribute("id")) {
            YaComponentCore::printFatal("missing definition of property id");
        }

        if (!prop->Attribute("onChange")) {
            const_cast<tinyxml2::XMLElement *>(prop)->SetAttribute("onChange", "0");
        }
        YaComponentCore::printDbg(std::string("property id: ") + std::string(prop->Attribute("id"))
                                  + ", onChange: " + std::string(prop->Attribute("onChange")));
        properties.push_back(prop);
        prop = prop->NextSiblingElement("prop");
    }

    auto *req = ifc->FirstChildElement("requests")->FirstChildElement("req");
    while (req) {
        YaComponentCore::printDbg(std::string("request id: ") + std::string(req->Attribute("id")));
        requests.push_back(req);
        req = req->NextSiblingElement("req");
    }

    auto *resp = ifc->FirstChildElement("responses")->FirstChildElement("resp");
    while (resp) {
        YaComponentCore::printDbg(std::string("response id: ") + std::string(resp->Attribute("id")));
        responses.push_back(resp);
        resp = resp->NextSiblingElement("resp");
    }

    auto *include = ifc->FirstChildElement("include");
    while (include) {
        YaComponentCore::printDbg(std::string("include file: ")
                                  + std::string(include->Attribute("file")));
        includes.push_back(include);
        include = include->NextSiblingElement("include");
    }
    return std::make_tuple(properties, requests, responses, includes);
}
