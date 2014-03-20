
#include "psenvbase.h"
#include "YaPUBImpl.h"
#include "zmq.h"

#include <unistd.h>

enum key
{
    A = 0
  , B = 1
};

int main (void)
{
// Prepare our context and publisher
void *context = zmq_ctx_new ();
YaPUBImpl oPubImpl(context);
oPubImpl.bind("tcp://*:5563");

PSEnvBase moPSBase;

while (moPSBase.hasNext())
{
  QByteArray oData = moPSBase.getText().toUtf8();
  int rc = oPubImpl.send(B,oData.count(), oData.constData());
}

zmq_ctx_destroy (context);
return 0;
}
