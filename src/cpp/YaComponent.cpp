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
  char* socket_rcv (void *socket)
  {
      char buffer [256];
      int size = zmq_recv (socket, buffer, 255, ZMQ_NOBLOCK);
      if (size == -1)
          return NULL;
      if (size > 255)
          size = 255;
      buffer [size] = 0;
      return strdup (buffer);
  }

  int socket_snd (void *socket, const char *string)
  {
      int size = zmq_send (socket, string, strlen (string), 0);
      return size;
  }

  //  Sends string as 0MQ string, as multipart non-terminal
  int socket_send_more (void *socket, const char *string)
  {
      int size = zmq_send (socket, string, strlen (string), ZMQ_SNDMORE);
      return size;
  }

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
