import os
import re

def rename_excel_files(directory):
    """
    Renames all .xlsx files in the given directory.
    Removes '[' from the filename, replaces ']' with '_',
    and removes spaces after '_'.

    Args:
        directory (str): The directory path to search and rename files.
    """
    for filename in os.listdir(directory):
        if filename.endswith(".xlsx"):
            new_filename = re.sub(r'\[', '', filename)
            new_filename = re.sub(r'\]', '_', new_filename)
            new_filename = re.sub(r'_(\s+)', '_', new_filename)  # Remove spaces after '_'
            if new_filename != filename:
                old_path = os.path.join(directory, filename)
                new_path = os.path.join(directory, new_filename)
                os.rename(old_path, new_path)
                print(f"Renamed '{filename}' to '{new_filename}'.")

if __name__ == "__main__":
    print("1. Rename files in the current directory")
    print("2. Rename files in another directory")
    choice = input("Select an option (1 or 2): ")

    if choice == "1":
        target_directory = os.getcwd()
    elif choice == "2":
        target_directory = input("Enter the directory path to rename files: ")
    else:
        print("Invalid selection.")
        exit(1)

    if os.path.isdir(target_directory):
        rename_excel_files(target_directory)
        print("File renaming completed.")
    else:
        print("Invalid directory path.")