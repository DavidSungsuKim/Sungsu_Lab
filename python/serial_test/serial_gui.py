import tkinter as tk
from tkinter import scrolledtext
from tkinter import filedialog
import threading
import serial
import time

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

    def send_data(self, data):
        self.serial.write(data.encode())

    def close(self):
        self.serial.close()

class Application(tk.Frame):
    def __init__(self, master=None, my_font=("Consolas", 10)):
        super().__init__(master)
        self.master = master
        self.my_font = my_font
        self.pack()
        self.create_widgets()
        
    def create_widgets(self):
        # Input frame 1
        self.input_frame1 = tk.Frame(self)
        self.input_frame1.pack(fill=tk.X)
        self.input_field1 = tk.Entry(self.input_frame1, font=self.my_font)
        self.input_field1.pack(side="left", padx=5, pady=5, fill=tk.X, expand=True)
        self.send_button1 = tk.Button(self.input_frame1, text="Send", command=self.send_data1)
        self.send_button1.pack(side="left", padx=5, pady=5)

        # Input frame 2
        self.input_frame2 = tk.Frame(self)
        self.input_frame2.pack(fill=tk.X)
        self.input_field2 = tk.Entry(self.input_frame2, font=self.my_font)
        self.input_field2.pack(side="left", padx=5, pady=5, fill=tk.X, expand=True)
        self.send_button2 = tk.Button(self.input_frame2, text="Send", command=self.send_data2)
        self.send_button2.pack(side="left", padx=5, pady=5)

        # Text output field
        self.output_field = scrolledtext.ScrolledText(self, wrap=tk.WORD, bg="black", fg="white", font=self.my_font)
        self.output_field.pack(side="top", fill=tk.BOTH, expand=True, padx=5, pady=5)

        # Control buttons (Clear and Save)
        self.button_frame = tk.Frame(self)
        self.button_frame.pack(fill=tk.X)
        self.clear_button = tk.Button(self.button_frame, text="Clear", command=self.clear_data)
        self.clear_button.pack(side="left", padx=5, pady=5)
        self.save_button = tk.Button(self.button_frame, text="Save", command=self.save_data)
        self.save_button.pack(side="left", padx=5, pady=5)

        # Filtering field and button
        self.filtering_frame = tk.Frame(self)
        self.filtering_frame.pack(fill=tk.X)
        self.filtering_field = tk.Entry(self.filtering_frame, font=self.my_font)
        self.filtering_field.pack(side="left", padx=5, pady=5, fill=tk.X, expand=True)
        self.filter_button = tk.Button(self.filtering_frame, text="Filter", command=self.filter_data)
        self.filter_button.pack(side="left", padx=5, pady=5)

        # Quit button
        self.quit = tk.Button(self, text="QUIT", fg="red", command=self.master.destroy)
        self.quit.pack(side="right", padx=5, pady=5)

    def send_data1(self):
        data = self.input_field1.get()
        serial_comm.send_data(data + "\r\n")

    def send_data2(self):
        data = self.input_field2.get()
        serial_comm.send_data(data + "\r\n")

    def display_data(self, data):
        self.output_field.insert(tk.END, data + '\n')

    def clear_data(self):
        self.output_field.delete(1.0, tk.END)

    def save_data(self):
        file_name = filedialog.asksaveasfilename(defaultextension=".txt")
        if file_name:
            selected_text = self.output_field.get(tk.SEL_FIRST, tk.SEL_LAST)  # Get the selected text
            with open(file_name, "w") as file:
                file.write(selected_text)

    def filter_data(self):
        filtering_text = self.filtering_field.get()
        filtered_text = ""
        full_text = self.output_field.get("1.0", tk.END)
        lines = full_text.split("\n")
        for line in lines:
            if filtering_text in line:
                filtered_text += line + "\n"
        self.clear_data()
        self.output_field.insert(tk.END, filtered_text)

if __name__ == "__main__":
    port_name = "COM7"
    baud_rate = 115200

    serial_comm = SerialCommunication(port_name, baud_rate)

    root = tk.Tk()
    app = Application(master=root)
    app.mainloop()
