#ifndef YASUBIMPL_H
#define YASUBIMPL_H

#include "YaBuffer.h"

class YaSUBImpl
{
  public:
    YaSUBImpl( void* context );
    virtual ~YaSUBImpl();
    bool connect( const char*, const char*);
    int receive(int& key, const char* pcData );
    int setNotification(int key);
    int clearNotification( int key);
    void close();
    int getMessageCnt() { return miMessageCnt; }
    bool checkSync();

  private:
    YaSUBImpl();
    YaSUBImpl( const YaSUBImpl& );
    YaSUBImpl& operator=( const YaSUBImpl& );

    void* mpSUBSocket;
    void* mpReqRespSocket;
    bool mbConnected;
    char *mpcKey;
    YaBuffer mMsgSizeBuffer;
    YaBuffer mMsgBuffer;
    int miMessageCnt;
};
#endif // YASUBIMPL_H
