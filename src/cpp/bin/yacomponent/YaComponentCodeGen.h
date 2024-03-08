/*!
 * **********************************************************************************
 * Firma     PROCITEC GmbH    (C) Copyright PROCITEC GmbH 2022
 * **********************************************************************************
 * Dieses Computerprogramm ist urheberrechtlich geschuetzt(Paragraphen 69a ff UrhG).
 * Urheber ist die PROCITEC GmbH.
 *
 * Die Nutzung, Weitergabe und Vervielfaeltigung dieses Computerprogramms,
 * sowie Verwertung und Mitteilung seines Inhaltes ist durch den Vertrag
 * BWB E/UR1A/4A104/L5305 geregelt.
 *
 * Anderweitige Nutzung, Weitergabe und Vervielfaeltigung dieses Computer-
 * programms, sowie Verwertung und Mitteilung seines Inhaltes bedarf der
 * ausdruecklichen Zustimmung der PROCITEC GmbH.
 *
 * Zuwiderhandlung verpflichtet zum Schadensersatz.
 * **********************************************************************************
 */

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
