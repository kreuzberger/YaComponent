#ifndef TEXTUNITTEST_H
#define TEXTUNITTEST_H

#include <QObject>

#include "YaComponent.h"
#include "YaComponentThread.h"
#include "textgencompunittest.h"
#include "textoutcompunittest.h"


class TextUnittest : public QObject
{
  Q_OBJECT
public:
  TextUnittest(QObject *parent = 0);
  ~TextUnittest() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testInit();
    void testNotification();

private:
    void* mContext;
    YaComponentThread mTextGenThread;
    TextGenComp mTextGen1;
    YaComponentThread mTextOutThread;
    void* mContext2;
    TextOutComp mTextOut;
};

#endif // TEXTUNITTEST_H
