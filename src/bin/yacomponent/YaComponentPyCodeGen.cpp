
#include "YaComponentPyCodeGen.h"

#include <core/YaComponentCore.h>
#include <fstream>
#include <iostream>

YaComponentPyCodeGen::YaComponentPyCodeGen() {}

void YaComponentPyCodeGen::writeComponent(const std::filesystem::path &codePath,
                                          const std::string &compName,
                                          const EntryList &providedIfc,
                                          const EntryList &usedIfc)
{
    std::ofstream fhSource;

    auto sourceFilename = (codePath / (compName + "Impl.py")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentPyCodeGen:: write component source code file ") + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);

    std::map<std::string, std::string> proxies;
    std::map<std::string, std::string> stubs;

    for (auto ifc : usedIfc) {
        proxies[ifc["classname"]] = ifc["id"];
    }

    for (auto ifc : providedIfc) {
        stubs[ifc["classname"]] = ifc["id"];
    }

    for (const auto &p : proxies) {
        fhSource << "from " << p.first << "Proxy import " << p.first << "Proxy" << std::endl;
    }

    for (const auto &s : stubs) {
        fhSource << "from " << s.first << "Stub import " << s.first << "Stub" << std::endl;
    }

    fhSource << "from yacomponent import variables as yc" << std::endl;
    fhSource << "import zmq" << std::endl;
    fhSource << "from PySide2.QtCore import QObject" << std::endl;
    fhSource << "from PySide2.QtCore import QTimer, Signal, Slot" << std::endl;
    fhSource << "from enum import IntEnum" << std::endl;
    fhSource << "import logging" << std::endl;
    fhSource << "from time import sleep" << std::endl;
    fhSource << std::endl;

    fhSource << "class " << compName << "Impl(QObject):";
    fhSource << std::endl;
    fhSource << std::endl;

    auto keyIdx = 0;

    fhSource << "    class PROXY_IDS(IntEnum):" << std::endl;
    fhSource << "        INVALID = " << keyIdx++ << std::endl;

    for (const auto &ifc : usedIfc) {
        fhSource << "        " << YaComponentCore::to_upper(ifc.at("id")) << " = " << keyIdx++
                 << std::endl;
    }

    fhSource << std::endl;

    keyIdx = 0;

    fhSource << "    class STUB_IDS(IntEnum):" << std::endl;
    fhSource << "        INVALID = " << keyIdx++ << std::endl;
    for (const auto &ifc : providedIfc) {
        fhSource << "        " << YaComponentCore::to_upper(ifc.at("id")) << " = " << keyIdx++
                 << std::endl;
    }

    fhSource << std::endl;

    auto strCtor = std::string("context: zmq.Context");

    for (const auto &p : proxies) {
        strCtor += ", cb_" + p.first;
    }

    for (const auto &s : stubs) {
        strCtor += ", cb_" + s.first;
    }

    for (const auto &ifc : usedIfc) {
        fhSource << "    _" << ifc.at("id") << " = None" << std::endl;
    }

    for (const auto &ifc : providedIfc) {
        fhSource << "    _" << ifc.at("id") << " = None" << std::endl;
    }

    fhSource << "    _Timer = None" << std::endl;
    fhSource << "    _startTimer = Signal(int)" << std::endl;
    fhSource << "    _stopTimer = Signal()" << std::endl;

    fhSource << std::endl << std::endl;

    fhSource << "    def __init__(self, " << strCtor << "):" << std::endl;
    fhSource << "        super().__init__()" << std::endl;

    for (const auto &ifc : usedIfc) {
        fhSource << "        self._" + ifc.at("id") + " = " + ifc.at("classname")
                        + "Proxy( context, self.PROXY_IDS." + YaComponentCore::to_upper(ifc.at("id"))
                        + ", cb_" + ifc.at("classname") + " )"
                 << std::endl;
    }

    for (const auto &ifc : providedIfc) {
        fhSource << "        self._" + ifc.at("id") + " = " + ifc.at("classname")
                        + "Stub( context, self.STUB_IDS." + YaComponentCore::to_upper(ifc.at("id"))
                        + ", cb_" + ifc.at("classname") + " )"
                 << std::endl;
    }

    fhSource << "        self._Timer = QTimer( self )" << std::endl;

    fhSource << std::endl;
    fhSource << std::endl;

    fhSource << "    def init(self):" << std::endl;
    fhSource << "        self._Timer.timeout.connect(self._onTimer)" << std::endl;
    fhSource << "        self._startTimer.connect(self._Timer.start)" << std::endl;
    fhSource << "        self._stopTimer.connect(self._Timer.stop)" << std::endl;
    fhSource << "        self._startTimer.emit( yc.TimeOutMs )" << std::endl;
    fhSource << std::endl << std::endl;

    fhSource << "    def close(self):" << std::endl;
    fhSource << "        self._stopTimer.emit()" << std::endl;
    fhSource << std::endl;
    fhSource << "        sleep(yc.TimeOut)" << std::endl;
    fhSource << std::endl;

    for (const auto &ifc : providedIfc) {
        fhSource << "        self._" << ifc.at("id") << ".close()" << std::endl;
    }

    for (const auto &ifc : usedIfc) {
        fhSource << "        self._" << ifc.at("id") << ".close()" << std::endl;
    }

    fhSource << std::endl << std::endl;


    fhSource << std::endl << std::endl;

    for (const auto &ifc : providedIfc) {
        fhSource << "    def setConnectionPara" << ifc.at("id")
                 << "(self, address: str, hwm:int ):" << std::endl;
        fhSource << "        self._" << ifc.at("id") << ".setConnectionPara( address, hwm )"
                 << std::endl;
        fhSource << std::endl << std::endl;
    }

    for (const auto &ifc : usedIfc) {
        fhSource << "    def setConnectionPara" << ifc.at("id")
                 << "( self, address: str, ident: str ):" << std::endl;
        fhSource << "        self._" << ifc.at("id") << ".setConnectionPara( address, ident )"
                 << std::endl;
        fhSource << std::endl << std::endl;
    }

    fhSource << "    def _onTimer(self):" << std::endl;
    fhSource << "        # logging.info(f\"PublisherCompImpl::_onTimer\")" << std::endl;
    for (const auto &ifc : usedIfc) {
        fhSource << "        self._" << ifc.at("id") << ".receive()" << std::endl;
    }
    for (const auto &ifc : providedIfc) {
        fhSource << "        self._" << ifc.at("id") << ".receive()" << std::endl;
    }

    fhSource << std::endl;
    fhSource.close();
}

void YaComponentPyCodeGen::writeIfc(const std::filesystem::path &codePath,
                                    const std::string &ifcName,
                                    const ElementList &properties,
                                    const ElementList &requests,
                                    const ElementList &responses,
                                    const ElementList &includes)
{
    writeIfcProxy(codePath, ifcName, properties, requests, responses, includes);
    writeIfcStub(codePath, ifcName, properties, requests, responses, includes);
}
void YaComponentPyCodeGen::writeIfcProxy(const std::filesystem::path &codePath,
                                         const std::string &ifcName,
                                         const ElementList &properties,
                                         const ElementList &requests,
                                         const ElementList &responses,
                                         const ElementList &includes)
{
    std::ofstream fhSource;
    std::ofstream fhHeaderIfc;

    auto sourceFilename = (codePath / (ifcName + "Proxy.py")).string();
    auto ifcFilename = (codePath / (ifcName + "ProxyHandler.py")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentPyCodeGen:: write interface proxy source code file ") + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);
    fhHeaderIfc.open(ifcFilename, std::ios::out | std::ios::trunc);

    fhSource << "# import " << ifcName << "ProxyHandler" << std::endl;
    fhSource << "import zmq" << std::endl;
    fhSource << "from yacomponent.proxy import *" << std::endl;
    fhSource << "from yacomponent.stub import *" << std::endl;

    fhSource << "from PySide2.QtCore import QObject" << std::endl;
    fhSource << "from enum import IntEnum, auto" << std::endl;
    fhSource << "import logging" << std::endl;

    for (const auto *include : includes) {
        auto filename = std::string(include->Attribute("file"));
        auto suffix = std::string(".py");
        if (filename.size() >= suffix.size()
            && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
            fhSource << "from " << filename.erase(filename.find(suffix)) << " import *" << std::endl;
            fhHeaderIfc << "from " << filename << " import *" << std::endl;
        }
    }

    fhSource << std::endl;
    fhSource << std::endl;

    fhSource << "class " << ifcName << "Proxy( Proxy ):" << std::endl << std::endl;
    fhSource << "    _callback = None" << std::endl;

    for (const auto *prop : properties) {
        fhSource << "    ";
        fhSource << "_" << prop->Attribute("id") << " = " << prop->Attribute("id") << "()"
                 << std::endl;
    }
    for (const auto *resp : responses) {
        fhSource << "    ";
        auto *para = resp->FirstChildElement("para");
        while (para) {
            fhSource << "_" << para->Attribute("id") << " = " << para->Attribute("id") << "()"
                     << std::endl;
            para = para->NextSiblingElement("para");
        }
    }

    fhSource << std::endl;

    auto keyIdx = 0;
    if (!(properties.empty() && requests.empty() && responses.empty())) {
        fhSource << "    class KEYS(IntEnum):" << std::endl;

        for (const auto *prop : properties) {
            std::string strProp;
            strProp += "PROP_";
            strProp += YaComponentCore::to_upper(prop->Attribute("id"));
            fhSource << "        " << strProp << " = " << keyIdx++ << std::endl;
        }
        for (const auto *req : requests) {
            std::string strReq;
            strReq += "REQ_";
            strReq += YaComponentCore::to_upper(req->Attribute("id"));
            fhSource << "        " << strReq << " = " << keyIdx++ << std::endl;
        }
        for (const auto *resp : responses) {
            std::string strResp;
            strResp += "RESP_";
            strResp += YaComponentCore::to_upper(resp->Attribute("id"));
            fhSource << "        " << strResp << " = " << keyIdx++ << std::endl;
        }

        fhSource << std::endl << std::endl;
    }
    fhSource << "    def __init__(self, context: zmq.Context, id: int, callback ): # todo callback "
                "handler"
             << std::endl;

    fhSource << "        super().__init__(context,id)" << std::endl;
    fhSource << "        self._callback = callback" << std::endl;

    fhSource << std::endl << std::endl;

    if (!(properties.empty() && responses.empty())) {
        fhHeaderIfc << "class " << ifcName << "ProxyHandler:" << std::endl;
        fhHeaderIfc << std::endl;

        for (const auto *prop : properties) {
            fhHeaderIfc << "    def onProperty" << prop->Attribute("id")
                        << "(self, id: int, property: ";
            fhHeaderIfc << prop->Attribute("id") << " ):" << std::endl;
            fhHeaderIfc << "        pass" << std::endl;
            fhHeaderIfc << std::endl << std::endl;
        }
    }

    for (const auto *req : requests) {
        //     fhHeader << "    int request" << req->Attribute("id");
        std::string strPara;
        auto *para = req->FirstChildElement("para");
        auto has_para = para != nullptr;
        int paraIdx = 1;
        while (para) {
            strPara += std::string(", msg") /*+ std::to_string(paraIdx)*/ + std::string(": ");
            strPara += std::string(para->Attribute("id"));
            para = para->NextSiblingElement("para");
            paraIdx++;
        }
        strPara += " ) -> int:";
        fhSource << "    def request" << req->Attribute("id") << "(self" << strPara << std::endl;

        fhSource << "        return self._Subscriber.request( self.KEYS.REQ_";
        fhSource << YaComponentCore::to_upper(req->Attribute("id"));
        if (has_para) {
            fhSource << ", msg";
        } else {
            fhSource << ", None";
        }
        fhSource << " )" << std::endl;
        fhSource << std::endl << std::endl;
    }

    fhSource << "    def receive(self) -> None:" << std::endl;
    fhSource << "        msgAvailable = True" << std::endl;

    fhSource << "        while msgAvailable:" << std::endl;
    fhSource << "            key = -1" << std::endl;
    fhSource << "            msgData = None" << std::endl;

    fhSource << "            (key, msgData) = self._Subscriber.receive( )" << std::endl;
    fhSource << "            if key is not None:" << std::endl;
    fhSource << "                match (key):" << std::endl;
    for (const auto *resp : responses) {
        std::string strResp;
        strResp += "RESP_";
        strResp += YaComponentCore::to_upper(resp->Attribute("id"));
        fhSource << "                    case self.KEYS." << strResp << ":" << std::endl;

        std::string strPara;

        auto *para = resp->FirstChildElement("para");
        while (para) {
            fhSource << "                        if msgData is not None:" << std::endl;
            fhSource << "                            self._" << para->Attribute("id")
                     << ".ParseFromString(msgData)" << std::endl;

            strPara += ", ";
            strPara += "self._" + std::string(para->Attribute("id"));
            para = para->NextSiblingElement("para");
        }

        if (!strPara.empty()) {
            fhSource << "                            self._callback.onResponse"
                     << resp->Attribute("id") << "(self._id" << strPara << ")" << std::endl;
        } else {
            fhSource << "                        self._callback.onResponse" << resp->Attribute("id")
                     << "(self._id )" << std::endl;
        }
    }

    for (const auto *prop : properties) {
        std::string strProp;
        strProp += "PROP_";
        strProp += YaComponentCore::to_upper(prop->Attribute("id"));
        fhSource << "                    case self.KEYS." << strProp << ":" << std::endl;
        fhSource << "                        if msgData is not None:" << std::endl;
        fhSource << "                            self._" << prop->Attribute("id")
                 << ".ParseFromString(msgData)" << std::endl;
        fhSource << "                            self._callback.onProperty" << prop->Attribute("id")
                 << "( self._id, self._" << prop->Attribute("id") << ")" << std::endl;
    }

    fhSource << "                    case _:" << std::endl;
    fhSource << "                        if -1 < key:" << std::endl;
    fhSource << "                            raise RuntimeError(f\"" << ifcName
             << "Proxy::receive(): unknown key '{key}'\")" << std::endl;

    fhSource << "            else:" << std::endl;
    fhSource << "                msgAvailable = False" << std::endl;

    fhSource << std::endl << std::endl;

    for (const auto *resp : responses) {
        std::string strPara;
        auto *para = resp->FirstChildElement("para");
        while (para) {
            strPara += ", msg: ";
            strPara += std::string(para->Attribute("id"));
            para = para->NextSiblingElement("para");
        }
        fhHeaderIfc << "    def onResponse" << resp->Attribute("id") << "( self, proxyId: int";
        fhHeaderIfc << strPara << "):" << std::endl;
        fhHeaderIfc << "        pass" << std::endl;
        fhHeaderIfc << std::endl << std::endl;
    }

    fhSource.close();
    fhHeaderIfc.close();
}
void YaComponentPyCodeGen::writeIfcStub(const std::filesystem::path &codePath,
                                        const std::string &ifcName,
                                        const ElementList &properties,
                                        const ElementList &requests,
                                        const ElementList &responses,
                                        const ElementList &includes)
{
    std::ofstream fhSource;
    std::ofstream fhHeaderIfc;

    auto sourceFilename = (codePath / (ifcName + "Stub.py")).string();
    auto interfaceFilename = (codePath / (ifcName + "StubHandler.py")).string();

    YaComponentCore::printDbg(
        std::string("YaComponentPyCodeGen:: write interface stub source code file ") + sourceFilename);
    fhSource.open(sourceFilename, std::ios::out | std::ios::trunc);
    fhHeaderIfc.open(interfaceFilename, std::ios::out | std::ios::trunc);

    for (const auto *include : includes) {
        auto filename = std::string(include->Attribute("file"));
        auto suffix = std::string(".py");
        if (filename.size() >= suffix.size()
            && filename.compare(filename.size() - suffix.size(), suffix.size(), suffix) == 0) {
            fhSource << "from " << filename.erase(filename.find(suffix)) << " import *" << std::endl;
            fhHeaderIfc << "from " << filename << " import *" << std::endl;
            //fhHeaderIfc << "#include \"" << include->Attribute("file") << "\"" << std::endl;
        }
    }

    fhSource << "# import" << ifcName << "StubHandler\n";

    fhSource << "import zmq" << std::endl;
    fhSource << "from yacomponent.stub import *" << std::endl;

    fhSource << "# todo python equivalent to <google/protobuf/util/message_differencer.h>\n";

    fhSource << "from PySide2.QtCore import QObject" << std::endl;
    fhSource << "from enum import IntEnum, auto" << std::endl;
    fhSource << "import logging" << std::endl;

    fhSource << std::endl;
    fhSource << std::endl;

    fhSource << "class " << ifcName << "Stub( Stub ):" << std::endl << std::endl;
    fhSource << "    _callback = None" << std::endl;

    for (const auto *prop : properties) {
        fhSource << "    ";
        fhSource << "_" << prop->Attribute("id") << " = None" << std::endl;
    }

    for (const auto *req : requests) {
        auto *para = req->FirstChildElement("para");
        while (para) {
            fhSource << "    "
                     << "_" << req->Attribute("id") << "_" << para->Attribute("id") << " = "
                     << para->Attribute("id") << "()" << std::endl;
            para = para->NextSiblingElement("para");
        }
        auto *resp = req->FirstChildElement("resp");
        while (resp) {
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            auto *resp_para = resp_definition->FirstChildElement("para");
            while (resp_para) {
                fhSource << "    "
                         << "_" << req->Attribute("id") << "_" << resp_para->Attribute("id")
                         << " = " << resp_para->Attribute("id") << "()" << std::endl;
                resp_para = resp_para->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }
    }

    fhSource << std::endl;

    auto keyIdx = 0;

    if (!(properties.empty() && requests.empty() && responses.empty())) {
        fhSource << "    class KEYS(IntEnum):" << std::endl;

        for (const auto *prop : properties) {
            std::string strProp;
            strProp += "PROP_";
            strProp += YaComponentCore::to_upper(prop->Attribute("id"));
            fhSource << "        " << strProp << " = " << keyIdx++ << std::endl;
        }
        for (const auto *req : requests) {
            std::string strReq;
            strReq += "REQ_";

            strReq += YaComponentCore::to_upper(req->Attribute("id"));
            fhSource << "        " << strReq << " = " << keyIdx++ << std::endl;
        }
        for (const auto *resp : responses) {
            std::string strResp;
            strResp += "RESP_";
            strResp += YaComponentCore::to_upper(resp->Attribute("id"));
            fhSource << "        " << strResp << " = " << keyIdx++ << std::endl;
        }

        fhSource << std::endl << std::endl;
    }

    if (!(properties.empty() && requests.empty() && responses.empty())) {
        fhHeaderIfc << "class " << ifcName << "StubHandler:" << std::endl;
        fhHeaderIfc << std::endl;

        for (const auto *req : requests) {
            fhHeaderIfc << "    def onRequest" << req->Attribute("id");
            fhHeaderIfc << "(self, id: int ";
            std::string strPara;

            auto *para = req->FirstChildElement("para");
            while (para) {
                strPara += ", ";
                strPara += std::string(para->Attribute("id"));
                para = para->NextSiblingElement("para");
            }

            auto *resp = req->FirstChildElement("resp");
            while (resp) {
                strPara += ", ";
                strPara += std::string(resp->Attribute("id"));
                resp = resp->NextSiblingElement("resp");
            }
            if (!strPara.empty()) {
                fhHeaderIfc << strPara << " ):\n";
            } else {
                fhHeaderIfc << " ):\n";
            }
            fhHeaderIfc << "        pass" << std::endl;
            fhHeaderIfc << std::endl << std::endl;
        }
    }

    fhSource << "    def __init__(self, context: zmq.Context, id: int, callback ): # todo callback "
                "handler"
             << std::endl;

    fhSource << "        super().__init__(context,id)" << std::endl;
    fhSource << "        self._callback = callback" << std::endl;

    fhSource << std::endl;
    fhSource << std::endl;

    for (const auto *resp : responses) {
        fhSource << "    def response" << resp->Attribute("id") << "( self, key: int, ident: str";
        auto *para = resp->FirstChildElement("para");
        auto strParaMsg = (para) ? std::string(", msg") : std::string(", None");
        while (para) {
            fhSource << ", msg: " << para->Attribute("id");
            para = para->NextSiblingElement("para");
        }
        fhSource << ") -> int:\n";

        fhSource << "        return self._Publisher.response(key, ident" << strParaMsg << ")\n";
        fhSource << std::endl;
        fhSource << std::endl;
    }

    for (const auto *prop : properties) {
        std::string strMethod;
        strMethod += prop->Attribute("id");
        fhSource << "    def send" << prop->Attribute("id") << "(self, key: int, msg: " << strMethod
                 << ") -> int:\n";

        bool onChange = std::string(prop->Attribute("onChange")) == "0" ? false : true;

        if (onChange) {
            fhSource << "        rc = 0\n";
            fhSource << "        if not self._" << prop->Attribute("id") << " == msg:\n";
            fhSource << "            rc = self._Publisher.send(key, msg )\n";
            fhSource << "            self._" << prop->Attribute("id") << " = msg\n";
            fhSource << "        return rc\n";
        } else {
            fhSource << "        return self._Publisher.send(key, msg )\n";
        }

        fhSource << std::endl;
        fhSource << std::endl;
    }

    fhSource << "    def receive(self) -> None:" << std::endl;
    fhSource << "        msgAvailable = True" << std::endl;

    fhSource << "        while msgAvailable:" << std::endl;
    fhSource << "            (key, msgSize, msgData, ident) = self._Publisher.receive()" << std::endl;
    fhSource << "            if key is not None:" << std::endl;
    fhSource << "                logging.info(f\"" << ifcName << "Stub::receive: key {key}\")"
             << std::endl;
    fhSource << std::endl;
    fhSource << "                match key:" << std::endl;

    for (const auto *req : requests) {
        std::string strReq;
        strReq += "REQ_";
        strReq += YaComponentCore::to_upper(req->Attribute("id"));
        fhSource << "                    case self.KEYS." << strReq << ":\n";
        std::string strMember;

        auto *para = req->FirstChildElement("para");
        if (para) {
            fhSource << "                        if msgData is not None:" << std::endl;
        }
        while (para) {
            strMember += std::string("                            self._") + req->Attribute("id")
                         + "_" + para->Attribute("id") + ".ParseFromArray(msgData, msgSize)\n";
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
                fhSource << "                        self._" << req->Attribute("id") << "_"
                         << para->Attribute("id") << ".Clear();\n";
                para = para->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }

        fhSource << "                        self._callback.onRequest" << req->Attribute("id")
                 << "( self._id";
        std::string strPara;

        para = req->FirstChildElement("para");
        while (para)

        {
            strPara += std::string(", self._") + req->Attribute("id") + "_" + para->Attribute("id");
            para = para->NextSiblingElement("para");
        }

        resp = req->FirstChildElement("resp");
        while (resp) {
            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            para = resp_definition->FirstChildElement("para");
            while (para) {
                strPara += std::string(", self._") + req->Attribute("id") + "_"
                           + para->Attribute("id");
                para = para->NextSiblingElement("para");
            }
            resp = resp->NextSiblingElement("resp");
        }
        if (!strPara.empty()) {
            fhSource << strPara << " )\n";
        } else {
            fhSource << " )\n";
        }

        resp = req->FirstChildElement("resp");
        while (resp) {
            std::string strResp;
            strResp += "self.KEYS.RESP_";
            strResp += YaComponentCore::to_upper(resp->Attribute("id"));
            fhSource << "                        self.response" << resp->Attribute("id") << "( "
                     << strResp << ", ident";

            auto resp_definition = findResponse(responses, resp->Attribute("id"));
            para = resp_definition->FirstChildElement("para");
            while (para) {
                fhSource << ", self._" << req->Attribute("id") << "_" << resp->Attribute("id");
                para = para->NextSiblingElement("para");
            }
            fhSource << ")\n";

            resp = resp->NextSiblingElement("resp");
        }

        fhSource << "\n";
    }

    fhSource << "                    case _:\n";
    fhSource << "                        if  -1 < key:\n";
    fhSource << "                            raise RuntimeError(f\"" << ifcName
             << "Stub::receive() unknown key '{key}'\")" << std::endl;

    fhSource << std::endl;
    fhSource << "            else:" << std::endl;
    fhSource << "                msgAvailable = False" << std::endl;

    fhSource << std::endl << std::endl;

    for (const auto *req : requests) {
        std::string strPara;

        auto *para = req->FirstChildElement("para");
        while (para) {
            strPara += "    ";
            strPara += std::string(para->Attribute("id")) + " self._" + req->Attribute("id") + "_"
                       + para->Attribute("id") + "\n";
            para = para->NextSiblingElement("para");
        }

        auto *resp = req->FirstChildElement("resp");
        while (resp) {
            strPara += "    ";
            strPara += std::string(resp->Attribute("id")) + " m" + req->Attribute("id") + "_"
                       + resp->Attribute("id") + ";\n";
            resp = resp->NextSiblingElement("resp");
        }
    }

    fhSource.close();
    fhHeaderIfc.close();
}

const tinyxml2::XMLElement *YaComponentPyCodeGen::findResponse(const ElementList &responses,
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
