#include "psenvbase.h"

PSEnvBase::PSEnvBase()
: moText("%1: The quick brown fox jumps over the lazy dog 0123456789")
, miTextCnt(0)
, miMaxTextCnt(50000)
{
}

PSEnvBase::~PSEnvBase()
{

}


QString PSEnvBase::getText()
{
  return moText.arg(miTextCnt,5);
  miTextCnt++;
}
