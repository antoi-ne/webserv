from constant import SERV1, SERV2
from utils_test import req_check

'''
	Need:
		3 servers on 8080
		server_names: serv1= serv1, 1serv; serv2= serv2
		each serv have loc on same as server_name with GET
		main serv have /main for GET
		
'''

def req(path, hostname):
	req_check("GET /{} HTTP/1.1\r\nhost: {}\r\n\r\n".format(path, hostname), 200)

def	test_serv1():
	req(SERV1, SERV1)

def	test_serv1_2():
	req(SERV1, "1serv")

def	test_serv2():
	req(SERV2, SERV2)

def	test_mainserv():
	req("main", "bloublou")

def test_no_host():
	req_check("GET / HTTP/1.1\r\npopo: lala\r\n\r\n", 400)