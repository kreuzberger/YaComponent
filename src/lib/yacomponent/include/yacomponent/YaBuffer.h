#pragma once
class YaBuffer
{
public:
  YaBuffer();
  YaBuffer( int size );
  ~YaBuffer();

  char* data() { return mpcData; }
  int size() const { return miDataCnt; }
  bool isEmpty() const { return 0 == miDataCnt; }
  void resize( int iSize );

private:
  YaBuffer( const YaBuffer& );
  YaBuffer& operator=( const YaBuffer& );
  char* mpcData;
  int miDataCnt;
};
