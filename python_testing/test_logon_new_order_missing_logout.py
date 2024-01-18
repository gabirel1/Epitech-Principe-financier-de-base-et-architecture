import socket

PORT = 8080
HOST = "localhost"

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=67{chr(1)}35=A{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=1{chr(1)}52=20240112-19:06:49.787{chr(1)}98=0{chr(1)}108=30{chr(1)}10=166{chr(1)}"
NEW_ORDER_MISSING_MSG = f"8=FIX.4.2{chr(1)}9=116{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=2{chr(1)}52=20240112-19:06:49.787{chr(1)}11=1{chr(1)}21=3{chr(1)}38=10{chr(1)}40=2{chr(1)}44=100{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=067{chr(1)}"
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=55{chr(1)}35=5{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=3{chr(1)}52=20240112-19:06:49.787{chr(1)}10=128{chr(1)}"

msg_list = [
    {"msg": LOGON_MSG, "name": "logon"},
    {"msg": NEW_ORDER_MISSING_MSG, "name": "new_order_missing"},
    {"msg": LOGOUT_MSG, "name": "logout"}
]

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((HOST, PORT))
print(f"Connected to server at {HOST}:{PORT}")

for _msg in msg_list:
    print("sending message: ", _msg["name"], "\n")
    print("msg == ", _msg["msg"], "\n")
    socket.sendall(_msg["msg"].encode())
    data = socket.recv(1024).decode()
    print(f"Received message from server: '{data}'", "\n\n")

socket.close()