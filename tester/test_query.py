from utils_test import req_check

'''
	Need page on /
'''

def	test_success():
	req_check("GET /?aa=bb HTTP/1.1\r\nhost: x\r\n\r\n", 200)

def	test_query_http_ver():
	req_check("GET /?aa=bb&v=HTTP/1.1&cc=dd HTTP/1.1\r\nhost: x\r\n\r\n", 200)

def	test_fail_char():
	req_check("GET /?aa=b\vb&c=dd HTTP/1.1\r\nhost: x\r\n\r\n", 400)

def	test_fail_pos():
	req_check("GET / HTTP/1.1?aa=bb\r\nhost: x\r\n\r\n", 400)
	req_check("GET ?aa=bb/ HTTP/1.1\r\nhost: x\r\n\r\n", 400)
