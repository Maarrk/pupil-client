import msgpack
from time import sleep
import zmq

ctx = zmq.Context()
pupil_remote = zmq.Socket(ctx, zmq.REQ)

ip = 'localhost'
port = 50020

pupil_remote.connect(f'tcp://{ip}:{port}')

# Request 'SUB_PORT' for reading data
pupil_remote.send_string('SUB_PORT')
sub_port = pupil_remote.recv_string()

subscriber = ctx.socket(zmq.SUB)
subscriber.connect(f'tcp://{ip}:{sub_port}')
subscriber.subscribe('custom.')

while True:
    topic, payload = subscriber.recv_multipart()
    message = msgpack.loads(payload)
    print(f"{topic}: {message}")
