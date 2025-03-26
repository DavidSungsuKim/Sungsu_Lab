import os
import platform

def open_current_directory():
    try:
        current_path = os.getcwd()
        if platform.system() == "Windows":
            os.startfile(current_path)
            print(f"Opened directory: {current_path}")
        else:
            print("This function is only supported on Windows.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    open_current_directory()