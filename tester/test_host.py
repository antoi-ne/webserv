from constant import SERV1, SERV2
from utils_test import req_check

'''
	Need:
		3 servers on 8080
		server_names: serv1 serv2
		each serv have loc on same as server_name with GET
		main serv have /main for GET
		
'''

def	test_serv1():
	res = req_check("GET /{} HTTP/1.1\r\nhost: {}\r\n\r\n".format(SERV1, SERV1), 200)

def	test_serv2():
	res = req_check("GET /{} HTTP/1.1\r\nhost: {}\r\n\r\n".format(SERV2, SERV2), 200)

def	test_mainserv():
	res = req_check("GET /main HTTP/1.1\r\nhost: x\r\n\r\n", 200)