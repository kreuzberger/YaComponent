#pragma once

namespace YaComponent {
static const int KeySize = 4;
static const int MessageSize = 8;
static const char *KeyFmt = "%4d";
static const char *MessageSizeFmt = "%8d";
static const int MaxMessageSize = 20 * 1024;
static const int MaxIdentSize = 512;
static const char *SynAck = "SYNC_ACK";
static const int KeySync = -2;
static const int KeyEnd = -3;
static const int KeySetNotification = -4;
static const int KeyClearNotification = -5;
static const int TimeOut = 20;
void *context_new();
void context_term(void *context);
} // namespace YaComponent
