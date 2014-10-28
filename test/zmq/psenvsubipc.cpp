//
//  Pubsub envelope subscriber
//
// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>

#include "zmq.h"
#include "zmq_utils.h"
#include "zhelpers.h"

#include <assert.h>

#include <string.h>
int main (void)
{
  // Prepare our context and subscriber
  void *context = zmq_ctx_new ();
  void *subscriber = zmq_socket (context, ZMQ_SUB);
  zmq_connect (subscriber, "ipc:///tmp/hardcore");
  zmq_setsockopt (subscriber, ZMQ_SUBSCRIBE, "B", 1);

  // 0MQ is so fast, we need to wait a while…
  s_sleep (1);

  // Second, synchronize with publisher
  void *syncclient = zmq_socket (context, ZMQ_REQ);
  zmq_connect (syncclient, "ipc:///tmp/hardcoresync");

  // - send a synchronization request
  s_send (syncclient, "");

  // - wait for synchronization reply
  char *string = s_recv (syncclient);
  free (string);

  double charmax=10E8;
  double charcnt=0;
  void* watch = zmq_stopwatch_start();

  int iNew, iOld = 0;
  while (charcnt < charmax) {
    // Read envelope with address
    char buf [256];
    memset(buf,0,256);
    char address[2];
    memset(address,0,2);
    int nbytes = zmq_recv (subscriber, address, 1, 0); assert (nbytes != -1);
    nbytes = zmq_recv (subscriber, buf, 256, 0); assert (nbytes != -1);
    //printf ("[%s] %s\n", address, buf);
    sscanf(buf,"%d:",&iNew);
    if( iNew != iOld + 1)
    {
      fprintf(stderr,"missed %d packages\n", iNew-iOld);
    }
    assert(iNew == iOld+1);
    if( (iNew % 100000) == 0) fprintf(stderr,"received %d\n",iNew);
    iOld = iNew;

    charcnt += nbytes +1;
  }

  unsigned long microsecs = zmq_stopwatch_stop (watch);
  double secs = microsecs/1000000.0;
  printf("received %g Mbytes in %g secs\n",charmax/1E6,secs);
  printf("average: %g Mbytes / second\n", charcnt/1E6/secs);
  // We never get here but clean up anyhow
  zmq_close (subscriber);
  zmq_ctx_destroy (context);

  return 0;
}
