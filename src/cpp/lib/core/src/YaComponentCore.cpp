#include "core/YaComponentCore.h"

#include <sstream>
#include <iostream>

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
