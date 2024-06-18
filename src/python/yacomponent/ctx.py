import zmq

def context_new():
    return zmq.Context()

def context_term( ctx : zmq.Context):
    ctx.destroy()


