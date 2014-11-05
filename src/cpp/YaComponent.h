#ifndef YACOMPONENT_H
#define YACOMPONENT_H

#include <string.h>

namespace YaComponent {
  static const int KeySize = 2;
  static const int MessageSize = 8;
  static const char* KeyFmt = "%2d";
  static const char* MessageSizeFmt = "%8d";
  static const int MaxMessageSize = 20*1024;
  static const char* SynAck = "SYNC_ACK";
  char* socket_rcv (void *socket);
  int socket_snd(void *socket, const char *string);
  int socket_snd_more(void *socket, const char *string);
  void sleep(int msecs);
  void* context_new();
}


#endif // YAPUBSUB_H
