from utils import *
from utils_test import common_check

'''
	Need:
		Page on /
		POST must be not accepted on /
		Nothing on /youyou.html
'''

def test_get_8_success():
	client = connect_one()
	one_req_div_send(client, "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n", 8)
	res = get_res(client)
	common_check(res, client, 200)

def test_get_8_fail():
	client = connect_one()
	one_req_div_send(client, "GET / HTTP/1.1\r\nHost: localhost\r\n : \r\n\r\n", 8)
	res = get_res(client)
	common_check(res, client, 400)
