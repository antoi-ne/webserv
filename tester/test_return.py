from utils import connect_req
from utils_test import common_check

'''
	Need:
		redirect 301 to google.com on /ret301
'''

def	test_301():
	res = connect_req("GET /ret301 HTTP/1.1\r\nhost: x\r\n\r\n")
	common_check(res, 301, "keep-alive")
	assert res.getheader("location") == "http://google.com"

