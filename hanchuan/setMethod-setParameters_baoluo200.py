import socket   
import time   
    
address = ('192.168.1.5', 5000)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)
print 'connect success!!!'
s.send('\x01\x00\x00\xd9{\"sequence\":\"98293837\",\"Method\":\"setParameters\",\"Parameters\":{\"depth\":40000,\"position\":10000,\"saving\":200,\"envelopeU\":[7,500,300,1000,100,2000,100,2000,300,40000,300,40000,100,50000,100],\"envelopeB\":[2,10000,50,50000,50]}}')
time.sleep(0.1)
s.close()
