import socket
import threading
import time
import argparse

# UDP Server
def udp_server(host="0.0.0.0", port=12345):
    socket.bind((host, port))


    last_count=None
    while True:
        data, addr = socket.recvfrom(1024)  # Buffer size is 1024 bytes
        count=int.from_bytes(data)

        if last_count is not None and count!=last_count+1:
            print(f"{addr[0]}:{addr[1]} #{count} ERROR")
        else:
            print(f"{addr[0]}:{addr[1]} #{count}")

        last_count=count

# UDP Client
def udp_client(target_host="127.0.0.1", target_port=12345):


    count=0
    while True:
        count=count+1
        socket.sendto(count.to_bytes(), (target_host, target_port))
        # print(f"Sent message: {count} to {target_host}:{target_port}")
        time.sleep(1)  # Send a message every second


parser = argparse.ArgumentParser(description="Simple UDP Server and Client")
parser.add_argument('local_port', type=int,  help="Local source port to use")
parser.add_argument('host', type=str,  help="Host to send udp packet to")
parser.add_argument('remote_port', type=int,  help="Remote port to send packets to")
# parser.add_argument('--port', type=int, default=12345, help="Port to bind the server or target the client (default: 12345)")
# parser.add_argument('--mode', type=str, choices=['server', 'client'], required=True, help="Run as 'server' or 'client'")
# parser.add_argument('--message', type=str, default="Hello, UDP Server!", help="Message to send (client mode only)")
# parser.add_argument('--interval', type=int, default=1, help="Interval in seconds between sending messages (client mode only)")


args=parser.parse_args()
socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

print(f"Sending from {args.local_port} to { args.host}:{args.remote_port}")

# Run the server in a separate thread
server_thread = threading.Thread(target=udp_server, args=("0.0.0.0", args.local_port))
server_thread.daemon = True
server_thread.start()

time.sleep(1)
# Run the client
udp_client(args.host, args.remote_port)


