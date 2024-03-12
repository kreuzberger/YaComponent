#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <tinyxml2.h>

class YaComponentCodeGen
{
public:
    YaComponentCodeGen();
    using ElementList = std::list<const tinyxml2::XMLElement *>;
    using EntryList = std::list<std::map<std::string, std::string>>;

    void writeComponent(const std::filesystem::path &codePath,
                        const std::string &compName,
                        const EntryList &providedIfc,
                        const EntryList &usedIfc);
    void writeIfc(const std::filesystem::path &codePath,
                  const std::string &ifcName,
                  const ElementList &properties,
                  const ElementList &requests,
                  const ElementList &responses,
                  const ElementList &includes);

private:
    void writeIfcProxy(const std::filesystem::path &codePath,
                       const std::string &ifcName,
                       const ElementList &properties,
                       const ElementList &requests,
                       const ElementList &responses,
                       const ElementList &includes);

    void writeIfcStub(const std::filesystem::path &codePath,
                      const std::string &ifcName,
                      const ElementList &properties,
                      const ElementList &requests,
                      const ElementList &responses,
                      const ElementList &includes);
};
