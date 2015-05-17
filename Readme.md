This is an example implementation of the AllJoyn interface for the Heatworks Model1 hot water heater.
 
This code has been tested on Linux and OS X. A Windows implementation is forthcoming.

It requires you have downloaded and compiled a recent version of the AllJoyn Thin Core (https://allseenalliance.org/developers/learn/core/thin-core) from https://allseenalliance.org/developers/download.
Once you have built that, set the environment variable AJTCL to point at the location.

To build this you should be able to invoke make:

> make

and then you'll have:

> model1_service
> model1_client

