#include "psenvbase.h"

#include "YaComponent.h"
#include "zmq.h"


#include <unistd.h>



int main (void)
{
  // Prepare our context and publisher
  void *context = zmq_ctx_new ();
  void *publisher = zmq_socket (context, ZMQ_PUB);
  zmq_setsockopt(publisher,ZMQ_XPUB_NODROP,0,0);
  int hwm = 1E5;
  zmq_setsockopt(publisher,ZMQ_SNDHWM,&hwm,sizeof(hwm));
  zmq_bind (publisher, "ipc:///tmp/hardcore");

  // Socket to receive signals
  void *syncservice = zmq_socket (context, ZMQ_REP);
  zmq_bind (syncservice, "ipc:///tmp/hardcoresync");

  printf ("Waiting for subscribers\n");
  int subscribers = 0;
  while (subscribers < 1) {
    // - wait for synchronization request
    char *string = YaComponent::socket_rcv(syncservice);
    free (string);
    // - send synchronization reply
    YaComponent::socket_snd (syncservice, "");
    subscribers++;
  }

  fprintf (stderr, "Broadcasting messages\n");

  PSEnvBase moPSBase;

  while (moPSBase.hasNext()) {
    // Write two messages, each with an envelope and content
    //int rc = zmq_send(publisher,"A", 1, ZMQ_SNDMORE);
    //rc = zmq_send(publisher,"We don't want to see this", 25,0);
    int rc = zmq_send(publisher,"B", 1, ZMQ_SNDMORE);
    QByteArray oText = moPSBase.getText().toUtf8();
    rc = zmq_send(publisher,oText.constData(), oText.count(),0);
    if(rc == -1 && EAGAIN == errno)
    {
      moPSBase.decrText();
      YaComponent::sleep(1);
    }
  }
  // We never get here but clean up anyhow
  zmq_close (publisher);
  zmq_ctx_destroy (context);
  return 0;
}
