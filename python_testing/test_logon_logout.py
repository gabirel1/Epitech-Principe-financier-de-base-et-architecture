import socket

PORT = 8080
HOST = "localhost"

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=67{chr(1)}35=A{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=1{chr(1)}52=20240112-15:41:22.616{chr(1)}98=0{chr(1)}108=30{chr(1)}10=143{chr(1)}"
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=55{chr(1)}35=5{chr(1)}49=CLIENT{chr(1)}56=SERVER{chr(1)}34=2{chr(1)}52=20240112-15:41:22.616{chr(1)}10=104{chr(1)}"

msg_list = [
    {"msg": LOGON_MSG, "name": "logon"},
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