from utils_test import req_check

'''
	Need:
		location: /
		  methods GET
		location: /post
		  methods POST
		
'''
def	test_colon():
	req_check("GET / HTTP/1.1\r\nhost: x\r\n:\r\n", 400)

def	test_ws_colon():
	req_check("GET / HTTP/1.1\r\nhost: x\r\n :\r\n", 400)

def	test_ws_before_key():
	req_check("GET / HTTP/1.1\r\nhost: x\r\n hey:\r\n", 400)
	req_check("GET / HTTP/1.1\r\nhost: x\r\n\vhey:\r\n", 400)
