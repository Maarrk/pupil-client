import argparse
import msgpack
from threading import Event, Thread
from time import sleep
import zmq


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', type=int,
                        help='TCP port to serve Pupil Remote on', default=50020)
    parser.add_argument('-s', '--sub-port', type=int,
                        help='TCP port to publish Pupil IPC Backbone on', default=50030)
    args = parser.parse_args()

    context = zmq.Context()

    should_stop = Event()

    remote = Thread(target=serve_remote, args=(
        context, args.port, should_stop, args.sub_port))
    remote.start()

    backbone = Thread(target=serve_backbone, args=(
        context, args.sub_port, should_stop))
    backbone.start()

    while not should_stop.is_set():
        try:
            sleep(0.1)
        except KeyboardInterrupt:
            should_stop.set()

    remote.join()
    backbone.join()


def serve_remote(context: zmq.Context, port: int, should_stop: Event, sub_port: int):
    socket = context.socket(zmq.REP)
    socket.setsockopt(zmq.RCVTIMEO, 1000)
    socket.bind(f'tcp://*:{port}')

    while not should_stop.is_set():
        try:
            command = socket.recv_string()
            print(f'req: {command}')
            reply = handle_command(command, sub_port)
            print(f'rep: {reply}')
            socket.send_string(reply)

        except zmq.error.Again:
            pass


def serve_backbone(context: zmq.Context, port: int, should_stop: Event, interval=0.5):
    pub_socket = context.socket(zmq.PUB)
    pub_socket.bind(f'tcp://*:{port}')

    payload = {'hello': 'world'}

    while not should_stop.is_set():
        pub_socket.send_string("custom.hello", flags=zmq.SNDMORE)
        pub_socket.send(msgpack.dumps(payload, use_bin_type=True))
        sleep(interval)


def handle_command(command: str, sub_port: int) -> str:
    param = command[2:]
    param_float = None
    try:
        param_float = float(param)
    except ValueError:
        pass

    if command == 'R':
        return 'start recording with auto generated session name'
    elif command.startswith('R '):
        return f'start recording named "{param}"'
    elif command == 'r':
        return 'stop recording'
    elif command == 'C':
        return 'start currently selected calibration'
    elif command == 'c':
        return 'stop currently selected calibration'
    elif command.startswith('T ') and param_float is not None:
        return f'resets current Pupil time to {param_float}'
    elif command == 't':
        # get current Pupil time; returns a float as string
        return str(42.0)
    elif command == 'v':
        return 'get the Pupil Core software version string'
    elif command == 'SUB_PORT':
        return str(sub_port)
    else:
        return f'could not handle {command}'


if __name__ == '__main__':
    main()
