#include "core/YaComponentCore.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __unix__
std::string YaComponentCore::SEP = "/";
#else
std::string YaComponentCore::SEP = "\\";
#endif

bool YaComponentCore::VERBOSE = false;

YaComponentCore::YaComponentCore() {}

std::vector<std::string> &YaComponentCore::split(const std::string &s,
                                                 char delim,
                                                 std::vector<std::string> &elems)
{
  std::stringstream ss( s );
  std::string       item;
  while ( std::getline( ss, item, delim ) )
  {
    if ( item.length() > 0 )
    {
      elems.push_back( item );
    }
  }
  return elems;
}

std::string YaComponentCore::to_upper(const std::string &str)
{
    auto conv = str;
    std::transform(conv.begin(), conv.end(), conv.begin(), ::toupper);
    return conv;
}

std::string YaComponentCore::to_lower(const std::string &str)
{
    auto conv = str;
    std::transform(conv.begin(), conv.end(), conv.begin(), ::tolower);
    return conv;
}

std::vector<std::string> YaComponentCore::split(const std::string &s, char delim)
{
  std::vector<std::string> elems;
  split( s, delim, elems );
  return elems;
}

void YaComponentCore::printDbg(const std::string &str)
{
    if (YaComponentCore::VERBOSE) {
        std::cout << "info: " << str << std::endl;
    }
}

void YaComponentCore::printWarn(const std::string &str)
{
    std::cout << "warning: " << str << std::endl;
}

void YaComponentCore::printFatal(const std::string &str)
{
  std::cerr << "error: " << str << std::endl;
  exit( 1 );
}

void YaComponentCore::printDbgXml(const tinyxml2::XMLElement *elem)
{
    if (YaComponentCore::VERBOSE) {
        tinyxml2::XMLPrinter printer;
        elem->Accept(&printer);
        std::cout << printer.CStr() << std::endl;
    }
}
