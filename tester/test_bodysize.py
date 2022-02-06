from utils_test import req_check, common_check
from utils import connect_one, chunk_send, get_res
'''
	Need:
		body_size <= 1024
			/post
			/upload upload accepted
'''

def req(path, size):
	content = 'A' * size 
	return "POST {} HTTP/1.1\r\nhost: x\r\nContent-Length: {}\r\n\r\n{}".format(path, size, content)

def test_fail_upload():
	req_check(req("/upload", 1025), 413)

def test_fail_post():
	req_check(req("/post", 1025), 413)

def test_fail_chunked():
	client = connect_one()
	cont = 'A' * 1000
	chunk_send(client, "POST /post HTTP/1.1\r\nhost: x\r\ntransfer-encoding: chunked\r\n", cont, 21)
	res = get_res(client)
	common_check(res, 413)

'''
	weird
def	test_content_length():
	req_check("POST /post HTTP/1.1\r\nhost: x\r\nContent-Length: 8\r\n\r\nmorethan8adhiiahdoah", 400)
'''