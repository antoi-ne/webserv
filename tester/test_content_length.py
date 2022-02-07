import pytest
from utils import connect_req
from utils_test import req_check

def	test_fail_to_long():
	req_check("POST / HTTP/1.1\r\nhost: x\r\nContent-Length: 8\r\n\r\nmorethan8adhiiahdoah\r\n", 400)

def	test_success_fit():
	[res, client] = connect_req("POST /post HTTP/1.1\r\nhost: x\r\nContent-Length: 8\r\n\r\niam8char")
	assert res.status < 400 or res.status > 499