import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# 建立连接:
s.connect(('127.0.0.1', 9999))
# 接收欢迎消息:
print (s.recv(1024).decode())
while True:
    name = input('enter your name(quit to exit):')
    if name=='quit':
        break
    if len(name)==0 :
        print('你输入了空字符')
        continue
    # 发送数据:
    s.send(name.encode())
    print(s.recv(1024).decode())
s.send(b'exit')
s.close()
