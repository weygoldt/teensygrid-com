# TeensyGridCom

This repository is dedicated to solving the issue of orchestrating 
multiple multi-electrode logger units so to produce an electrode grid.

The units will be connected to a CAN-bus which is managed by a master
unit that is capable to monitor each units state as well as send 
simultaneous reset messages to restart all units periodically to keep them
in sync.

