import sys

data = sys.stdin.readlines()
print("HTTP/1.1 200 OK\r\n", end='')
print(''.join(data), end='')
exit(0)