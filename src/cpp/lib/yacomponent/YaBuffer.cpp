#include "YaBuffer.h"

#include <math.h>

YaBuffer::YaBuffer()
    : mpcData(0)
    , miDataCnt(0)
{}

YaBuffer::YaBuffer(int iSize)
    : mpcData(0)
    , miDataCnt(0)
{
    resize(iSize);
}

YaBuffer::~YaBuffer()
{
    delete[] mpcData;
    miDataCnt = 0;
}

void YaBuffer::resize(int iLength)
{
    if (miDataCnt != iLength) {
        delete[] mpcData;
        miDataCnt = pow(2, ceil(log2(iLength)));
        mpcData = new char[miDataCnt];
    }
}
