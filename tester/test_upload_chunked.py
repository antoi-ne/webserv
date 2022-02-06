import pytest
from utils import connect_req, chunk_send, connect_one, get_res
from utils_test import common_check
from file import File

'''
	Need:
		location: /upload
		 upload_path == root
		 accept all methods
		
'''

@pytest.fixture
def file():
	return File("/Volumes/Storage/goinfre/vneirinc/webserv/tester/page/wopwop.txt")

def	test_chunked_upload(file):
	client = connect_one()
	chunk_send(client, "POST /upload/{} HTTP/1.1\r\nhost: x\r\ntransfer-encoding: chunked\r\n".format(file.file_name), file.content, 8)
	res = get_res(client)
	common_check(res, 201, "keep-alive")

def	test_retrieve(file):
	res = connect_req("GET /upload/{} HTTP/1.1\r\nhost: x\r\n\r\n".format(file.file_name))
	common_check(res, 200, "keep-alive")
	body = res.read().decode()
	assert len(body) == len(file.content)
	assert body == file.content