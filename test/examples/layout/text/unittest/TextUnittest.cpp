#include "TextUnittest.h"
#include <QtTest>

QTEST_MAIN(TextUnittest);

TextUnittest::TextUnittest(QObject *parent)
  : QObject(parent)
  , mContext(YaComponent::context_new())
  , mTextGenThread()
  , mTextGen1( mContext )
  , mContext2()
  , mTextOutThread()
  , mTextOut( mContext )
{

}

void TextUnittest::cleanupTestCase()
{
  mTextGen1.deInit();
  mTextOut.deInit();

  mTextGenThread.quit();
  mTextOutThread.quit();
  QTest::qWait(100);
  QVERIFY(mTextGenThread.isFinished());
  QVERIFY(mTextOutThread.isFinished());
}

void TextUnittest::testInit()
{

  mTextGenThread.start();
  mTextGen1.init();
  mTextGen1.setConnectionParaTextGenXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync", 5000 );
  mTextGen1.setConnectionParaTextGenPlain( "inproc:///tmp/hurtmeplain", "inproc:///tmp/hurtmeplainsync", 5000 );
//  mTextGen1.moveToThread(&mTextGenThread);
  printf("calling move to thread\n");
  printf("starting thread\n");
  mTextOutThread.start();
  mTextOut.init();
  mTextOut.setConnectionParaXml( "inproc:///tmp/hurtmexml", "inproc:///tmp/hurtmexmlsync" );
//mTextOut.moveToThread(&mTextOutThread);

}


void TextUnittest::testNotification()
{
  mTextOut.setNotifications();
  mTextOut.requestStart();

  for( int i=1;i<3;i++)
  {
    QTest::qWait(YaComponent::TimeOut);
  }

  QCOMPARE(mTextOut.miPropertiesCnt,1);
}
