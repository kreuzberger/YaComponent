#include "YaCompLayoutParser.h"
#include "YaCompLayoutCodeGen.h"

#include <core/YaComponentCore.h>

YaCompLayoutParser::YaCompLayoutParser() {}

void YaCompLayoutParser::init( const std::filesystem::path& layout, const std::filesystem::path& code, bool verbose )
{
  mLayoutPath = std::filesystem::absolute( layout );
  mBaseName   = std::filesystem::absolute( mLayoutPath ).stem().string();
  if ( !mCodePath.empty() )
  {
    mCodePath = std::filesystem::absolute( code );
  }
  else
  {
    mCodePath = std::filesystem::current_path() / mBaseName / std::filesystem::path( "code" );
  }
  std::filesystem::create_directories( mCodePath );
  mVerbose = verbose;
}

void YaCompLayoutParser::parse()
{
  tinyxml2::XMLDocument doc;
  tinyxml2::XMLError    load_state = doc.LoadFile( mLayoutPath.string().c_str() );
  if ( tinyxml2::XML_SUCCESS == load_state )
  {
  }
  else
  {
    YaComponentCore::printFatal( "cannot read xml file " + mLayoutPath.string() );
  }

  tinyxml2::XMLElement* layout = doc.FirstChildElement( "componentlayout" );
  if ( layout )
  {
    parseDefinitions( layout );
    YaCompLayoutCodeGen code_writer;
    code_writer.write( mCodePath, mBaseName, mProcesses, mAdresses );
  }
  else
  {
    YaComponentCore::printFatal( "expected <componentlayout> as first tag" );
  }
}

void YaCompLayoutParser::parseDefinitions( Element* layout )
{
  auto* include = layout->FirstChildElement( "include" );
  while ( include )
  {
    const char* file = include->Attribute( "file" );
    if ( file )
    {
      YaComponentCore::printDbg( std::string( "found include " ) + std::string( file ) );
      mIncludes.push_back( std::filesystem::path( file ) );
    }
    include = include->NextSiblingElement( "include" );
  }

  const tinyxml2::XMLElement* processes = layout->FirstChildElement( "processes" );
  while ( processes )
  {
    auto* process = processes->FirstChildElement( "process" );
    while ( process )
    {
      const char* name = process->Attribute( "name" );
      if ( name )
      {
        YaComponentCore::printDbg( std::string( "found process " ) + std::string( name ) );
      }
      mProcesses.push_back( process );
      process = process->NextSiblingElement( "process" );
    }
    processes = processes->NextSiblingElement( "processes" );
  }

  if ( !mProcesses.empty() )
  {
    mAdresses = parseAddressInformations( mProcesses );
  }
}

std::map<std::string, std::string> YaCompLayoutParser::parseAddressInformations( const ElementList& processes )
{
  std::map<std::string, std::string> adresses;
  for ( auto* process : processes )
  {
    auto* thread = process->FirstChildElement( "thread" );
    while ( thread )
    {
      auto* component = thread->FirstChildElement( "component" );
      while ( component )
      {
        auto* provides = component->FirstChildElement( "provides" );
        while ( provides )
        {
          auto* ifc = provides->FirstChildElement( "interface" );
          while ( ifc )
          {
            auto id = std::string( component->Attribute( "name" ) ) + std::string( "." ) + std::string( ifc->Attribute( "id" ) );
            adresses.insert_or_assign( id, std::string( ifc->Attribute( "address" ) ) );
            ifc = ifc->NextSiblingElement( "interface" );
          }

          provides = provides->NextSiblingElement( "provides" );
        }
        component = component->NextSiblingElement( "component" );
      }

      thread = thread->NextSiblingElement( "thread" );
    }
  }
  return adresses;
}
