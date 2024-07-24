#include "TextUnittestMP.h"
#include <QtTest>

QTEST_MAIN( TextUnittestMP );

TextUnittestMP::TextUnittestMP( QObject* parent )
  : QObject( parent )
{
}

void TextUnittestMP::cleanupTestCase() {}

void TextUnittestMP::testMPMT()
{
  QString textgen = "./textgenMPMT";
  QString textout = "./textoutMPMT";
#ifdef Q_OS_WIN
  textgen = "textgenMPMT";
  textout = "textoutMPMT";
#endif
  moProcessTextGen.start( textgen, QStringList() );
  QVERIFY( moProcessTextGen.waitForStarted() );
  moProcessTextOut.start( textout, QStringList() );
  QVERIFY( moProcessTextOut.waitForStarted() );

  QVERIFY( moProcessTextOut.waitForFinished() );
  QVERIFY( moProcessTextGen.waitForFinished() );

  fprintf( stderr, "%s\n", moProcessTextGen.readAll().constData() );
  fprintf( stderr, "%s\n", moProcessTextOut.readAll().constData() );
}
