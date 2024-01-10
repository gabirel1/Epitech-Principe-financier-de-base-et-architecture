import socket
import signal

PORT = 8080
HOST = "localhost"

NEW_ORDER_CHECKSUM = f"10=004{chr(1)}"

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=67{chr(1)}35=A{chr(1)}49=CLIENT{chr(1)}56=TARGET{chr(1)}34=1{chr(1)}52=20240110-13:43:05.567{chr(1)}98=0{chr(1)}108=30{chr(1)}10=131{chr(1)}"
NEW_ORDER_MSG = f"8=FIX.4.2{chr(1)}9=125{chr(1)}35=D{chr(1)}49=CLIENT{chr(1)}56=TARGET{chr(1)}34=2{chr(1)}52=20240110-13:43:05.567{chr(1)}11=1{chr(1)}21=3{chr(1)}38=100{chr(1)}40=2{chr(1)}44=100{chr(1)}54=355=EUR/USD{chr(1)}60=20200101-00:00:00.000{chr(1)}10=181{chr(1)}"
# ORDER_CANCEL_REQUEST_MSG
# ORDER_CANCEL_REPLACE_REQUEST_MSG
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=55{chr(1)}35=549=CLIENT{chr(1)}56=TARGET{chr(1)}34=3{chr(1)}52=20240110-13:43:05.567{chr(1)}10=093{chr(1)}"

def signal_handler(signal, frame):
    socket.close()
    exit(0)

signal.signal(signal.SIGINT, signal_handler)
socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
socket.connect((HOST, PORT))
print(f"Connected to server at {HOST}:{PORT}")

while True:
    message = input("Enter message to send: ")
    if (message == "logon"):
        print("sending logon message: ", LOGON_MSG, "\n");
        socket.sendall(LOGON_MSG.encode())
        print("sent logon message")
    elif (message == "new_order"):
        print("sending new order message: ", NEW_ORDER_MSG, "\n")
        socket.sendall(NEW_ORDER_MSG.encode())
    elif (message == "logout"):
        print("sending logout message: ", LOGOUT_MSG, "\n")
        socket.sendall(LOGOUT_MSG.encode())
    else:
        continue
    data = socket.recv(1024).decode()
    print(f"Received message from server: {data}")

