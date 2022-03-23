from utils_test import req_check

def test_invalid():
	req_check("spj / HTTP/1.1\r\nhost: x\r\n\r\n", 400)
	req_check("GeT / HTTP/1.1\r\nhost: x\r\n\r\n", 400)
