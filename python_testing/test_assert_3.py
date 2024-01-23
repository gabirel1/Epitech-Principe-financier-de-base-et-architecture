import socket

PORT = 8080
HOST = "localhost"

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=57{chr(1)}35=A{chr(1)}49=1{chr(1)}56=0{chr(1)}34=1{chr(1)}52=20240116-00:33:33.247{chr(1)}98=0{chr(1)}108=30{chr(1)}10=090{chr(1)}"
NEW_ORDER_SINGLE_1 = f"8=FIX.4.2{chr(1)}9=111{chr(1)}35=D{chr(1)}49=1{chr(1)}56=0{chr(1)}34=2{chr(1)}52=20240116-01:13:13.836{chr(1)}11=1{chr(1)}21=3{chr(1)}38=10{chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=206{chr(1)}"
NEW_ORDER_SINGLE_2 = f"8=FIX.4.2{chr(1)}9=111{chr(1)}35=D{chr(1)}49=1{chr(1)}56=0{chr(1)}34=3{chr(1)}52=20240116-01:15:18.481{chr(1)}11=2{chr(1)}21=3{chr(1)}38=15{chr(1)}40=2{chr(1)}44=200{chr(1)}54=4{chr(1)}55=EUR/USD{chr(1)}60=20210201-10:00:000{chr(1)}10=218{chr(1)}"
ORDER_CANCEL_REPLACE = f"8=FIX.4.2{chr(1)}9=116{chr(1)}35=G{chr(1)}49=1{chr(1)}56=0{chr(1)}34=4{chr(1)}52=20240116-01:27:45.571{chr(1)}11=1{chr(1)}21=3{chr(1)}38=25{chr(1)}40=2{chr(1)}41=1{chr(1)}44=150{chr(1)}55=EUR/USD{chr(1)}54=4{chr(1)}60=20210201-10:00:000{chr(1)}10=190{chr(1)}"
ORDER_CANCEL = f"8=FIX.4.2{chr(1)}9=93{chr(1)}35=F{chr(1)}49=1{chr(1)}56=0{chr(1)}34=5{chr(1)}52=20240116-01:27:45.571{chr(1)}11=2{chr(1)}41=2{chr(1)}55=EUR/USD{chr(1)}54=4{chr(1)}60=20210201-10:00:000{chr(1)}10=160{chr(1)}"
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=45{chr(1)}35=5{chr(1)}49=1{chr(1)}56=0{chr(1)}34=6{chr(1)}52=20240116-01:27:45.571{chr(1)}10=062{chr(1)}"

msg_list = [
    {"msg": LOGON_MSG, "name": "logon"},
    {"msg": NEW_ORDER_SINGLE_1, "name": "new_order1", "bid_state": {"orders": 1, "price": 100, "quantity": 10}, "ask_state": {"orders": 0, "price": 0, "quantity": 0}},
    {"msg": NEW_ORDER_SINGLE_2, "name": "new_order2", "bid_state": {"orders": 1, "price": 100, "quantity": 10}, "ask_state": {"orders": 1, "price": 200, "quantity": 15}},
    {"msg": ORDER_CANCEL_REPLACE, "name": "order_cancel_replace", "bid_state": {"orders": 1, "price": 150, "quantity": 25}, "ask_state": {"orders": 1, "price": 200, "quantity": 15}},
    {"msg": ORDER_CANCEL, "name": "order_cancel", "bid_state": {"orders": 1, "price": 150, "quantity": 25}, "ask_state": {"orders": 0, "price": 0, "quantity": 0}},
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
    if "bid_state" in _msg:
        print("=" * 20)
        print("Market state should be:\n")
        print("bid_state == ", _msg["bid_state"])
        print("ask_state == ", _msg["ask_state"])
        print("=" * 20, "\n\n")

socket.close()
