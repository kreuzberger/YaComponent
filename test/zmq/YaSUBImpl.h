#ifndef YASUBIMPL_H
#define YASUBIMPL_H

class YaSUBImpl
{
  public:
    YaSUBImpl( void* context );
    virtual ~YaSUBImpl();
    bool connect( const char* );
    int receive(const char* pcData );
    int setNotification(int key);
    int clearNotification( int key);

  private:
    YaSUBImpl();
    YaSUBImpl( const YaSUBImpl& );
    YaSUBImpl& operator=( const YaSUBImpl& );

    void* mpSUBSocket;
    bool mbConnected;
};
#endif // YASUBIMPL_H
