#pramga once

#include <QtCore/QObject>

class YaComponent : public QObject
{
    Q_OBJECT
public:
    YaComponent()
        : QObject()
    {}
    virtual ~YaComponent() {}
};

