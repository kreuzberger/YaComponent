#pragma once
#include "BasicReqRespTest.h"
#include <QtCore/QObject>
#include <QtCore/QThread>

class BasicReqRespGuiTest : public BasicReqRespTest
{
  Q_OBJECT
public:
  BasicReqRespGuiTest( QObject* parent = 0 );
  ~BasicReqRespGuiTest() {}

protected:
  void initComponents();
  void cleanupComponents();
};
