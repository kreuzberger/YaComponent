#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <tinyxml2.h>
class YaComponentDocGen
{
public:
  YaComponentDocGen();
  using ElementList = std::list<const tinyxml2::XMLElement*>;
  using EntryList   = std::list<std::map<std::string, std::string>>;

  void writeIfc( const std::filesystem::path& docPath,
                 const std::string&           ifcName,
                 const ElementList&           properties,
                 const ElementList&           requests,
                 const ElementList&           responses,
                 const ElementList&           includes );

  void writeComponent( const std::filesystem::path& docPath, const std::string& compName, const EntryList& providedIfc, const EntryList& usedIfc );
};
