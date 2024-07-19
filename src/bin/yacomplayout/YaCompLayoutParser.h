#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <tinyxml2.h>

class YaCompLayoutParser
{
public:
  YaCompLayoutParser();

public:
  void init( const std::filesystem::path& layout, const std::filesystem::path& code, bool verbose );
  void parse();

private:
  using Element = const tinyxml2::XMLElement;
  using ElementList = std::list<const tinyxml2::XMLElement*>;

  void parseDefinitions( Element* layout );
  std::map<std::string, std::string> parseAddressInformations( const ElementList& processes );

  std::filesystem::path mCodePath = {};
  std::filesystem::path mLayoutPath = {};
  std::string mBaseName = {};
  bool mVerbose = false;
  ElementList mProcesses;
  std::list<std::filesystem::path> mIncludes;
  std::map<std::string, std::string> mAdresses;
};
