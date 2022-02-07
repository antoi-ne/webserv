from utils import connect_req

'''
	Utils assert for http response
'''

def req_check(request, exp_status, exp_conn = None):
	[res, client] = connect_req(request)
	common_check(res, client, exp_status, exp_conn)

def common_check(response, client, exp_status, exp_conn = None):
	assert response.version == 11
	assert response.status == exp_status
	check_conn(response, client, exp_status, exp_conn)

def check_conn(response, client, exp_status, exp_conn):
	if exp_conn != None:
		conn_check(response, client, exp_conn)
	else:
		match_conn(response, client, exp_status)

def match_conn(response, client, exp_status):
	if exp_status != 400:
		conn_check(response, client, "keep-alive")
	else:
		conn_check(response, client, "close")

def conn_check(response, client, exp_conn):
	conn = response.getheader("connection")
	assert conn == exp_conn
#	if (exp_conn == "keep-alive"):
#		assert client.fileno() != -1
#	else:
#		assert client.fileno() == -1