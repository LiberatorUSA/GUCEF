# The pubsub2pubsub service

The purpose of this service is to allow for a config driven conceptual publish-subscribe flow to be defined and operated, eliminating the need to build specific applications for specific interoperability problemsets. 
This service aims to have a plugin architecture driven approach that acts as a middleware glue solution.

## Roles & responsibilities hierarchy
The service as a whole ties together various building blocks to tie together various abstractions that together allow for a middleware abstracted formulation of pubsub behavior which can then be flow directed 
and managed across multiple different instantiations of said abstractions.
The overall roles & responsibilities hierarchy is as follows:

```
+----------------+         +---------+        +------+        +---------------+        +---------------------+
| pubsub2pubsub  |  1 -> N | channel | 1 -> M | side | 1 -> 1 | pubsub client | 1 -> X | pubsub client topic |
+----------------+         +---------+        +------+        +---------------+        +---------------------+
                                | 1              /\ M
                                | 1               | 
                           +-------------+ 1      |
                           | flow router |<-------|
                           +-------------+						   
```

## What is a 'channel'?
A channel is the concept used to manage the 1:N relationship between the host processs and the value-added services running in said processs.
For dedicated node scenarios it provides a convenient single (1) command & control API for multiple (N) pubsub2pubsub routing networks
Every channel has completly independent "sides" which determine what data flows from where to where. Pinning threads to logical CPUs occurs at the per-channel level, if one choses to avail themselves of that feature.
It is possible to have one channel do UDP->Kafka and another Redis->STORAGE or any other combination of supported backends. However its recommended to keep the combinations homogenous across the host application 
as this helps to keep the load profile more homogenous as well resulting in more predictable behavior across the entire process.
Channels also provide a convenient way of creating N channels from a single channel template, reducing the configuration burden on a per channel basis.
Note that every channel has at minimum its own thread, this is not optional, contrary to many other areas where the threading model can be altered config time.

## What is a 'side'?
A side is a logical edge of the logical network you define on a per channel basis. (Right now the application only supports 2 sides, but allowing M number is a design goal down the line).
What a side is in real world terms completely depends on your configuration. 
For example, if you have traffic only flowing in one direction on your logical network than you could think of one side being the ingress side flowing the data towards the egress side.
If data flow is bi-direction such a definition makes less sense so you can see why going with a more abtsract definition of a 'side' affords the needed by-config-dictated latitude.
One could argue for calling it a network or transport interface but there as well the name may conjure up things that don't match reality based on config since one can have 'storage' as a side.
So really a side is just whatever you have defined it to be on that logical side of the data flow as dictated by the pubsub backend chosen for that side plus applicable config settings for said backend.
A side acts as the glue that ties the pubsub client abstraction into the larger context with more specific behavior appropriote, in this case, for the pubsub2pubsub context.
Note that every side can have its own thread if so desired per the config. If one uses that ability for very side then the total number of generic application threads would be (main(1) + nrOfChannels(x) * nrOfSides(y)) so threads=x*y+1

## What is a 'flow router'?
As the name might suggest a flow router is intended to direct traffic between sides in a config driven manner. It takes routes defined in config and builds from those a flow network along which to route traffic.
The simplest route is 'any' to 'any' denoted using wildcards which does not impose any restrictions on how traffic flows between sides. 
As part of routing the message flow the routes you define allow you to identify sides with special purpose beyond just the regular 'from' -> 'to' style routing.
You can also specify a side to act as a failover side for a route or a dead letter destination. Lastly you can also define a side to act as a special route called a 'spillover bufffer'
A spillover buffer is a concept that is intended to act as an emergency buffer between sides to prevent messages getting backed up in the 'from' side during an outage of regular primary and failover routes.
In such a case the spillover will act as a temporary holding area for messages that is not just in-memory but acts as a real interim destination. The critical difference with a 'failover' is that
contrary to a failover the spillover is not a 'final' destination but merely a stop along the route. Once the primary or failover route is reestablished the spillover will act as a 'from' side
and it will empty its data towards the reestablished primary or failover routes, after which regular flow resumes.

## The pubsub 'STORAGE' backend
The storage backend was designed to allow for block based recording and replay of any messages from any of the other backends
This provides a singular performant option regardless of the other backends used. In order to achieve this it handles messages as-is without making any assumptions about what is or isnt available on a given message.
such messages are serialized into buffers using a binary format and subsequently written to the VFS as a single block, a container of messages.
The message containers are more than just a raw recording of messages. They include a header and a footer for verification, faster seeking and limited recovery / detection of corrupted and or trunkated files.
The binary format uses the Galaxy Unlimited Platform provided binary serialization code to ensure compatibility. Because the pubsub system at large makes a lot of use of Variant storage every format/type the variant can represent we can also store faithfully.
Note that on a per topic basis the backend only supports mutually exclusive ingress or egress, not both at the same time.
For performance reasons dynamic memory management is kept to an absolute minimum and for this reason buffers are continually reused for serialization of the blocks of messages.
In order to get the best performance it is key to choose the backend settings such to avoid alterations to the in-memory storage. This is easier on egress (writing out blocks) then it is on ingress (reading blocks into memory) especially when compression is used.
