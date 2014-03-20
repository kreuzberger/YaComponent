#include "YaSUBImpl.h"
#include "zmq.h"
#include "zmq_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

enum key
{
    A = 0
  , B = 1
};

int main (void)
{
// Prepare our context and subscriber
void *context = zmq_ctx_new ();
assert (context);

YaSUBImpl oSUB( context );
bool bConnect = oSUB.connect("tcp://localhost:5563");
assert( bConnect );
if( !bConnect )
{
    return(1);
}

oSUB.setNotification(B);
double charmax=10E8;
double charcnt=0;
void* watch = zmq_stopwatch_start();

while (charcnt < charmax) {
  int nbytes = oSUB.receive(0);
  if((int(charcnt/1000.0) % 1000) == 0) printf (".");
  charcnt += nbytes +1;
}

unsigned long microsecs = zmq_stopwatch_stop (watch);
double secs = microsecs/1000000.0;
printf("\nreceived %g Mbytes in %g secs\n",charmax/1E6,secs);
printf("average: %g Mbytes / second\n", charcnt/1E6/secs);
// We never get here but clean up anyhow
zmq_ctx_destroy (context);

return 0;
}
