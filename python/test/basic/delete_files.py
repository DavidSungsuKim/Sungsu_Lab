def delete_files_with_extension(directory_path, extension):
    try:
        for filename in os.listdir(directory_path):
            file_path = os.path.join(directory_path, filename)
            if os.path.isfile(file_path) and file_path.endswith(extension):
                os.remove(file_path)
                print(f"Deleted file: {file_path}")
            else:
                print(f"Skipped file: {file_path}")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    current_directory = os.getcwd()
    file_extension = ".txt"  # 삭제할 파일 확장명
    delete_files_with_extension(current_directory, file_extension)