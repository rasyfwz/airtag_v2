import serial
import numpy as np

def processBuffer(buffer, x, y, z, bias):

    raw_value_x = np.int16(np.uint16((buffer[0] << 8) | buffer[1]))
    raw_value_y = np.int16(np.uint16((buffer[2] << 8) | buffer[3]))
    raw_value_z = np.int16(np.uint16((buffer[4] << 8) | buffer[5]))

    x.append(float(raw_value_x / bias))
    y.append(float(raw_value_y / bias))
    z.append(float(raw_value_z / bias))

def 




if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 115200)

    if ser.is_open:
        print("Port is open")
        ser.close()
    
    ser.open()
    
    data_buffer = bytearray()
    x_accel = []
    y_accel = []
    z_accel = []

    x_gyro = []
    y_gyro = []
    z_gyro = []
    

    preamble = bytearray(b'\xaa\x55')
    # print(ser.out_waiting)
    while True:
        read = ser.read()
        data_buffer.extend(read)
        
        try:
            start = data_buffer.index(b'\xAA\x55')
            end  = data_buffer.index(b'\xAA\x55', start + 2)
            accel_buf = data_buffer[start + 2 : start + 8]
            gyro_buf = data_buffer[start + 8 : start + 14]

            processBuffer(accel_buf, x_accel, y_accel, z_accel, 16384)

            processBuffer(accel_buf, x_gyro, y_gyro, z_gyro, 28571)

            data_buffer = data_buffer[end:]

            print(f'Acceleration (x, y, z): {x_accel[-1]}, {y_accel[-1]} {z_accel[-1]}')
            print(f'Angular Velocity (x, y, z): {x_gyro[-1]}, {y_gyro[-1]} {z_gyro[-1]}')


        except ValueError:
            continue







