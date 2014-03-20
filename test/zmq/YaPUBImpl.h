#ifndef YAPUBIMPL_H
#define YAPUBIMPL_H

class YaPUBImpl
{
  public:
    YaPUBImpl( void* context );
    virtual ~YaPUBImpl();
    bool bind( const char* );
    int send(int key, int iDataCnt, const char* pcData );

  private:
    YaPUBImpl();
    YaPUBImpl( const YaPUBImpl& );
    YaPUBImpl& operator=( const YaPUBImpl& );

    void* mpPUBSocket;
    bool mbBound;
};

#endif // YAPUBIMPL_H
