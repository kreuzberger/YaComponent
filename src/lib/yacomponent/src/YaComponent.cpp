#include <yacomponent/YaBuffer.h>

#include "zmq.h"
#include <QtCore/QtCore>

namespace YaComponent {
void *context_new()
{
    return zmq_ctx_new();
}

void context_term(void *context)
{
    auto ok = zmq_ctx_term(context);
    if (-1 == ok) {
        qFatal("%s", zmq_strerror(zmq_errno()));
    }
}

} // namespace YaComponent
