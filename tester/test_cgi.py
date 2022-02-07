from utils import connect_req
from utils_test import common_check

'''
	Need:
		cgi who just send back request inside response body
		/cgi
'''

def test_cgi():
	req = "GET /cgi HTTP/1.1\r\nhost: x\r\n\r\n"
	[res, client] = connect_req(req)
	body = res.read().decode()
	common_check(res, client, 200)
	assert len(body) == len(req)
	assert body == req
