from utils_test import req_check

'''
	Need:
		Page on /
		POST must be not accepted on /
		Nothing on /youyou.html
'''

### TEST

def	test_crlf():
	req_check("GET / HTTP/1.1\r\nhost: x\r\nconnection: close\r\n\r\n", 200, "close")
	req_check("GET / HTTP/1.1\r\nhost: x\r\n\r\n", 200)
	req_check("GET / HTTP/1.1\nhost: x\n\n", 200)
	req_check("GET / HTTP/1.1\nhost: x\r\n\n", 200)
	req_check("GET / HTTP/1.1\nhost: x\n\r\n", 200)
	req_check("GET / HTTP/1.1\r\nhost: x\n\n", 200)

def test_wrong_uri():
	req_check("GET /.. HTTP/1.1\r\nhost: x\r\n\r\n", 400)

#def	test_fail_403():
#	req_check("POST / HTTP/1.1\r\nhost: x\r\n\r\n", 403, "keep-alive")

def	test_fail_404():
	req_check("GET /youyou.html HTTP/1.1\r\nhost: x\r\n\r\n", 404, "keep-alive")
