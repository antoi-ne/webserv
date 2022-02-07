import pytest
from os import path
from utils import connect_req, chunk_send, connect_one
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
	script_dir = path.dirname(__file__)
	return File(script_dir + "/page/hello_world.txt")

def	test_upload(file):
	res = connect_req("POST /upload/{} HTTP/1.1\r\nhost: x\r\nContent-Length: {}\r\n\r\n{}".format(file.file_name, str(len(file.content)), file.content))
	common_check(res, 201, "keep-alive")

def	test_retrieve(file):
	res = connect_req("GET /upload/{} HTTP/1.1\r\nhost: x\r\n\r\n".format(file.file_name))
	common_check(res, 200, "keep-alive")
	body = res.read().decode()
	assert len(body) == len(file.content)
	assert body == file.content
