import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('127.0.0.1', 9999))
# 接收欢迎消息:
print (s.recv(1024).decode())
for data in ['Michael', 'Tracy', 'Sarah']:
    # 发送数据:
    s.send(data.encode())
    print(s.recv(1024).decode())
s.send(b'exit')
s.close()
