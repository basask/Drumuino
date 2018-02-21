import serial
import time
import datetime

# This programs intend to stream serial input to an file in a comma separated file forma.
# To use it just run the Drumuino in an arduino e this program in your computer.
# You'll problably need to change some configurations like port, baudrate and filespaths

def run(port, baudrate, filepath):
    ser = serial.Serial(port, baudrate)
    ts = time.time()
    with open(filepath, "w+") as file_descriptor:
        while True:
            line = ser.readline()
            line = line.decode("utf-8")
            th = time.time()
            # st = datetime.datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
            st = th - ts
            writeline = '%s;%f\n' % (line.rstrip(), st)
            print(writeline)
            file_descriptor.write(writeline)

if __name__ == '__main__':
    port = '/dev/ttyACM0'
    baudrate = 115200
    filepath = 'serial.txt'
    run(port, baudrate, filepath)
