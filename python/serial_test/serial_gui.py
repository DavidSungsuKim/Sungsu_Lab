import tkinter as tk
from tkinter import scrolledtext
from tkinter import filedialog
import threading
import serial

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
                app.display_data(data)  # Display received data in the GUI
                app.save_data_to_file(data)  # Save received data to file

    def send_data(self, data):
        self.serial.write(data.encode())

    def close(self):
        self.serial.close()

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.master = master
        self.pack()
        self.create_widgets()

    def create_widgets(self):
        self.input_field = tk.Entry(self)
        self.input_field.pack(side="top")

        self.send_button = tk.Button(self)
        self.send_button["text"] = "Send"
        self.send_button["command"] = self.send_data
        self.send_button.pack(side="top")

        self.clear_button = tk.Button(self)
        self.clear_button["text"] = "Clear"
        self.clear_button["command"] = self.clear_data
        self.clear_button.pack(side="left")

        self.save_button = tk.Button(self)
        self.save_button["text"] = "Save"
        self.save_button["command"] = self.save_data
        self.save_button.pack(side="left")

        self.output_field = scrolledtext.ScrolledText(self, wrap=tk.WORD)
        self.output_field.pack(side="top", fill=tk.BOTH, expand=True)

        self.quit = tk.Button(self, text="QUIT", fg="red",
                              command=self.master.destroy)
        self.quit.pack(side="bottom")

        self.file_name = ""

    def send_data(self):
        data = self.input_field.get()
        
        print("Sending data:", data)
        serial_comm.send_data(data + "\r\n")

    def display_data(self, data):
        self.output_field.insert(tk.END, data + '\n')

    def clear_data(self):
        self.output_field.delete(1.0, tk.END)  # Clear the output field

    def save_data(self):
        if not self.file_name:
            self.file_name = filedialog.asksaveasfilename(defaultextension=".txt")
        
        with open(self.file_name, "a") as file:
            file.write(self.output_field.get(1.0, tk.END))

    def save_data_to_file(self, data):
        if self.file_name:
            with open(self.file_name, "a") as file:
                file.write(data + '\n')

if __name__ == "__main__":
    port_name = "COM7"
    baud_rate = 115200

    serial_comm = SerialCommunication(port_name, baud_rate)

    root = tk.Tk()
    app = Application(master=root)
    app.mainloop()