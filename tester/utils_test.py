from utils import connect_req

'''
	Utils assert for http response
'''

def req_check(request, exp_status, exp_conn = None):
	res = connect_req(request)
	common_check(res, exp_status, exp_conn)

def common_check(response, exp_status, exp_conn = None):
	assert response.version == 11
	assert response.status == exp_status
	check_conn(response, exp_status, exp_conn)

def check_conn(response, exp_status, exp_conn):
	if exp_conn != None:
		conn_check(response, exp_conn)
	else:
		match_conn(response, exp_status)

def match_conn(response, exp_status):
	if exp_status != 400:
		conn_check(response, "keep-alive")
	else:
		conn_check(response, "close")

def conn_check(response, exp_conn):
	conn = response.getheader("connection")
	assert conn == exp_conn
	#if (exp_conn == "keep-alive"):
	#	assert response.closed == False
	#else:
	#	assert response.closed == True