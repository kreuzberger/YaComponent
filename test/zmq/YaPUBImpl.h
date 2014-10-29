#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H

class YaPUBImpl
{

public:
  YaPUBImpl( void* context );
  virtual ~YaPUBImpl();
  bool bind( const char* address, const char* syncaddress);
  int send(int key, int iDataCnt, const char* pcData );
  bool checkSubscribers(int iNumExpectedSubscribers = 1);
  void close();

private:
  YaPUBImpl();
  YaPUBImpl( const YaPUBImpl& );
  YaPUBImpl& operator=( const YaPUBImpl& );

  void* mpPUBSocket;
  void* mpReqRespSocket;
  bool mbBound;
  int miSubscribersCnt;
};

#endif // YAPUBIMPL_H
