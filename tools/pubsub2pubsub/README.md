# The pubsub2pubsub service

The purpose of this service is to allow for a config driven conceptual publish-subscribe flow to be defined and operated, eliminating the need to build specific applications for specific interoperability problemsets. 
This service aims to have a plugin architecture driven approach that acts as a middleware glue solution.

## Roles & responsibilities hierarchy
The service as a whole ties together various building blocks to tie together various abstractions that together allow for a middleware abstracted formulation of pubsub behavior which can then be flow directed 
and managed across multiple different instantiations of said abstractions.
The overall roles & responsibilities hierarchy is as follows:

+----------------+         +---------+        +------+        +---------------+        +---------------------+
| pubsub2pubsub  |  1 -> N | channel | 1 -> M | side | 1 -> 1 | pubsub client | 1 -> X | pubsub client topic |
+----------------+         +---------+        +------+        +---------------+        +---------------------+

## What is a 'channel'?
A channel is the concept used to manage the 1:N relationship between the host processs and the value-added services running in said processs.
For dedicated node scenarios it provides a convenient single (1) command & control API for multiple (N) pubsub2pubsub routing networks
Every channel has completly independent "sides" which determine what data flows from where to where. Pinning threads to logical CPUs occurs at the per-channel level, if one choses to avail themselves of that feature.
It is possible to have one channel do UDP->Kafka and another Redis->STORAGE or any other combination of supported backends. However its recommended to keep the combinations homogenous across the host application 
as this helps to keep the load profile more homogenous as well resulting in more predictable bahavior across the entire process.
Channels also provide a convenient way of creating N channels from a single channel template, reducing the configuration burden on a per channel basis.

## What is a 'side'?
A side is a logical edge of the logical network you define on a per channel basis. (Right now the application only supports 2 sides, but allowing M number is a design goal down the line).
What a side is in real world terms completely depends on your configuration. 
For example, if you have traffic only flowing in one direction on your logical network than you could think of one side being the ingress side flowing the data towards the egress side.
If data flow is bi-direction such a definition makes less sense so you can see why going with a more abtsract definition of a 'side' affords the needed by-config-dictated latitude.
One could argue for calling it a network or transport interface but there as well the name may conjure up things that don't match reality based on config since one can have 'storage' as a side.
So really a side is just whatever you have defined it to be on that logical side of the data flow as dictated by the pubsub backend chosen for that side plus applicable config settings for said backend.
A side acts as the glue that ties the pubsub client abstraction into the larger context with more specific behavior appropriote, in this case, for the pubsub2pubsub context

