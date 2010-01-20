*
*  ServerPortExtender
*  Copyright (C) 2002 - 2010.  Dinand Vanvelzen
*
*  This software is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This software is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
*


About:
------------------------

The ServerPortExtender (SPE) tool is meant to allow TCP based clients to connect
to TCP based servers behind a NAT firewall without having to do any port 
forwarding. In some cases port forwarding is not an option because the NAT
gateway/firewall is not under your control. As such the only kind of connection
you can make from that location is an outgoing one. This is where the SPE tool
comes in. As the name suggests it "extends" the TCP server port beyond the NAT
boundry to the location where the SPE server is running. Thus allowing clients
to connect at the SPE server location instead of the actual server location which 
is unreachable due to NAT. Lets take the application UltraVNC as an example case:

Normally:
[UltraVNC client] -> [UltraVNC Server]

This normall won't work with the server behind NAT, but with SPE it will:
[UltraVNC client] -> [SPE Server] -> [SPE Client] -> [UltraVNC Server]

This works because reversed connections (outgoing connections from the NAT
UltraVNC server's network viewpoint) are being established. To this end the
SPE server actually runs 3 seperate TCP server ports. One acts as a control
port to control SPE functionality, another acts as the server port for the 
reversed connections coming in from the SPE client. Finally you have the server
port used by actual clients, in this example the UltraVNC client.

Chain of events:

1: SPE control connection is established
[SPE Client] -> [SPE Server]

2: Client connects to SPE server
[UltraVNC client] -> [SPE Server]

3: SPE server asks the SPE client to establish a new reversed (outgoing) connection
[SPE Server] -> [SPE Client]

4: SPE Client connects to the real server
[SPE Client] -> [UltraVNC server]

5: SPE Client establishes a new reversed (outgoing) connection
[SPE Client] -> [SPE Server]

6: The connection is now fully routed and established
[UltraVNC client] -> [SPE Server] -> [SPE Client] -> [UltraVNC Server]

Note that is besides the NAT issue you are also limited to HTTP connections
then you will have route the SPE via a HTTP tunnel. SPE does not include HTTP
tunneling itself at this time.


Features:
------------------------

- Establish connections to a TCP based server behind a NAT firewall


Usage: Overview
------------------------

1 - Run the SPE server at the location where you want your actual clients to connect
2 - Run the SPE client at the location where the actual server is directly reachable
3 - Enjoy!


Usage: Command line parameters
------------------------

If no command line arguments are given then the tool will start processing 
the directory structure using the current working directory as the root.

Like most GU applications command line parameters accepted in the form:
'<key>=<value>' '<key>=<value>' etc

The tool accepts the following command line parameters:

- Optional command to enable or disable showing the console output window
  'showConsole=<yes/no/true/false/1/0>'
     Example:
        'showConsole=yes'
  The default behaviour is that this functionality is enabled.

- Optional command to enable or disable running a diagnostic self-test
  'runTest=<yes/no/true/false/1/0>'
    Example:
      'runTest=no'
  The default behaviour is that this functionality is disabled.

- Optional command to specify the Application Role, or appRole for short.
  The appRole is whether the application acts as an SPE server or an SPE client
   'appRole=<client/server>'
     Example:
       'appRole=client'
  The default behaviour is that that the application will run as a SPE server.

The following command line parameters depend on whether the appRole is SPE Server
or SPE Client. So make sure you use the appropriote set.

For SPE Server's only:

- Optional command to set the port on which actual clients will connect
   'extendedServerPort=<port number>'
     Example:
       'extendedServerPort=10234'
  The default behaviour is for 'extendedServerPort' to be set to 10234

- Optional command to set the port on which the SPE control connection is established
   'serverControlPort=<port number>'
     Example:
       'serverControlPort=10236'
  The default behaviour is for 'serverControlPort' to be set to 10236

- Optional command to set the port on which actual clients will connect
   'reversedServerPort=<port number>'
     Example:
       'reversedServerPort=10235'
  The default behaviour is for 'reversedServerPort' to be set to 10235

For SPE Clients's only:

- Optional command to set the port on which the actual server is running
   'actualServerPort=<port number>'
     Example:
       'actualServerPort=10234'
  The default behaviour is for 'actualServerPort' to be set to 10234

- Optional command to set the DNS hostname or IP on which the actual server is running
   'actualServerHostname=<DNS or IP of actual server>'
     Example:
       'actualServerHostname=localhost'
  The default value for 'actualServerHostname' is "localhost"

- Optional command to set the DNS hostname or IP on which the SPE server is running
   'speServerHostname=<DNS or IP of actual server>'
     Example:
       'speServerHostname=example.no-ip.org'
  The default value for 'speServerHostname' is "localhost"

- Optional command to set the port on which the SPE server is accepting control connections
   'speServerControlPort=<port number>'
     Example:
       'speServerControlPort=10236'
  The default behaviour is for 'speServerControlPort' to be set to 10236
  This value should be the same as whatever you have 'serverControlPort' set to on the SPE server.

- Optional command to set the port on which the SPE server is accepting control connections
   'reversedServerPort=<port number>'
     Example:
       'reversedServerPort=10235'
  The default behaviour is for 'reversedServerPort' to be set to 10235
  This value should be the same as whatever you have 'reversedServerPort' set to on the SPE server.


History:
------------------------

- 1'th Nov 2009: First release