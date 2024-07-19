#include "YaComponentDocGen.h"
#include <core/YaComponentCore.h>

#include <fstream>
#include <iostream>

YaComponentDocGen::YaComponentDocGen() {}

void YaComponentDocGen::writeIfc( const std::filesystem::path& docPath,
                                  const std::string& ifcName,
                                  const ElementList& properties,
                                  const ElementList& requests,
                                  const ElementList& responses,
                                  const ElementList& includes )
{
  std::ofstream fhDoc;

  auto docFilename = ( docPath / ( ifcName + ".uml" ) ).string();

  YaComponentCore::printDbg( std::string( "YaComponentDocGen:: write component ifc documentation file" ) + docFilename );

  fhDoc.open( docFilename, std::ios::out | std::ios::trunc );

  for ( const auto* include : includes )
  {
  }

  for ( const auto* prop : properties )
  {
    std::string strProp;
    strProp += "PROP_";
    if ( prop->Attribute( "package" ) && prop->Attribute( "package" )[0] != '\0' )
    {
      strProp += YaComponentCore::to_upper( prop->Attribute( "package" ) ) + "_";
    }
    strProp += YaComponentCore::to_upper( prop->Attribute( "id" ) );
  }
  for ( const auto* req : requests )
  {
    std::string strReq;
    strReq += "REQ_";
    if ( req->Attribute( "package" ) && req->Attribute( "package" )[0] != '\0' )
    {
      strReq += YaComponentCore::to_upper( req->Attribute( "package" ) ) + "_";
    }

    strReq += YaComponentCore::to_upper( req->Attribute( "id" ) );

    std::string strPara;
    auto* para = req->FirstChildElement( "para" );
    while ( para )
    {
      para = para->NextSiblingElement( "para" );
    }
  }

  for ( const auto* resp : responses )
  {
    std::string strResp;
    strResp += "RESP_";
    if ( resp->Attribute( "package" ) && resp->Attribute( "package" )[0] != '\0' )
    {
      strResp += YaComponentCore::to_upper( resp->Attribute( "package" ) ) + "_";
    }
    strResp += YaComponentCore::to_upper( resp->Attribute( "id" ) );

    std::string strPara;

    auto* para = resp->FirstChildElement( "para" );
    while ( para )
    {
      // strPara += "m" + std::string(para->Attribute("id"));
      para = para->NextSiblingElement( "para" );
    }
  }
  fhDoc.close();
}

void YaComponentDocGen::writeComponent( const std::filesystem::path& docPath,
                                        const std::string& compName,
                                        const EntryList& providedIfc,
                                        const EntryList& usedIfc )
{
  std::ofstream fhDoc;

  auto docFilename = ( docPath / ( compName + ".uml" ) ).string();

  YaComponentCore::printDbg( std::string( "YaComponentDocGen:: write component documentation file" ) + docFilename );

  fhDoc.open( docFilename, std::ios::out | std::ios::trunc );

  auto strCompDefine = std::string( "!definelong COMP_" ) + YaComponentCore::to_upper( compName ) + "(name";

  strCompDefine += ")\n";

  strCompDefine += "  [name] << " + compName + " >>\n";

  for ( const auto& ifc : providedIfc )
  {
    strCompDefine += "  interface " + ifc.at( "id" ) + " << " + ifc.at( "classname" ) + " >> as name." + ifc.at( "id" ) + "\n";
    strCompDefine += "  name." + ifc.at( "id" ) + " << " + ifc.at( "classname" ) + " - [name]\n";
  }

  strCompDefine += "!enddefinelong\n";

  fhDoc << strCompDefine << std::endl;

  fhDoc.close();
}
