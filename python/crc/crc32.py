import binascii
import os
import sys

def crc32_of_file(filepath):
   
   # Handle errors
   if not os.path.isfile(filepath):
      raise FileNotFoundError(f"No such file: '{filepath}'")
   if os.path.getsize(filepath) == 0:
      raise ValueError(f"File is empty: '{filepath}'")
   
   # Core logic : Calculate CRC32 checksum
   crc = 0
   with open(filepath, 'rb') as f:
      for chunk in iter(lambda: f.read(4096), b''):
         crc = binascii.crc32(chunk, crc)
   
   # Return only 32 bits of the result
   return crc & 0xFFFFFFFF
   
if __name__ == "__main__":
   # Check if the script is run with a file path argument
   if len(sys.argv) != 2:
      print(f"Usage: python {os.path.basename(__file__)} <file_path>")
      sys.exit(1)
   file_path = sys.argv[1]
   
   # Try to calculate the CRC32 checksum
   try:
      checksum = crc32_of_file(file_path)
      print(f"CRC32: {checksum:08x}")
   except Exception as e:
      print(f"Error: {e}")
      sys.exit(1)