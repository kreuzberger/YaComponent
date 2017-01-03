#include "YaComponent.h"
#include "zmq.h"

#if (defined (WIN32))
#   include <windows.h>
#else
#   include <time.h>
#   include <sys/time.h>
#endif
namespace YaComponent
{
  void sleep (int msecs)
  {
  #if (defined (WIN32))
      Sleep (msecs);
  #else
      struct timespec t;
      t.tv_sec  =  msecs / 1000;
      t.tv_nsec = (msecs % 1000) * 1000000;
      nanosleep (&t, NULL);
  #endif
  }

  void* context_new()
  {
    return zmq_ctx_new ();
  }

}
