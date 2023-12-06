from time import sleep
import zmq

ctx = zmq.Context()
pupil_remote = zmq.Socket(ctx, zmq.REQ)
pupil_remote.connect('tcp://127.0.0.1:50020')

# start recording
pupil_remote.send_string('R')
print(pupil_remote.recv_string())

sleep(5)
pupil_remote.send_string('r')
print(pupil_remote.recv_string())
