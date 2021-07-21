#ifndef TEXTUNITTESTMP_H
#define TEXTUNITTESTMP_H

#include <QObject>

#include <QtCore/QProcess>

class TextUnittestMP : public QObject
{
    Q_OBJECT
public:
    TextUnittestMP(QObject *parent = 0);
    ~TextUnittestMP() {}

signals:

private slots:

    void initTestCase() {}
    void cleanupTestCase();
    void testMPMT();

private:
    QProcess moProcessTextGen;
    QProcess moProcessTextOut;
};

#endif // TEXTUNITTEST_H
