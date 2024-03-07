
#include "YaCompLayoutCodeGen.h"
#include <core/YaComponentCore.h>
#include <fstream>
#include <iostream>

YaCompLayoutCodeGen::YaCompLayoutCodeGen() {}

void YaCompLayoutCodeGen::write(const std::filesystem::path &codePath,
                                const std::string &componentName,
                                const ElementList &processes,
                                const AddressMap &adresses)
{
    for (auto *process : processes) {
        std::ofstream fhSource;

        auto filename = (codePath / (componentName + std::string(process->Attribute("name")) + ".cpp"))
                            .string();
        YaComponentCore::printDbg(std::string("write code file ") + filename);
        fhSource.open(filename, std::ios::out | std::ios::trunc);

        fhSource << "    void* context = YaComponent::context_new();" << std::endl;

        //YaComponentCore::printDbgXml(process);
        auto *thread = process->FirstChildElement("thread");
        while (thread) {
            YaComponentCore::printDbg(std::string("write code for thread ")
                                      + thread->Attribute("name"));
            fhSource << "    YaComponentThread " << thread->Attribute("name") << ";" << std::endl;
            fhSource << "    printf(\"starting thread\\n\");" << std::endl;
            fhSource << "    " << thread->Attribute("name") << ".start();" << std::endl;

            auto *component = thread->FirstChildElement("component");
            while (component) {
                writeComponentParts(fhSource, component, adresses);
                fhSource << "    " << component->Attribute("name")
                         << ".moveToThread(&$thread->{name});" << std::endl;
                fhSource << "    printf(\"calling move to thread\\n\");" << std::endl;
                component = component->NextSiblingElement();
            }

            thread = thread->NextSiblingElement();
        }

        // components associated with the main thread
        auto *component = process->FirstChildElement("component");
        while (component) {
            YaComponentCore::printDbg(std::string("write code for main thread "));
            writeComponentParts(fhSource, component, adresses);
            component = component->NextSiblingElement();
        }

        fhSource.close();
    }
}

void YaCompLayoutCodeGen::writeComponentParts(std::ofstream &fhSource,
                                              Element *component,
                                              const AddressMap &addresses)
{
    std::filesystem::path xmlfilename = component->Attribute("xml");

    fhSource << "    " << xmlfilename.stem().string() << " " << component->Attribute("name")
             << "(context);" << std::endl;

    auto *provides = component->FirstChildElement("provides");
    while (provides) {
        auto *ifc = provides->FirstChildElement("interface");
        while (ifc) {
            fhSource << "    " << component->Attribute("name") << ".setConnectionPara"
                     << ifc->Attribute("id") << "( \"" << ifc->Attribute("address") << "\"";
            if (ifc->Attribute("hwm")) {
                fhSource << ", " << ifc->Attribute("hwm") << " );" << std::endl;
            } else {
                fhSource << " );" << std::endl;
            }

            ifc = ifc->NextSiblingElement();
        }

        provides = provides->NextSiblingElement();
    }

    auto *uses = component->FirstChildElement("uses");
    while (uses) {
        auto *ifc = uses->FirstChildElement("interface");
        auto key = std::string(ifc->Attribute("sub"));
        if (addresses.find(key) != addresses.end()) {
            fhSource << "    " << component->Attribute("name") << ".setConnectionPara"
                     << ifc->Attribute("id") << "( \"";
            fhSource << addresses.at(key);
            fhSource << "\", \"";
            fhSource << component->Attribute("name");
            fhSource << "\" );" << std::endl;
        }

        while (ifc) {
            ifc = ifc->NextSiblingElement();
        }

        uses = uses->NextSiblingElement();
    }

    fhSource << "    " << component->Attribute("name") << ".init();" << std::endl;
}
