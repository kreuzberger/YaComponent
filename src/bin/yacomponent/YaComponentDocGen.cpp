#include "YaComponentDocGen.h"
#include <core/YaComponentCore.h>

#include <fstream>
#include <iostream>

YaComponentDocGen::YaComponentDocGen() {}

void YaComponentDocGen::writeIfc(const std::filesystem::path &docPath,
                                 const std::string &ifcName,
                                 const ElementList &properties,
                                 const ElementList &requests,
                                 const ElementList &responses,
                                 const ElementList &includes)
{
    std::ofstream fhDoc;

    auto docFilename = (docPath / (ifcName + ".txt")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentDocGen:: write component ifc documentation file") + docFilename);

    fhDoc.open(docFilename, std::ios::out | std::ios::trunc);

    for (const auto *include : includes) {
    }

    fhDoc.close();
}

void YaComponentDocGen::writeComponent(const std::filesystem::path &docPath,
                                       const std::string &compName,
                                       const EntryList &providedIfc,
                                       const EntryList &usedIfc)
{
    std::ofstream fhDoc;

    auto docFilename = (docPath / (compName + ".txt")).string();

    YaComponentCore::printDbg(std::string("YaComponentDocGen:: write component documentation file")
                              + docFilename);

    fhDoc.open(docFilename, std::ios::out | std::ios::trunc);

    auto strCompDefine = std::string("!definelong COMP_") + YaComponentCore::to_upper(compName)
                         + "(name";

    strCompDefine += ")\n";

    strCompDefine += "  [name] << " + compName + " >>\n";

    for (const auto &ifc : providedIfc) {
        strCompDefine += "  interface " + ifc.at("id") + " << " + ifc.at("classname")
                         + " >> as name." + ifc.at("id") + "\n";
        strCompDefine += "  name." + ifc.at("id") + " << " + ifc.at("classname") + " - [name]\n";
    }

    strCompDefine += "!enddefinelong\n";

    fhDoc << strCompDefine << std::endl;

    fhDoc.close();
}
