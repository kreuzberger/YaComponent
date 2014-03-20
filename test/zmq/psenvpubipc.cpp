#include "psenvbase.h"

#include "zmq.h"

#include <unistd.h>



int main (void)
{
// Prepare our context and publisher
void *context = zmq_ctx_new ();
void *publisher = zmq_socket (context, ZMQ_PUB);
zmq_bind (publisher, "ipc:///tmp/hardcore");

PSEnvBase moPSBase;

while (moPSBase.hasNext()) {
// Write two messages, each with an envelope and content
//int rc = zmq_send(publisher,"A", 1, ZMQ_SNDMORE);
//rc = zmq_send(publisher,"We don't want to see this", 25,0);
int rc = zmq_send(publisher,"B", 1, ZMQ_SNDMORE);
QByteArray oText = moPSBase.getText().toUtf8();
rc = zmq_send(publisher,oText.constData(), oText.count(),0);
}
// We never get here but clean up anyhow
zmq_close (publisher);
zmq_ctx_destroy (context);
return 0;
}
