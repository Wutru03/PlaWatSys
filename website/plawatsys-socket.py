#!/usr/bin/python3
#sudo chmod +x /usr/lib/cgi-bin/plawatsys-socket.py
import cgi
import socket


print("Content-type: text/plain\n")


port = int(cgi.FieldStorage().getvalue("port"))


HOST = "plawatsys" # Replace with the hostname or IP address of your C socket server
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, port))
    s.sendall(b"measure request")
    data = s.recv(1024)


print(data.decode())
