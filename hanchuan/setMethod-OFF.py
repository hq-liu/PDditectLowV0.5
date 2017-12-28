import socket   
import time   
    
address = ('192.168.1.5', 5000)    
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)
s.send('\x01\x00\x00\x26{\"Sequence\":\"23209322\",\"Method\":\"OFF\"}')
time.sleep(0.1)
s.close()

