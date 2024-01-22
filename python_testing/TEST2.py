import socket

HOST = "localhost"
PORT = 8080

LOGON_MSG = f"8=FIX.4.2{chr(1)}9=70{chr(1)}35=A{chr(1)}49=CLIENT1{chr(1)}56=MyMarket{chr(1)}34=1{chr(1)}52=20240121-23:15:30.218{chr(1)}98=0{chr(1)}108=30{chr(1)}10=010{chr(1)}"
NEW_ORDER_SINGLE_MSG = f"8=FIX.4.2{chr(1)}9=118{chr(1)}35=D{chr(1)}49=CLIENT1{chr(1)}56=MyMarket{chr(1)}34=2{chr(1)}52=20240121-23:15:30.218{chr(1)}11=1{chr(1)}55=GOLD/EURO{chr(1)}54=3{chr(1)}38=100{chr(1)}40=2{chr(1)}44=1000{chr(1)}21=3{chr(1)}60=20201010-10:10:100{chr(1)}10=115{chr(1)}"
NEW_ORDER_SINGLE_MSG2 = f"8=FIX.4.2{chr(1)}9=128{chr(1)}35=D{chr(1)}49=CLIENT1{chr(1)}56=MyMarket{chr(1)}34=2{chr(1)}52=20240121-23:15:30.218{chr(1)}11=1{chr(1)}55=GOLD/EURO{chr(1)}54=3{chr(1)}38=100{chr(1)}40=2{chr(1)}44=1000{chr(1)}21=3{chr(1)}60=20201010-10:10:100{chr(1)}10=105{chr(1)}"
NEW_ORDER_SINGLE_MSG_3 = f"8=FIX.4.2{chr(1)}9=123{chr(1)}35=D{chr(1)}49=Client1{chr(1)}56=MyMarket{chr(1)}34=2{chr(1)}52=20240122-12:01:31.223{chr(1)}11=1{chr(1)}21=3{chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=GOLD/EURO{chr(1)}60=20201010-10:10:10.000{chr(1)}10=041{chr(1)}"
NEW_ORDER_SINGLE_MSG_4 = f"8=FIX.4.2{chr(1)}9=127{chr(1)}35=D{chr(1)}49=Client1{chr(1)}56=MyMarket{chr(1)}34=2{chr(1)}52=20240122-12:01:31.223{chr(1)}11=1{chr(1)}21=3{chr(1)}38={chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=GOLD/EURO{chr(1)}60=20201010-10:10:10.000{chr(1)}10=214{chr(1)}"
NEW_ORDER_SINGLE_MSG_5 = f"8=FIX.4.2{chr(1)}9=128{chr(1)}35=D{chr(1)}49=Client1{chr(1)}56=MyMarket{chr(1)}34=2{chr(1)}52=20240122-12:01:31.223{chr(1)}11=1{chr(1)}21=3{chr(1)}38= {chr(1)}40=2{chr(1)}44=100{chr(1)}54=3{chr(1)}55=GOLD/EURO{chr(1)}60=20201010-10:10:10.000{chr(1)}10=247{chr(1)}"
LOGOUT_MSG = f"8=FIX.4.2{chr(1)}9=58{chr(1)}35=5{chr(1)}49=CLIENT1{chr(1)}56=MyMarket{chr(1)}34=3{chr(1)}52=20240121-23:15:30.218{chr(1)}10=237{chr(1)}"

def decode_message(msg: str) -> dict:
    """Decode a FIX message into a dictionary."""
    msg_dict = {}
    msg_list = msg.split(chr(1))
    for item in msg_list:
        if "=" in item:
            key, value = item.split("=")
            msg_dict[key] = value
    return msg_dict

try:
    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((HOST, PORT))

    print("[TEST_1] sending message: logon\n")
    socket.sendall(LOGON_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_1] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "A", "Logon message failed (Didn't receive a Logon)."
    assert response["49"] == "MyMarket", "Logon message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logon message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "1", "Logon message failed. Expected MsgSeqNum(34) = 1."
    assert response["108"] == "30", "Logon message failed. Expected HeartBtInt = 30."
    print("[TEST_1] Logon message passed.", "\n\n")

    print("[TEST_1] sending message: new_order_single\n")
    socket.sendall(NEW_ORDER_SINGLE_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_1] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "3", "New Order Single message failed (Didn't receive an Execution Report)."
    assert response["49"] == "MyMarket", "New Order Single message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "New Order Single message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "2", "New Order Single message failed. Expected MsgSeqNum(34) = 2."
    # assert response["45"] == "2", "New Order Single message failed. Expected RefSeqNum(45) = 2."
    assert response["371"] == "9", "New Order Single message failed. Expected RefTagID(371) = 9."
    # assert response["372"] == "D", "New Order Single message failed. Expected RefMsgType(372) = D."
    # assert response["35"] == "8", "New Order Single message failed (Didn't receive an Execution Report)."

    print("[TEST_1] New Order Single message passed.", "\n\n")

    print("[TEST_1] sending message: logout\n")
    socket.sendall(LOGOUT_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_1] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "5", "Logout message failed (Didn't receive a Logout)."
    assert response["49"] == "MyMarket", "Logout message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logout message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "3", "Logout message failed. Expected MsgSeqNum(34) = 3."

    print("[TEST_1] Logout message passed.", "\n\n")
    socket.close()

    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((HOST, PORT))

    print("[TEST_2] sending message: logon\n")
    socket.sendall(LOGON_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_2] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "A", "Logon message failed (Didn't receive a Logon)."
    assert response["49"] == "MyMarket", "Logon message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logon message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "1", "Logon message failed. Expected MsgSeqNum(34) = 1."
    assert response["108"] == "30", "Logon message failed. Expected HeartBtInt = 30."
    print("[TEST_2] Logon message passed.", "\n\n")

    print("[TEST_2] sending message: new_order_single\n")
    socket.sendall(NEW_ORDER_SINGLE_MSG2.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_2] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "3", "New Order Single message failed (Didn't receive an Execution Report)."
    assert response["49"] == "MyMarket", "New Order Single message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "New Order Single message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "2", "New Order Single message failed. Expected MsgSeqNum(34) = 2."
    # assert response["45"] == "2", "New Order Single message failed. Expected RefSeqNum(45) = 2."
    assert response["371"] == "10", "New Order Single message failed. Expected RefTagID(371) = 10."
    # assert response["372"] == "D", "New Order Single message failed. Expected RefMsgType(372) = D."

    print("[TEST_2] New Order Single message passed.", "\n\n")

    print("[TEST_2] sending message: logout\n")
    socket.sendall(LOGOUT_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_2] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "5", "Logout message failed (Didn't receive a Logout)."
    assert response["49"] == "MyMarket", "Logout message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logout message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "3", "Logout message failed. Expected MsgSeqNum(34) = 3."

    print("[TEST_2] Logout message passed.", "\n\n")
    socket.close()

    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((HOST, PORT))

    print("[TEST_3] sending message: logon\n")
    socket.sendall(LOGON_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_3] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "A", "Logon message failed (Didn't receive a Logon)."
    assert response["49"] == "MyMarket", "Logon message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logon message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "1", "Logon message failed. Expected MsgSeqNum(34) = 1."
    assert response["108"] == "30", "Logon message failed. Expected HeartBtInt = 30."
    print("[TEST_3] Logon message passed.", "\n\n")

    print("[TEST_3] sending message: new_order_single\n")
    socket.sendall(NEW_ORDER_SINGLE_MSG_3.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_3] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "3", "New Order Single message failed (Didn't receive an Execution Report)."
    assert response["49"] == "MyMarket", "New Order Single message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "New Order Single message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "2", "New Order Single message failed. Expected MsgSeqNum(34) = 2."
    assert response["371"] == "38", "New Order Single message failed. Expected RefTagID(371) = 38."

    print("[TEST_3] New Order Single message passed.", "\n\n")

    print("[TEST_3] sending message: logout\n")
    socket.sendall(LOGOUT_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_3] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "5", "Logout message failed (Didn't receive a Logout)."
    assert response["49"] == "MyMarket", "Logout message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logout message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "3", "Logout message failed. Expected MsgSeqNum(34) = 3."

    print("[TEST_3] Logout message passed.", "\n\n")
    socket.close()

    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((HOST, PORT))

    print("[TEST_4] sending message: logon\n")
    socket.sendall(LOGON_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_4] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "A", "Logon message failed (Didn't receive a Logon)."
    assert response["49"] == "MyMarket", "Logon message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logon message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "1", "Logon message failed. Expected MsgSeqNum(34) = 1."
    assert response["108"] == "30", "Logon message failed. Expected HeartBtInt = 30."
    print("[TEST_4] Logon message passed.", "\n\n")

    print("[TEST_4] sending message: new_order_single\n")
    socket.sendall(NEW_ORDER_SINGLE_MSG_4.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_4] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "3", "New Order Single message failed (Didn't receive an Execution Report)."
    assert response["49"] == "MyMarket", "New Order Single message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "New Order Single message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "2", "New Order Single message failed. Expected MsgSeqNum(34) = 2."
    assert response["371"] == "38", "New Order Single message failed. Expected RefTagID(371) = 38."

    print("[TEST_4] New Order Single message passed.", "\n\n")

    print("[TEST_4] sending message: logout\n")
    socket.sendall(LOGOUT_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_4] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "5", "Logout message failed (Didn't receive a Logout)."
    assert response["49"] == "MyMarket", "Logout message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logout message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "3", "Logout message failed. Expected MsgSeqNum(34) = 3."

    print("[TEST_4] Logout message passed.", "\n\n")
    socket.close()

    socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    socket.connect((HOST, PORT))

    print("[TEST_5] sending message: logon\n")
    socket.sendall(LOGON_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_5] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "A", "Logon message failed (Didn't receive a Logon)."
    assert response["49"] == "MyMarket", "Logon message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logon message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "1", "Logon message failed. Expected MsgSeqNum(34) = 1."
    assert response["108"] == "30", "Logon message failed. Expected HeartBtInt = 30."
    print("[TEST_5] Logon message passed.", "\n\n")

    print("[TEST_5] sending message: new_order_single\n")
    socket.sendall(NEW_ORDER_SINGLE_MSG_5.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_5] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "3", "New Order Single message failed (Didn't receive an Execution Report)."
    assert response["49"] == "MyMarket", "New Order Single message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "New Order Single message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "2", "New Order Single message failed. Expected MsgSeqNum(34) = 2."
    assert response["371"] == "38", "New Order Single message failed. Expected RefTagID(371) = 38."

    print("[TEST_5] New Order Single message passed.", "\n\n")

    print("[TEST_5] sending message: logout\n")
    socket.sendall(LOGOUT_MSG.encode())
    data = socket.recv(1024).decode()
    print(f"[TEST_5] Received message from server: '{data}'", "\n\n")
    response = decode_message(data)

    assert response["35"] == "5", "Logout message failed (Didn't receive a Logout)."
    assert response["49"] == "MyMarket", "Logout message failed. Expected senderCompID(49) = MyMarket."
    assert response["56"] == "CLIENT1", "Logout message failed. Expected targetCompID(56) = CLIENT1."
    assert response["34"] == "3", "Logout message failed. Expected MsgSeqNum(34) = 3."

    print("[TEST_5] Logout message passed.", "\n\n")
    socket.close()


except Exception as e:
    print(e)
    socket.close()