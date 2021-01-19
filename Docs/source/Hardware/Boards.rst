********
Boards
********

Fan Board: Manthan Upadhyaya
=============================

Purpose
    The purpose of this board is to set the speed of the fans that will be used to cool the battery 
    pack. It recieves signals from the Master Board and sends them across a MOSFET that switches the 
    fan on or off.

Blocks
    Mosfets - Each mosfet is between the power provided by the Master Board and power provided to the
    fans. When the PWM signal is set, it switches the fan on or off at a certain duty cycle.

Connections
    6 Connector: 4 of these connections are the PWM signals sent by the Master board. The other 2
    are power and ground.
    
    2 Connector(4): These are the power and grounds each fan is connected to.

Additional Considerations
    None
