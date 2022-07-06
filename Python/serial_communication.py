# Register keypress and send volume level over serial

import serial, time, osascript
from pynput import keyboard

serialPort = serial.Serial('/dev/cu.usbmodemHIDJB1', 115200)
serialPort.timeout = 0

def keypress(key):
    if str(key) == "Key.media_volume_up" or str(key) == "Key.media_volume_down" or str(key) == "Key.media_volume_mute":
        result = osascript.osascript("get volume settings")
        volumeLevel = result[1].split(',')[0].replace('output volume:','')
        volumeLevel = volumeLevel + "\n"
        serialPort.write(volumeLevel.encode())
        print(volumeLevel)

# Collect events until released
with keyboard.Listener(
        on_press=keypress) as listener:
    listener.join()
