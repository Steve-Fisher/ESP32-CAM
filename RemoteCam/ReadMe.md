Set Serial Monitor baud rate to 115200 (same as board baud rate)

Works fine when powered by powerpack and USB.
Works when powered by LiFePo4 battery directly attached to 3V3 and GND.  BUT ...
* Single 18500 LiFePo4 cell seems underpowered (although output amp spec says it should be OK).  Board has to be close to WiFi router, and even then data transfer rate over WiFi is very slow.
* Check LiFePo4 cell voltage before directly attaching to 3V3.  Must be below 3.6V (or will burnout the board).  A freshly charged LiFePo4 cell could have a voltage of 3.65v.
