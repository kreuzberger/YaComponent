#include "YaSUBImpl.h"
#include "zmq.h"
#include "zmq_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "YaComponent.h"

enum key
{
  INVALID = -1
  , A = 0
  , B = 1
  , END = 2
};

int main (void)
{
// Prepare our context and subscriber
void *context = zmq_ctx_new ();
assert (context);

YaSUBImpl oSUB( context );
bool bConnect = oSUB.connect("ipc:///tmp/hardcore","ipc:///tmp/hardcoresync");
//bool bConnect = oSUB.connect("tcp://localhost:42123","tcp://localhost:42124");

assert( bConnect );

if( !bConnect )
{
    return(1);
}

oSUB.setNotification(B);
oSUB.setNotification(END);

YaComponent::sleep(1);

while(!oSUB.checkSync())
{
}


double charmax=10E8;
double charcnt=0;
void* watch = zmq_stopwatch_start();
int iKey = INVALID;
while (END != iKey) {
  int nbytes = oSUB.receive(iKey,0);

  if((int(charcnt/1000.0) % 1000) == 0) printf (".");
  charcnt += nbytes +1;
}

unsigned long microsecs = zmq_stopwatch_stop (watch);
double secs = microsecs/1000000.0;
printf("average: %g Mbytes / second\n", charcnt/1E6/secs);
printf("total number of messages: %d \n", oSUB.getMessageCnt());
oSUB.close();
zmq_ctx_destroy (context);

return 0;
}
