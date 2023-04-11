#!/usr/bin/python3
#sudo chmod +x /usr/lib/cgi-bin/toggle.py
import cgi
import socket


port = int(cgi.FieldStorage().getvalue("port"))
Id = int(cgi.FieldStorage().getvalue("plantId"))






print("Content-type: text/plain\n")


HOST = "plawatsys" # Replace with the hostname or IP address of your C socket server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, port))
    s.send(b"toggle request")
    s.recv(1024)
    s.send(str(Id).encode())
    data = s.recv(1024)


print(data.decode())
