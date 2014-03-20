#ifndef PSENVBASE_H
#define PSENVBASE_H

#include <QtCore/QString>

class PSEnvBase
{
public:
    PSEnvBase();
    virtual ~ PSEnvBase();

    QString getText();
    void setMaxTextCnt(int iMax);
    bool hasNext();

private:
    QString moText;
    int miTextCnt;
    int miMaxTextCnt;
};

inline bool PSEnvBase::hasNext()
{
  return (miTextCnt < miMaxTextCnt);
}

inline void PSEnvBase::setMaxTextCnt( int iMax )
{
  miMaxTextCnt = iMax;
}

#endif // PSENVBASE_H
