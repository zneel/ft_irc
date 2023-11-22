#!/usr/bin/python3
import socket
import random
import string
import time
import threading
import queue
import signal

# Shared channel list for threads to join and interact
shared_channels = queue.Queue()
MAX_CHANNELS = 5  # Max number of channels to create and join
stop_fuzzing = False  # Global flag to stop fuzzing

def random_string(length):
    """Generate a random string of fixed length."""
    return ''.join(random.choice(string.ascii_letters + string.digits + string.punctuation) for _ in range(length))

def send_command(s, command):
    """Send a command to the IRC server."""
    if command is not None:
        print(f"[{threading.current_thread().name}] Sending: {command.strip()}")
        s.sendall(command.encode())
    else:
        print(f"[{threading.current_thread().name}] Error: Command is None")

def fuzz_irc_server(host, port, timeout=0.01):
    """Fuzz an IRC server."""
    while not stop_fuzzing:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, port))
            my_nick = random_string(8)

            try:
                # Send initial valid commands for registration
                send_command(s, "CAP LS\r\n")
                send_command(s, f"PASS oui\r\n")
                send_command(s, f"NICK {my_nick}\r\n")
                send_command(s, f"USER {random_string(8)} 0 * :{random_string(12)}\r\n")

                # Join a channel or create a new one if the queue is empty
                try:
                    channel = shared_channels.get_nowait()
                except queue.Empty:
                    channel = f"#channel{random.randint(1, MAX_CHANNELS)}"
                    shared_channels.put(channel)

                send_command(s, f"JOIN {channel}\r\n")

                # Fuzz with random data and possibly disconnect and reconnect
                for _ in range(random.randint(5, 10)):  # Random number of commands before possible reconnect
                    fuzz_command = generate_fuzz_command(my_nick, channel)
                    send_command(s, fuzz_command)
                    time.sleep(timeout)

                # Randomly decide to send QUIT and reconnect
                if random.choice([True, False]):
                    send_command(s, f"QUIT :Fuzzing client {my_nick} signing off\r\n")
                    time.sleep(random.uniform(0.5, 2))  # Random delay before reconnect

            except Exception as e:
                print(f"[{threading.current_thread().name}] Error occurred: {e}")

def generate_fuzz_command(nick, channel):
    """Generate a fuzzed IRC command."""
    valid_commands = ["PART", "PRIVMSG", "PING", "AWAY", "MODE", "KICK"]
    command = random.choice(valid_commands)

    if command == "PRIVMSG":
        target = random.choice([nick, channel])  # Send message to self or the channel
        return f"{command} {target} :{random_string(10)}"
    elif command == "MODE":
        mode_changes = random_string(5)  # Example: "+iwx -s"
        return f"{command} {channel} {mode_changes}"
    elif command == "KICK":
        return f"{command} {channel} {nick} :{random_string(10)}"
    else:
        random_data = random_string(random.randint(1, 30))
        return f"{command} {random_data}"

    return None  # Return None explicitly if no command is generated

def start_fuzzing_clients(host, port, client_count=5):
    """Start multiple fuzzing clients."""
    threads = []
    for _ in range(client_count):
        thread = threading.Thread(target=fuzz_irc_server, args=(host, port))
        thread.start()
        threads.append(thread)

    try:
        for thread in threads:
            thread.join()
    except KeyboardInterrupt:
        global stop_fuzzing
        stop_fuzzing = True
        print("Stopping fuzzing clients...")

# Signal handler for graceful shutdown
def signal_handler(sig, frame):
    global stop_fuzzing
    stop_fuzzing = True

# Configuration
IRC_HOST = "localhost"  # Replace with your IRC server's address
IRC_PORT = 3000         # Replace with your IRC server's port
CLIENT_COUNT = 3       # Number of concurrent clients

# Register signal handler
signal.signal(signal.SIGINT, signal_handler)

# Start fuzzing with multiple clients
start_fuzzing_clients(IRC_HOST, IRC_PORT, CLIENT_COUNT)