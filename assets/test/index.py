import sys

data = sys.stdin.readlines()
print("HTTP/1.1 200 OK")
print(''.join(data))
exit(0)