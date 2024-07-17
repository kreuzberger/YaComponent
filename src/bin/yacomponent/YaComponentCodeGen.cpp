
#include "YaComponentCodeGen.h"

#include <core/YaComponentCore.h>
#include <fstream>
#include <iostream>

YaComponentCodeGen::YaComponentCodeGen() {}

void YaComponentCodeGen::writeComponent(const std::filesystem::path &codePath,
                                        const std::string &compName,
                                        const EntryList &providedIfc,
                                        const EntryList &usedIfc)
{
    std::ofstream fhSource;
    std::ofstream fhHeader;

    auto sourceFilename = (codePath / (compName + "Impl.cpp")).string();
    auto headerFilename = (codePath / (compName + "Impl.h")).string();

    YaComponentCore::printDbg(std::string("YaComponentCodeGen:: write component source code file ")
                              + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);
    fhHeader.open(headerFilename, std::ios::out | std::ios::trunc);

    fhSource << "#include \"" + compName + "Impl.h\"\n" << std::endl;
    fhSource << "namespace YaComponent {\n" << std::endl;

    fhHeader << "#pragma once" << std::endl;

    std::map<std::string, std::string> proxies;
    std::map<std::string, std::string> stubs;

    for (auto ifc : usedIfc) {
        proxies[ifc["classname"]] = ifc["id"];
    }

    for (auto ifc : providedIfc) {
        stubs[ifc["classname"]] = ifc["id"];
    }

    for (const auto &p : proxies) {
        fhHeader << "#include \"" << p.first << "Proxy.h\"" << std::endl;
    }

    for (const auto &s : stubs) {
        fhHeader << "#include \"" << s.first << "Stub.h\"" << std::endl;
    }

    fhHeader << "#include <QtCore/QObject>" << std::endl;
    fhHeader << "#include <QtCore/QTimer>" << std::endl;
    //fhHeader << "#include <stdio.h>" << std::endl;
    fhHeader << "#include <thread>" << std::endl;
    fhHeader << "#include <chrono>" << std::endl;

    fhHeader << std::endl;

    fhHeader << "namespace YaComponent {" << std::endl;

    fhHeader << "class " << compName << "Impl: public QObject";
    fhHeader << "{" << std::endl;
    fhHeader << "  Q_OBJECT" << std::endl;
    fhHeader << "  public:" << std::endl;
    fhHeader << "    " + compName + "Impl(";

    fhSource << compName << "Impl::" << compName << "Impl(";
    auto strCtor = std::string("void* context");

    for (const auto &p : proxies) {
        strCtor += ", I" + p.first + "ProxyHandler& r" + p.first;
    }

    for (const auto &s : stubs) {
        strCtor += ", I" + s.first + "StubHandler& r" + s.first;
    }

    fhHeader << strCtor << " );" << std::endl;
    fhSource << strCtor << " )" << std::endl;

    fhHeader << "    virtual ~" << compName << "Impl();" << std::endl;

    for (const auto &ifc : providedIfc) {
        fhHeader << "    void setConnectionPara" << ifc.at("id")
                 << "( const char* address, int hwm = 0 );" << std::endl;
    }

    for (const auto &ifc : usedIfc) {
        fhHeader << "    void setConnectionPara" << ifc.at("id")
                 << "( const char* address, const char* ident );" << std::endl;
    }

    fhHeader << "    /// called after setting connections and moveToThread" << std::endl;
    fhHeader << "    /// if overloaded base class implementation must be called!" << std::endl;
    fhHeader << "    virtual void init();" << std::endl;
    fhHeader << "    virtual void close();" << std::endl;

    fhHeader << "  public slots:" << std::endl;
    fhHeader << "    void onTimer();" << std::endl;
    //fhHeader << "    void killTimer();" << std::endl;

    fhHeader << "  signals:" << std::endl;
    fhHeader << "    void startTimer( int iTimeOutMs );" << std::endl;
    fhHeader << "    void stopTimer( );" << std::endl;

    fhHeader << "  protected:" << std::endl;

    fhHeader << "    enum PROXY_IDS {" << std::endl;
    fhHeader << "      PROXY_INVALID = -1" << std::endl;

    for (const auto &ifc : usedIfc) {
        fhHeader << "    , PROXY_" << YaComponentCore::to_upper(ifc.at("id")) << std::endl;
    }

    fhHeader << "     };\n" << std::endl;

    fhHeader << "    enum STUB_IDS {" << std::endl;
    fhHeader << "      STUB_INVALID = -1" << std::endl;
    for (const auto &ifc : providedIfc) {
        fhHeader << "    , STUB_" << YaComponentCore::to_upper(ifc.at("id")) << std::endl;
    }

    fhHeader << "     };\n" << std::endl;

    std::string strCtorImpl;

    strCtorImpl += " :QObject()";

    for (const auto &ifc : usedIfc) {
        fhHeader << "  " << ifc.at("classname") << "Proxy m" << ifc.at("id") << ";" << std::endl;
        strCtorImpl += " , m" + ifc.at("id") + "( context, PROXY_"
                       + YaComponentCore::to_upper(ifc.at("id")) + ", r" + ifc.at("classname") + " )";
    }

    for (const auto &ifc : providedIfc) {
        fhHeader << "  " << ifc.at("classname") << "Stub m" << ifc.at("id") << ";" << std::endl;
        strCtorImpl += " , m" + ifc.at("id") + "( context, STUB_"
                       + YaComponentCore::to_upper(ifc.at("id")) + ", r" + ifc.at("classname") + " )";
    }

    // todo strCtorImpl.pop_back();

    fhSource << strCtorImpl << std::endl;
    fhSource << "{" << std::endl;
    fhSource << "  mpoTimer = new QTimer( this );" << std::endl;
    fhSource << "}" << std::endl;

    fhSource << compName << "Impl::~" << compName << "Impl()" << std::endl;
    fhSource << "{" << std::endl;
    fhSource << "}" << std::endl;

    fhSource << "void " << compName << "Impl::init()" << std::endl;
    fhSource << "{" << std::endl;
    fhSource << "  connect( mpoTimer, SIGNAL(timeout()), this, SLOT(onTimer()));" << std::endl;
    fhSource << "  connect( this, SIGNAL(startTimer(int)), mpoTimer, SLOT(start(int)));" << std::endl;
    fhSource << "  connect( this, SIGNAL(stopTimer()), mpoTimer, SLOT(stop()));" << std::endl;
    fhSource << "  emit startTimer( YaComponent::TimeOut );" << std::endl;
    fhSource << "}" << std::endl;

    fhSource << "void " << compName << "Impl::close()" << std::endl;
    fhSource << "{" << std::endl;
    fhSource << "  emit stopTimer( );" << std::endl;
    fhSource << "  while(mpoTimer->isActive())" << std::endl;
    fhSource << "  {" << std::endl;
    fhSource << "    std::this_thread::sleep_for(std::chrono::milliseconds(50));" << std::endl;
    fhSource << "  }" << std::endl;

    for (const auto &ifc : providedIfc) {
        fhSource << "  m" << ifc.at("id") << ".close();" << std::endl;
    }

    for (const auto &ifc : usedIfc) {
        fhSource << "  m" << ifc.at("id") << ".close();" << std::endl;
    }

    fhSource << "}" << std::endl;

    // fhSource << "void " << compName << "Impl::killTimer()" << std::endl;
    // fhSource << "{" << std::endl;
    // fhSource << "  delete mpoTimer;" << std::endl;
    // fhSource << "  mpoTimer = nullptr;" << std::endl;
    // fhSource << "}" << std::endl;

    for (const auto &ifc : providedIfc) {
        fhSource << "void " << compName << "Impl::setConnectionPara" << ifc.at("id")
                 << "( const char* address, int hwm )" << std::endl;
        fhSource << "{" << std::endl;
        fhSource << "  m" << ifc.at("id") << ".setConnectionPara( address, hwm );" << std::endl;
        fhSource << "}" << std::endl;
    }

    for (const auto &ifc : usedIfc) {
        fhSource << "void " << compName << "Impl::setConnectionPara" << ifc.at("id")
                 << "( const char* address, const char* ident )" << std::endl;
        fhSource << "{" << std::endl;
        fhSource << "  m" << ifc.at("id") << ".setConnectionPara( address, ident );" << std::endl;
        fhSource << "}" << std::endl;
    }

    fhSource << "void " << compName << "Impl::onTimer()" << std::endl;
    fhSource << "{" << std::endl;
    for (const auto &ifc : usedIfc) {
        fhSource << "  m" << ifc.at("id") << ".receive();" << std::endl;
    }
    for (const auto &ifc : providedIfc) {
        fhSource << "  m" << ifc.at("id") << ".receive();" << std::endl;
    }

    fhSource << "}" << std::endl;

    fhHeader << "\n  private:" << std::endl;
    fhHeader << "    " << compName << "Impl( const " << compName << "Impl& );" << std::endl;
    fhHeader << "    " << compName << "Impl& operator= ( const " << compName << "Impl& );"
             << std::endl;
    fhHeader << "    QTimer* mpoTimer = nullptr;" << std::endl;

    fhHeader << "};" << std::endl;

    fhHeader << "}" << std::endl;

    fhHeader << "" << std::endl;

    fhSource << "}" << std::endl; // closes the namespace

    fhSource.close();
    fhHeader.close();
}

void YaComponentCodeGen::writeIfc(const std::filesystem::path &codePath,
                                  const std::string &ifcName,
                                  const ElementList &properties,
                                  const ElementList &requests,
                                  const ElementList &responses,
                                  const ElementList &includes)
{
    writeIfcProxy(codePath, ifcName, properties, requests, responses, includes);
    writeIfcStub(codePath, ifcName, properties, requests, responses, includes);
}
void YaComponentCodeGen::writeIfcProxy(const std::filesystem::path &codePath,
                                       const std::string &ifcName,
                                       const ElementList &properties,
                                       const ElementList &requests,
                                       const ElementList &responses,
                                       const ElementList &includes)
{
    std::ofstream fhSource;
    std::ofstream fhHeaderIfc;
    std::ofstream fhHeader;

    auto sourceFilename = (codePath / (ifcName + "Proxy.cpp")).string();
    auto headerFilename = (codePath / (ifcName + "Proxy.h")).string();
    auto interfaceFilename = (codePath / ("I" + ifcName + "ProxyHandler.h")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentCodeGen:: write interface proxy source code file ") + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);
    fhHeader.open(headerFilename, std::ios::out | std::ios::trunc);
    fhHeaderIfc.open(interfaceFilename, std::ios::out | std::ios::trunc);

    fhSource << "#include \"" << ifcName << "Proxy.h\"" << std::endl;
    fhSource << "namespace YaComponent {" << std::endl;
    fhSource << ifcName << "Proxy::" << ifcName << "Proxy( void* context, int id, I" << ifcName
             << "ProxyHandler& callbackHandler)" << std::endl;
    fhSource << ": YaProxyBase( context, id )" << std::endl;
    fhSource << ", mCallbackHandler( callbackHandler )" << std::endl;

    fhSource << "{\n}\n" << std::endl;

    std::string strDtor = "\n" + ifcName + "Proxy::~" + ifcName + "Proxy()\n{\n";

    fhHeader << "#pragma once" << std::endl;

    fhHeaderIfc << "#pragma once" << std::endl;

    for (const auto *include : includes) {
        auto filename = std::string(include->Attribute("file"));
        auto suffix = std::string(".h");
        if (filename.size() >= suffix.size()
            && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
            fhHeader << "#include \"" << include->Attribute("file") << "\"" << std::endl;
            fhHeaderIfc << "#include \"" << include->Attribute("file") << "\"" << std::endl;
        }
    }

    fhHeader << "#include \"I" << ifcName << "ProxyHandler.h\"" << std::endl;
    fhHeader << "#include <yacomponent/YaProxyBase.h>" << std::endl;
    fhHeader << "#include <yacomponent/YaSUBImpl.h>" << std::endl;
    fhHeader << "#include <QtCore/QObject>" << std::endl;

    fhHeader << "namespace YaComponent {" << std::endl;
    fhHeaderIfc << "namespace YaComponent {" << std::endl;

    fhHeader << "class " << ifcName << "Proxy: public YaProxyBase" << std::endl;
    fhHeader << "{" << std::endl;
    fhHeader << "  public:" << std::endl;
    fhHeader << "    " << ifcName << "Proxy( void* context, int id, I" << ifcName
             << "ProxyHandler& );" << std::endl;
    fhHeader << "    virtual ~" << ifcName << "Proxy();\n" << std::endl;
    fhHeader << "    virtual void receive();" << std::endl;

    fhHeaderIfc << "class I" << ifcName << "ProxyHandler" << std::endl;
    fhHeaderIfc << "{" << std::endl;
    fhHeaderIfc << "  public:" << std::endl;
    fhHeaderIfc << "    I" << ifcName << "ProxyHandler() {}" << std::endl;
    fhHeaderIfc << "    virtual ~I" << ifcName << "ProxyHandler() {}\n" << std::endl;

    fhHeader << "    enum KEYS {" << std::endl;
    for (const auto *prop : properties) {
        std::string strProp;
        strProp += "PROP_";
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            strProp += YaComponentCore::to_upper(prop->Attribute("package")) + "_";
        }
        strProp += YaComponentCore::to_upper(prop->Attribute("id"));
        fhHeader << "      " << strProp << "," << std::endl;
    }
    for (const auto *req : requests) {
        std::string strReq;
        strReq += "REQ_";
        if (req->Attribute("package") && req->Attribute("package")[0] != '\0') {
            strReq += YaComponentCore::to_upper(req->Attribute("package")) + "_";
        }

        strReq += YaComponentCore::to_upper(req->Attribute("id"));
        fhHeader << "      " << strReq << "," << std::endl;
    }
    for (const auto *resp : responses) {
        std::string strResp;
        strResp += "RESP_";
        if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
            strResp += YaComponentCore::to_upper(resp->Attribute("package")) + "_";
        }
        strResp += YaComponentCore::to_upper(resp->Attribute("id"));
        fhHeader << "      " << strResp << "," << std::endl;
    }
    fhHeader << "    };\n" << std::endl;

    for (const auto *prop : properties) {
        fhHeaderIfc << "    virtual void onProperty" << prop->Attribute("id")
                    << "( int proxyId, const ";
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            fhHeaderIfc << prop->Attribute("package") << "::";
        }
        fhHeaderIfc << prop->Attribute("id") << "& ) = 0;" << std::endl;
    }

    fhHeader << "" << std::endl;
    fhHeaderIfc << "" << std::endl;
    fhSource << strDtor << "\n}\n" << std::endl;

    for (const auto *req : requests) {
        fhHeader << "    int request" << req->Attribute("id");
        fhSource << "int " << ifcName << "Proxy::request" << req->Attribute("id");
        fhHeader << "( ";
        fhSource << "( ";
        std::string strPara;

        auto *para = req->FirstChildElement("para");
        while (para) {
            strPara += " const ";
            if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
                strPara += std::string(para->Attribute("package")) + "::";
            }
            strPara += std::string(para->Attribute("id")) + "&,";
            para = para->NextSiblingElement("para");
        }
        if (!strPara.empty()) {
            strPara.pop_back();
            fhHeader << strPara << ");" << std::endl;
            fhSource << strPara << " msg )\n{" << std::endl;
        } else {
            fhHeader << " );" << std::endl;
            fhSource << " )\n{" << std::endl;
        }
        fhSource << "  return mSubscriber.request( REQ_";
        if (req->Attribute("package") && req->Attribute("package")[0] != '\0') {
            fhSource << YaComponentCore::to_upper(req->Attribute("package")) + "_";
        }
        fhSource << YaComponentCore::to_upper(req->Attribute("id"));
        if (!strPara.empty()) {
            fhSource << ", msg";
        }
        fhSource << " );" << std::endl;
        fhSource << "}" << std::endl;
    }

    fhSource << "void " << ifcName << "Proxy::receive()" << std::endl;
    fhSource << "{" << std::endl;
    fhSource << "  bool bMsgAvailable = true;" << std::endl;

    fhSource << "  while( bMsgAvailable )" << std::endl;
    fhSource << "  {" << std::endl;
    fhSource << "    int key = -1;" << std::endl;
    fhSource << "    int size = -1;" << std::endl;
    fhSource << "    char* msgData = nullptr;" << std::endl;

    fhSource << "    int iBytes = mSubscriber.receive(key, size, &msgData );" << std::endl;
    fhSource << "    if( 0 <= key )" << std::endl;
    fhSource << "    {" << std::endl;
    //  fhSource << "    }" << std::endl;

    fhSource << "      switch( key )" << std::endl;
    fhSource << "      {" << std::endl;
    for (const auto *resp : responses) {
        std::string strResp;
        strResp += "RESP_";
        if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
            strResp += YaComponentCore::to_upper(resp->Attribute("package")) + "_";
        }
        strResp += YaComponentCore::to_upper(resp->Attribute("id"));
        fhSource << "      case " << strResp << ":" << std::endl;

        auto *para = resp->FirstChildElement("para");
        while (para) {
            fhSource << "        m" << para->Attribute("id") << ".ParseFromArray(msgData, size);"
                     << std::endl;
            para = para->NextSiblingElement("para");
        }

        std::string strPara;

        para = resp->FirstChildElement("para");
        while (para) {
            strPara += ", ";
            // if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
            //     strPara += std::string(para->Attribute("package")) + "::";
            // }
            strPara += "m" + std::string(para->Attribute("id"));
            para = para->NextSiblingElement("para");
        }

        // fhSource << "        mCallbackHandler.onResponse( mId, m" << resp->Attribute("id") << ");"
        //          << std::endl;
        fhSource << "        mCallbackHandler.onResponse" << resp->Attribute("id") << "( mId"
                 << strPara << ");" << std::endl;
        fhSource << "        break;" << std::endl;
    }

    for (const auto *prop : properties) {
        std::string strProp;
        strProp += "PROP_";
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            strProp += YaComponentCore::to_upper(prop->Attribute("package")) + "_";
        }
        strProp += YaComponentCore::to_upper(prop->Attribute("id"));
        fhSource << "      case " << strProp << ":" << std::endl;
        fhSource << "        if(msgData && 0 <= size)" << std::endl;
        fhSource << "        {" << std::endl;
        fhSource << "          m" << prop->Attribute("id") << ".ParseFromArray(msgData, size);"
                 << std::endl;
        fhSource << "          mCallbackHandler.onProperty" << prop->Attribute("id") << "( mId, m"
                 << prop->Attribute("id") << ");" << std::endl;
        fhSource << "        }" << std::endl;
        // fhSource << "      else {" << std::endl;
        // fhSource << "      }" << std::endl;

        fhSource << "        break;" << std::endl;
    }

    fhSource << "      default:" << std::endl;
    fhSource << "        if( -1 < key)" << std::endl;
    fhSource << "        {" << std::endl;
    fhSource << "          qFatal(\"" << ifcName << "Proxy::receive(): unknown key '%d'\",key);"
             << std::endl;
    fhSource << "        }" << std::endl;
    fhSource << "        break;" << std::endl;
    fhSource << "      }" << std::endl;
    fhSource << "    }" << std::endl;
    fhSource << "    else" << std::endl;
    fhSource << "    {" << std::endl;
    fhSource << "      bMsgAvailable = false;" << std::endl;
    fhSource << "    }" << std::endl;

    fhSource << "  }" << std::endl;

    fhSource << "}" << std::endl;

    fhHeader << "" << std::endl;

    //bool empty_response_handled = false;

    for (const auto *resp : responses) {
        std::string strPara;
        auto *para = resp->FirstChildElement("para");
        while (para) {
            strPara += ", const ";
            if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
                strPara += std::string(para->Attribute("package")) + "::";
            }
            strPara += std::string(para->Attribute("id")) + "&";
            para = para->NextSiblingElement("para");
        }
        // if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
        //     fhHeaderIfc << resp->Attribute("package") << "::";
        // }
        //fhHeaderIfc << resp->Attribute("id") << "& ) = 0;" << std::endl;
        fhHeaderIfc << "    virtual void onResponse" << resp->Attribute("id") << "( int proxyId";
        if (!strPara.empty()) {
            fhHeaderIfc << strPara << ") = 0;" << std::endl;
        } else { //if (!empty_response_handled) {
            fhHeaderIfc << ") = 0;" << std::endl;
        }

        // if (strPara.empty()) {
        //     empty_response_handled = true;
        // }
    }

    fhHeader << "private:" << std::endl;
    fhHeader << "  " << ifcName << "Proxy();" << std::endl;
    fhHeader << "  I" << ifcName << "ProxyHandler& mCallbackHandler;" << std::endl;

    for (const auto *prop : properties) {
        fhHeader << "  ";
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            fhHeader << prop->Attribute("package") << "::";
        }
        fhHeader << prop->Attribute("id");
        fhHeader << " m" << prop->Attribute("id") << ";" << std::endl;
    }
    for (const auto *resp : responses) {
        auto *para = resp->FirstChildElement("para");
        while (para) {
            fhHeader << "  ";
            if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
                fhHeader << std::string(para->Attribute("package")) + "::";
            }
            fhHeader << std::string(para->Attribute("id"));
            fhHeader << " m" << para->Attribute("id") << ";" << std::endl;

            para = para->NextSiblingElement("para");
        }
    }

    fhHeader << "};" << std::endl;
    fhHeaderIfc << "};" << std::endl;

    fhHeader << "}" << std::endl;
    fhHeaderIfc << "}" << std::endl;

    fhHeader << "" << std::endl;
    fhHeaderIfc << "" << std::endl;

    fhSource << "\n}" << std::endl;

    fhHeader.close();
    fhSource.close();
    fhHeaderIfc.close();
}
void YaComponentCodeGen::writeIfcStub(const std::filesystem::path &codePath,
                                      const std::string &ifcName,
                                      const ElementList &properties,
                                      const ElementList &requests,
                                      const ElementList &responses,
                                      const ElementList &includes)
{
    std::ofstream fhSource;
    std::ofstream fhHeaderIfc;
    std::ofstream fhHeader;

    auto sourceFilename = (codePath / (ifcName + "Stub.cpp")).string();
    auto headerFilename = (codePath / (ifcName + "Stub.h")).string();
    auto interfaceFilename = (codePath / ("I" + ifcName + "StubHandler.h")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentCodeGen:: write interface stub source code file ") + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);
    fhHeader.open(headerFilename, std::ios::out | std::ios::trunc);
    fhHeaderIfc.open(interfaceFilename, std::ios::out | std::ios::trunc);

    fhSource << "#include \"" << ifcName << "Stub.h\"\n";

    fhHeader << "#pragma once\n";
    fhHeaderIfc << "#pragma once\n";

    for (const auto *include : includes) {
        auto filename = std::string(include->Attribute("file"));
        auto suffix = std::string(".h");
        if (filename.size() >= suffix.size()
            && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
            fhHeader << "#include \"" << include->Attribute("file") << "\"" << std::endl;
            fhHeaderIfc << "#include \"" << include->Attribute("file") << "\"" << std::endl;
        }
    }

    fhHeader << "#include \"I" << ifcName << "StubHandler.h\"\n";
    fhHeader << "#include <yacomponent/YaStubBase.h>\n";
    fhHeader << "#include <yacomponent/YaPUBImpl.h>\n";
    fhHeader << "#include <google/protobuf/util/message_differencer.h>\n";

    //fhHeader << "#include <stdio.h>\n";
    fhHeader << std::endl;

    fhHeader << "namespace YaComponent {\n";
    fhHeaderIfc << "namespace YaComponent {\n";
    fhSource << "namespace YaComponent {\n";

    fhSource << ifcName + "Stub::" << ifcName << "Stub( void* context, int id, I" << ifcName
             << "StubHandler& rCallbackHandler )\n";

    fhHeaderIfc << "class I" << ifcName << "StubHandler\n";
    fhHeaderIfc << "{\n";

    fhHeader << "class " << ifcName << "Stub: public YaStubBase\n";
    fhHeader << "{\n";
    fhHeader << "  Q_OBJECT\n";
    fhHeader << "  public:\n";
    fhHeaderIfc << "  public:\n";
    fhHeader << "    " << ifcName << "Stub( void* context, int id, I" << ifcName
             << "StubHandler& );\n";
    fhHeader << "    virtual ~" << ifcName << "Stub();\n";

    fhHeaderIfc << "    I" << ifcName << "StubHandler() {}\n";
    fhHeaderIfc << "    virtual ~I" << ifcName << "StubHandler() {}\n";

    fhHeader << "    enum KEYS {\n";

    for (const auto *prop : properties) {
        std::string strProp;
        strProp += "PROP_";
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            strProp += YaComponentCore::to_upper(prop->Attribute("package")) + "_";
        }
        strProp += YaComponentCore::to_upper(prop->Attribute("id"));
        fhHeader << "      " << strProp << ",\n";
    }

    for (const auto *req : requests) {
        std::string strReq;
        strReq += "REQ_";
        if (req->Attribute("package") && req->Attribute("package")[0] != '\0') {
            strReq += YaComponentCore::to_upper(req->Attribute("package")) + "_";
        }
        strReq += YaComponentCore::to_upper(req->Attribute("id"));
        fhHeader << "      " << strReq << ",\n";
    }

    for (const auto *resp : responses) {
        std::string strResp;
        strResp += "RESP_";
        if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
            strResp += YaComponentCore::to_upper(resp->Attribute("package")) + "_";
        }
        strResp += YaComponentCore::to_upper(resp->Attribute("id"));
        fhHeader << "      " << strResp << ",\n";
    }
    fhHeader << "    };\n\n";

    for (const auto *prop : properties) {
        std::string strMethod;
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            strMethod += std::string(prop->Attribute("package")) + "::";
        }
        strMethod += prop->Attribute("id");
        fhHeader << "    int send(int key, const " << strMethod << "&);\n";
    }

    for (const auto *req : requests) {
        fhHeaderIfc << "    virtual void onRequest" << req->Attribute("id");
        fhHeaderIfc << "( int id ";
        std::string strPara;

        auto *para = req->FirstChildElement("para");
        while (para)
        {
            strPara += ", const ";
            if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
                strPara += std::string(para->Attribute("package")) + "::";
            }
            strPara += std::string(para->Attribute("id")) + "&";
            para = para->NextSiblingElement("para");
        }

        auto *resp = req->FirstChildElement("resp");
        while (resp) {
            auto resp_definition = findResponse(responses, resp->Attribute("id"));

            auto *respPara = resp_definition->FirstChildElement("para");
            while (respPara) {
                strPara += ", ";
                if (resp_definition->Attribute("package")
                    && resp_definition->Attribute("package")[0] != '\0') {
                    strPara += std::string(resp->Attribute("package")) + "::";
                }
                strPara += std::string(resp_definition->Attribute("id")) + "&";
                respPara = respPara->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }
        if (!strPara.empty()) {
            fhHeaderIfc << strPara << " ) = 0;\n";
        } else {
            fhHeaderIfc << " ) = 0;\n";
        }
    }

    fhHeader << "\n";

    for (const auto *resp : responses) {
        fhHeader << "    int response" << resp->Attribute("id")
                 << "( int key, const std::string& ident";
        // if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
        //     fhHeader << resp->Attribute("package") << "::";
        // }
        auto *respPara = resp->FirstChildElement("para");
        std::string strPara;
        while (respPara) {
            strPara += ", const ";
            if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
                strPara += std::string(resp->Attribute("package")) + "::";
            }
            strPara += std::string(resp->Attribute("id")) + "&";
            fhHeader << strPara;
            respPara = respPara->NextSiblingElement("para");
        }

        fhHeader << ");\n";
    }

    fhHeader << "    void receive();\n";

    fhHeader << "  protected:\n";

    fhSource << " : YaStubBase( context, id )\n";
    fhSource << " , mCallbackHandler( rCallbackHandler )\n";

    fhSource << "{\n\n}\n\n";

    for (const auto *resp : responses) {
        fhSource << "int " << ifcName << "Stub::response" << resp->Attribute("id")
                 << "( int key, const std::string& ident";
        auto *respPara = resp->FirstChildElement("para");
        std::string strPara;
        while (respPara) {
            strPara += ", const ";
            if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
                strPara += std::string(resp->Attribute("package")) + "::";
            }
            strPara += std::string(resp->Attribute("id")) + "& msg";
            fhSource << strPara;
            respPara = respPara->NextSiblingElement("para");
        }
        //fhSource << resp->Attribute("id") << "& msg )\n";
        fhSource << ")\n";
        fhSource << "{\n";
        if (!strPara.empty()) {
            fhSource << "  return mPublisher.response(key, ident, msg);\n";
        } else {
            fhSource << "  return mPublisher.response(key, ident);\n";
        }
        fhSource << "}\n\n";
    }

    fhSource << ifcName << "Stub::~" << ifcName << "Stub()\n";
    fhSource << "{\n\n}\n\n";

    for (const auto *prop : properties) {
        std::string strMethod;
        if (prop->Attribute("package") && prop->Attribute("package")[0] != '\0') {
            strMethod += std::string(prop->Attribute("package")) + "::";
        }
        strMethod += prop->Attribute("id");
        fhSource << "int " << ifcName << "Stub::send(int key, const " << strMethod
                 << "& rMessage )\n{\n";

        bool onChange = std::string(prop->Attribute("onChange")) == "0" ? false : true;

        if (onChange) {
            fhSource << "  auto rc = 0;\n";
            fhSource << "  if ( !google::protobuf::util::MessageDifferencer::Equals(m"
                     << prop->Attribute("id") << " , rMessage) ) {\n";
            fhSource << "    rc = mPublisher.send(key, rMessage );\n";
            fhSource << "    m" << prop->Attribute("id") << " = rMessage;\n";
            fhSource << "  }\n";
            fhSource << "  return rc;\n";
        } else {
            fhSource << "  return mPublisher.send(key, rMessage );\n";
        }

        fhSource << "}\n";

        fhHeader << "    " << strMethod << " m" << prop->Attribute("id") << ";\n";
    }

    fhSource << "void " << ifcName << "Stub::receive()\n";
    fhSource << "{\n";
    fhSource << "  bool bMsgAvailable = true;\n";

    fhSource << "  while( bMsgAvailable )\n";
    fhSource << "  {\n";
    fhSource << "    std::string ident;\n";
    fhSource << "    int key = -1;\n";
    fhSource << "    int size = -1;\n";
    fhSource << "    char* msgData = 0;\n";
    fhSource << "    ident.clear();\n";

    fhSource << "    int iBytes = mPublisher.receive(key, size, &msgData, ident );\n";
    fhSource << "    if( 0 <= key )\n";
    fhSource << "    {\n";

    fhSource << "      switch( key )\n";
    fhSource << "      {\n";

    for (const auto *req : requests) {
        std::string strReq;
        strReq += "REQ_";
        if (req->Attribute("package") && req->Attribute("package")[0] != '\0') {
            strReq += YaComponentCore::to_upper(req->Attribute("package")) + "_";
        }
        strReq += YaComponentCore::to_upper(req->Attribute("id"));
        fhSource << "      case " << strReq << ":\n";
        std::string strMember;

        auto *para = req->FirstChildElement("para");
        while (para) {
            strMember += std::string("        m") + req->Attribute("id") + "_"
                         + para->Attribute("id") + ".ParseFromArray(msgData, size);\n";
            para = para->NextSiblingElement("para");
        }

        if (!strMember.empty()) {
            fhSource << strMember;
        }

        auto *resp = req->FirstChildElement("resp");
        while (resp) {
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            para = resp_definition->FirstChildElement("para");
            while (para) {
                fhSource << "        m" << req->Attribute("id") << "_" << para->Attribute("id")
                         << ".Clear();\n";
                para = para->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }

        fhSource << "        mCallbackHandler.onRequest" << req->Attribute("id") << "( mId";
        std::string strPara;

        para = req->FirstChildElement("para");
        while (para)
        {
            strPara += std::string(", m") + req->Attribute("id") + "_" + para->Attribute("id");
            para = para->NextSiblingElement("para");
        }

        resp = req->FirstChildElement("resp");
        while (resp) {
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            para = resp_definition->FirstChildElement("para");
            while (para) {
                strPara += std::string(", m") + req->Attribute("id") + "_" + para->Attribute("id");
                para = para->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }
        if (!strPara.empty()) {
            fhSource << strPara << " );\n";
        } else {
            fhSource << " );\n";
        }

        resp = req->FirstChildElement("resp");
        while (resp) {
            std::string strResp;
            strResp += "RESP_";
            if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
                strResp += YaComponentCore::to_upper(resp->Attribute("package")) + "_";
            }
            strResp += YaComponentCore::to_upper(resp->Attribute("id"));
            fhSource << "        response" << resp->Attribute("id") << "( " << strResp << ", ident";
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            para = resp_definition->FirstChildElement("para");
            while (para) {
                fhSource << ", m" << req->Attribute("id") << "_" << para->Attribute("id");
                para = para->NextSiblingElement("para");
            }
            fhSource << " );\n";
            resp = resp->NextSiblingElement("resp");
        }

        fhSource << "        break;\n";
    }

    fhSource << "      default:\n";
    fhSource << "        if( -1 < key)\n";
    fhSource << "        {\n";
    fhSource << "          qFatal(\"" << ifcName << "Stub::receive() unknown key '%d'\",key);"
             << std::endl;
    fhSource << "        }\n";
    fhSource << "        break;\n";
    fhSource << "      }\n";
    fhSource << "    }\n";
    fhSource << "    else\n";
    fhSource << "    {\n";
    fhSource << "      bMsgAvailable = false;";
    fhSource << "    }\n";

    fhSource << "  }\n";

    fhSource << "}\n";

    fhHeader << "    I" << ifcName << "StubHandler& mCallbackHandler;\n";

    for (const auto *req : requests) {
        std::string strPara;

        auto *para = req->FirstChildElement("para");
        while (para) {
            strPara += "    ";
            if (para->Attribute("package") && para->Attribute("package")[0] != '\0') {
                strPara += std::string(para->Attribute("package")) + "::";
            }
            strPara += std::string(para->Attribute("id")) + " m" + req->Attribute("id") + "_"
                       + para->Attribute("id") + ";\n";
            para = para->NextSiblingElement("para");
        }

        auto *resp = req->FirstChildElement("resp");
        while (resp) {
            // strPara += "    ";
            // if (resp->Attribute("package") && resp->Attribute("package")[0] != '\0') {
            //     strPara += std::string(resp->Attribute("package")) + "::";
            // }
            // strPara += std::string(resp->Attribute("id")) + " m" + req->Attribute("id") + "_"
            //            + resp->Attribute("id") + ";\n";
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            auto *respPara = resp_definition->FirstChildElement("para");
            while (respPara) {
                strPara += "    ";
                if (respPara->Attribute("package") && respPara->Attribute("package")[0] != '\0') {
                    strPara += std::string(respPara->Attribute("package")) + "::";
                }
                strPara += std::string(respPara->Attribute("id")) + " m" + req->Attribute("id")
                           + "_" + respPara->Attribute("id") + ";\n";
                respPara = respPara->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }

        if (!strPara.empty()) {
            fhHeader << strPara << std::endl;
        }
    }

    fhHeader << "\n};\n";
    fhHeaderIfc << "\n};\n";

    fhHeader << "\n}\n";
    fhHeaderIfc << "\n}\n";
    fhSource << "}\n";

    fhHeader << "\n";
    fhHeaderIfc << "\n";

    fhHeader.close();
    fhSource.close();
    fhHeaderIfc.close();
}

const tinyxml2::XMLElement *YaComponentCodeGen::findResponse(const ElementList &responses,
                                                             const std::string &id)
{
    const tinyxml2::XMLElement *ret = nullptr;
    for (const auto *resp : responses) {
        if (resp->Attribute("id") == id) {
            ret = resp;
        }
    }
    return ret;
}
