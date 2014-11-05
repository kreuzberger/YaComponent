#ifndef PSENVBASE_H
#define PSENVBASE_H

#include <math.h>
#include "TextGenerator.pb.h"
class PSEnvBase
{
public:
    PSEnvBase();
    virtual ~ PSEnvBase();

    const ::google::protobuf::MessageLite& getText();
    void setMaxTextCnt(int iMax);
    int getMaxTextCnt() const { return miMaxTextCnt; }
    bool hasNext();
    void decrText() { miTextCnt--; }

private:
    char moText[4096];
    int miTextCnt;
    int miMaxTextCnt;
    int miPadding;
    TextGenerator::Text moTextSend;
};

inline bool PSEnvBase::hasNext()
{
  return (miTextCnt < miMaxTextCnt);
}

inline void PSEnvBase::setMaxTextCnt( int iMax )
{
  miMaxTextCnt = iMax;
  miPadding = log10(miMaxTextCnt);
}

#endif // PSENVBASE_H
