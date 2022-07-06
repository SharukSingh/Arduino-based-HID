# Search for available serial ports

import serial
import serial.tools.list_ports      #Function to iterate over serial ports

for port in serial.tools.list_ports.comports(): #Iterate over all serial ports
    print(port.device);
    print(port.vid);
    print(port.pid);
