import socket
import threading
import logging
import time

SERVER_ADDRESS = 'localhost'
SERVER_PORTS = [9000, 9001]
CLIENTS_PER_PORT = 10

# def client_thread(host, port, id):
# 	client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 	client.connect((host, port))
# 	client.send("client " + id + " on port " + port)

# if __name__ == "__main__":
# 	threads = []

# 	for port in SERVER_PORTS:
# 		for i in range(CLIENTS_PER_PORT):
# 			t = threading.Thread(target=client_thread, args=(SERVER_ADDRESS, port, i))
# 			t.start()
# 			threads.append(t)

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect(("localhost", 80))
client.send("G".encode())
client.send("E".encode())
client.send("T / HTTP/1.1\r\n\r\n".encode())
time.sleep(1);
x = client.recv(50000).decode()
print(x)