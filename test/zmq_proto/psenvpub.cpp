
#include "psenvbase.h"
#include "YaPUBImpl.h"
#include "zmq.h"

#include <unistd.h>

enum key
{
    A = 0
  , B = 1
  , END = 2
};

int main (void)
{
// Prepare our context and publisher
void *context = zmq_ctx_new ();
YaPUBImpl oPubImpl(context);
oPubImpl.bind("ipc:///tmp/hardcore","ipc:///tmp/hardcoresync");

fprintf (stderr, "Waiting for subscribers\n");

while(!oPubImpl.checkSubscribers())
{

}

fprintf(stderr, "All subscribers connected, start sending data\n");

PSEnvBase moPSBase;

while (moPSBase.hasNext())
{
  QByteArray oData = moPSBase.getText().toUtf8();
  int rc = oPubImpl.send(B,oData.count(), oData.constData());
}
oPubImpl.send(END,0, 0);
oPubImpl.close();
printf("now exiting, all %d messages are sent\n", moPSBase.getMaxTextCnt());
zmq_ctx_destroy (context);
return 0;
}
