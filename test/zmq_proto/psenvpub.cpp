
#include "psenvbase.h"
#include "YaPUBImpl.h"
#include "zmq.h"

#include <unistd.h>

#include "YaComponent.h"


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
oPubImpl.setConnectionPara("ipc:///tmp/hardcore","ipc:///tmp/hardcoresync",5000);
//oPubImpl.bind("tcp://*:42123","tcp://*:42124");

fprintf (stderr, "Waiting for subscribers\n");

while(!oPubImpl.checkSubscribers())
{

}

fprintf(stderr, "All subscribers connected, start sending data\n");

PSEnvBase moPSBase;
while (moPSBase.hasNext())
{
 // txt2Send.set_text(qPrintable(moPSBase.getText()));
//  moPSBase.getText();
  int rc = oPubImpl.send(B,moPSBase.getText());
  if( -1 == rc)
  {
    assert( errno == EAGAIN );
    moPSBase.decrText();
    YaComponent::sleep(1);
  }
}
oPubImpl.send(END,0,0);
oPubImpl.close();
printf("now exiting, all %d messages are sent\n", moPSBase.getMaxTextCnt());
zmq_ctx_destroy (context);
return 0;
}
