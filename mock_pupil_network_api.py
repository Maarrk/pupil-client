import argparse
from email import message
import zmq


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('-p', '--port', type=int,
                        help='TCP port to serve Pupil Remote on', default=50020)
    args = parser.parse_args()

    context = zmq.Context()

    serve_remote(context, args.port)


def serve_remote(context: zmq.Context, port: int):
    socket = context.socket(zmq.REP)
    socket.setsockopt(zmq.RCVTIMEO, 1000)
    socket.bind(f'tcp://*:{port}')

    should_stop = False

    while not should_stop:
        try:
            command = socket.recv_string()
            print(f'req: {command}')
            reply = handle_command(command)
            print(f'rep: {reply}')
            socket.send_string(reply)

        except zmq.error.Again:
            pass
        except KeyboardInterrupt:
            should_stop = True


def handle_command(command: str) -> str:
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
    else:
        return f'could not handle {command}'


if __name__ == '__main__':
    main()
