import serial
import threading

class SerialCommunication:
    def __init__(self, port, baudrate):
        self.port = port
        self.baudrate = baudrate
        self.serial = serial.Serial(port, baudrate)
        self.rx_thread = threading.Thread(target=self.receive_data, daemon=True)
        self.rx_thread.start()

    def receive_data(self):
        while True:
            if self.serial.in_waiting:
                data = self.serial.readline().decode().strip()
                print("Received:", data)

    def send_data(self, data):
        self.serial.write(data.encode())

    def close(self):
        self.serial.close()

if __name__ == "__main__":
    port_name = "COM7"
    baud_rate = 115200

    serial_comm = SerialCommunication(port_name, baud_rate)

    try:
        while True:
            message = input("Enter message to send (type 'exit' to quit): ")
            if message.lower() == 'exit':
                break
            print("Sending:", message)
            serial_comm.send_data(message + "\r\n")

    except KeyboardInterrupt:
        pass

    finally:
        serial_comm.close()