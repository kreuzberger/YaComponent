#ifndef PSENVBASE_H
#define PSENVBASE_H

#include <QtCore/QString>
#include <math.h>

class PSEnvBase
{
public:
    PSEnvBase();
    virtual ~ PSEnvBase();

    QString getText();
    void setMaxTextCnt(int iMax);
    int getMaxTextCnt() const { return miMaxTextCnt; }
    bool hasNext();
    void decrText() { miTextCnt--; }

private:
    QString moText;
    int miTextCnt;
    int miMaxTextCnt;
    int miPadding;
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
