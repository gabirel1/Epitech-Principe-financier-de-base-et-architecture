import socket
import time

PORT = 8080
HOST = "localhost"

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=67{chr(1)}35=A{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=1{chr(1)}52=20240112-01:07:56.740{chr(1)}98=0{chr(1)}108=30{chr(1)}10=145{chr(1)}"
NEW_ORDER_SINGLE_1 = f"8=FIX.4.2{chr(1)}9=121{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=2{chr(1)}52=20240112-01:10:28.888{chr(1)}11=1{chr(1)}21=3{chr(1)}38=10{chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=010{chr(1)}"
NEW_ORDER_SINGLE_2 = f"8=FIX.4.2{chr(1)}9=120{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=3{chr(1)}52=20240112-01:10:28.888{chr(1)}11=2{chr(1)}21=3{chr(1)}38=5{chr(1)}40=2{chr(1)}44=100{chr(1)}54=4{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=224{chr(1)}"
NEW_ORDER_SINGLE_3 = f"8=FIX.4.2{chr(1)}9=121{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=4{chr(1)}52=20240112-01:10:28.888{chr(1)}11=3{chr(1)}21=3{chr(1)}38=50{chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=018{chr(1)}"
NEW_ORDER_SINGLE_4 = f"8=FIX.4.2{chr(1)}9=120{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=5{chr(1)}52=20240112-01:10:28.888{chr(1)}11=4{chr(1)}21=3{chr(1)}38=5{chr(1)}40=2{chr(1)}44=200{chr(1)}54=4{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=229{chr(1)}"
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=55{chr(1)}35=5{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=6{chr(1)}52=20240112-01:17:29.460{chr(1)}10=110{chr(1)}"

msg_list = [
    {"msg": LOGON_MSG, "name": "logon"},
    {"msg": NEW_ORDER_SINGLE_1, "name": "new_order1"},
    {"msg": NEW_ORDER_SINGLE_2, "name": "new_order2"},
    {"msg": NEW_ORDER_SINGLE_3, "name": "new_order3"},
    {"msg": NEW_ORDER_SINGLE_4, "name": "new_order4"},
    {"msg": LOGOUT_MSG, "name": "logout"}
]

socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((HOST, PORT))

for _msg in msg_list:
    print("sending message: ", _msg["name"], "\n")
    print("msg == ", _msg["msg"], "\n")
    socket.sendall(_msg["msg"].encode())
    data = socket.recv(1024).decode()
    print(f"Received message from server: '{data}'", "\n\n")
    time.sleep(1)

print("Market state should be: ")
print("Bid ==> orders: 2, price: 100, quantity: 55")
print("Ask ==> orders: 1, price: 200, quantity: 5")

socket.close()