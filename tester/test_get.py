from utils_test import req_check, common_check
from utils import connect_one, one_req_div_send, get_res

'''
	Need:
		Page on /
		POST must be not accepted on /
		Nothing on /youyou.html
'''

### TEST

#def	test_startcrlf():
#	client = connect_one()
#	one_req_div_send(client, "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8)
#	res = get_res(client)
#	common_check(res, client, 200)

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
