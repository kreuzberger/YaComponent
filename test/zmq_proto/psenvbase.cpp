#include "psenvbase.h"

#include <stdio.h>

PSEnvBase::PSEnvBase()
: moText()
, miTextCnt(0)
, miMaxTextCnt(1E7)
, miPadding(8)
{
}

PSEnvBase::~PSEnvBase()
{

}


const ::google::protobuf::MessageLite* PSEnvBase::getText()
{
  miTextCnt++;
  sprintf(moText,"%08d: The quick brown fox jumps over the lazy dog 0123456789The quick brown fox jumps over the lazy dog 0123456789",miTextCnt);
  moTextSend.set_text(moText);
  return &moTextSend;
}
