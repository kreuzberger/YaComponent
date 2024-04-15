#include <yacomponent/YaBuffer.h>

#include "zmq.h"

namespace YaComponent {
void *context_new()
{
    return zmq_ctx_new();
}

} // namespace YaComponent
