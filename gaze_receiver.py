import msgpack
from time import sleep
from typing import Any, cast, Dict
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
subscriber.subscribe('gaze.')

while True:
    topic_bytes, payload = subscriber.recv_multipart()
    topic = topic_bytes.decode()
    message = cast(Dict[str, Any] ,msgpack.loads(payload))
    if topic == 'gaze.3d.01.':
        show = ['confidence', 'norm_pos', 'gaze_normals_3d', 'gaze_point_3d']
        for key in show:
            print(f"{key:>16} {message[key]}")
        print()

    elif topic.startswith('custom'):
        print(f"{topic}: {message}\n")
