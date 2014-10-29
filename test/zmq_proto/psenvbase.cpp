#include "psenvbase.h"

PSEnvBase::PSEnvBase()
: moText("%1: The quick brown fox jumps over the lazy dog 0123456789The quick brown fox jumps over the lazy dog 0123456789")
, miTextCnt(0)
, miMaxTextCnt(1E7)
, miPadding(8)
{
}

PSEnvBase::~PSEnvBase()
{

}


QString PSEnvBase::getText()
{
  miTextCnt++;
  return moText.arg(miTextCnt,miPadding);
}
