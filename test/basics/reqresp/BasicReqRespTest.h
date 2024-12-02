#pragma once
#include <QtCore/QObject>
#include "WorkerCompImpl.h"
#include "ControlCompImpl.h"
#include <yacomponent/YaComponent.h>
#include <QtCore/QThread>

class WorkerComp : public YaComponent::WorkerCompImpl, public YaComponent::IWorkerIfcStubHandler
{
public:
  explicit WorkerComp( void* context )
    : YaComponent::WorkerCompImpl( context, ( *this ) )
  {
  }
  void onRequestText( int id, const IncomingMessage& in, OutgoingMessage& out ) override
  {
    mReqMessageCnt++;
    out.set_msg( in.msg() + std::string( " " ) + std::to_string( mReqMessageCnt ) );
  }

  int messageCount() const { return mReqMessageCnt; }

private:
  IncomingMessage mLastReqMessage;
  int mReqMessageCnt = 0;
};

class ControlComp : public YaComponent::ControlCompImpl, public YaComponent::IWorkerIfcProxyHandler
{

public:
  explicit ControlComp( void* context )
    : YaComponent::ControlCompImpl( context, ( *this ) )
  {
  }
  void onResponseText( int id, const OutgoingMessage& msg ) override
  {
    mLastRespMessage = msg;
    mRespMessageCnt++;
  }
  const OutgoingMessage& message() const { return mLastRespMessage; }
  int messageCount() const { return mRespMessageCnt; }

  int sendReq( const std::string& str )
  {
    IncomingMessage msg;
    msg.set_id( 1 );
    msg.set_msg( str );
    return mWorker.requestText( msg );
  }

private:
  OutgoingMessage mLastRespMessage;
  int mRespMessageCnt = 0;
};

class BasicReqRespTest : public QObject
{
  Q_OBJECT
public:
  BasicReqRespTest( QObject* parent = 0 );
  ~BasicReqRespTest() {}

signals:

private slots:

  void initTestCase() {}
  void cleanupTestCase();
  void testSingleReqResp();
  void testReqRespRepeated();
  void testMaxMessageSize();

protected:
  void initComponents();
  void cleanupComponents();

  void* mpContext = nullptr;
  QThread* mpWorkerThread = nullptr;
  WorkerComp* mpWorker = nullptr;

  // void* mpContext2 = nullptr;
  QThread* mpControlThread = nullptr;
  ControlComp* mpControl = nullptr;
};
