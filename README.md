YaComponent
===========

YaComponent is a framework for handling components and the interfaces of components.
YaComponent includes
 * XML description to define interfaces, components and deployment
 * Interface definition is done with google protobuf
 * perl base generator for c++ code for components and interfaces
 * implementation of the publish/subscriper and request/response pattern by using [libzmq](http://zeromq.org//)

Status
------

First working example provided with tests.
The documentation growed by adding some simple PlantUML design views.
Currently evaluating using only one port for both patterns with ZMQ_ROUTER/ZMQ_DEALER


![Main Concepts](doc/Concept.md)
[User Guide](http://kreuzberger.github.io/YaComponent/UserGuide.html)


