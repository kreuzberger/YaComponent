Main concept
============


**Definition of a component**

A component should be a piece of code (i.e. normaly a simple class) the uses and/or provides interfaces.
The component could use interfaces from other components or provide interfaces to other components. The component
itself should not know anything about the process or thread its running in.

**Definition of a interface**

A interface should define
- properties
- requests
- responses

The properties should be used to transfer data or information through the interface. The components must set a 
notification on the property to receive them

properties could be all message types supported by google protobuf

Requests are triggers or procedure calls into the component providing the interface to start some actions, e.g. to start
data transmission or to get some status

Responses are procedure calls back to the component using the interface to get a "status" of an request (e.g. request is
accepted) or to handle returns for request (e.g. the id of an action started with this request).

**Definition of a layout**

A layout should define which components are used within applications (processes) and how they are organized in threads.
So independent from your components you could define several layouts for the same purposes



