# PTPdComparation

The scope of this program is to compare the time difference between two jetson TK1 boards while it is synchronizing using the PTPd.

The program consists of two parts:

The first one is the Server. This can be executed in any machine, but not on the Jetson TK1 boards. It's role is to send a time to start comparing the time they have with the board. The actual time to start is hardwired to 5 seconds. Also the number of boards that can be connected is hardwired to 2, but it can be incremented to as many as it is needed.

The second part is the Client. The IP and PORT of the server the clients has to connect is hardwired and at the moment is 192.168.1.12:150. There has to be at least 2 clients. The reason of it, is because the sistem will use the GPIO to send a signal on the `gpio160` when it reaches X time to wake up. In this case X = 500 miliseconds. At the same time, the sistem will recive a signal at the `gpio161`, from the other board when has reach the same time, and will compare the diference between them. More informatión about the JetsonTK1 GPIO can be found [here](http://elinux.org/Jetson/GPIO).

![Alt text](DSC_0662.JPG?raw=true "Optional Title")

In order to compilate the code you have to be in a UNIX sistem, enter en client and do:

```
mkdir build
cd build
cmake ..
make
```

For the server you just have to enter in the server folder and do `make`.

# TO DO:
- Change the sending tipe to broadcast.
