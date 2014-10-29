#ifndef YASUBIMPL_H
#define YASUBIMPL_H



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

  private:
    YaSUBImpl();
    YaSUBImpl( const YaSUBImpl& );
    YaSUBImpl& operator=( const YaSUBImpl& );
    bool checkSync();

    void* mpSUBSocket;
    void* mpReqRespSocket;
    bool mbConnected;
    char *mpcKey;
    char *mpcMessageSize;
    char* mpcMessage;
    int miMessageCnt;
};
#endif // YASUBIMPL_H
