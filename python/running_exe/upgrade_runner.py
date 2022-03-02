import os
import subprocess
import time

# environment
program_path = "PracticeCsharp.exe"
current_dir = os.getcwd()

def run_upgrade(repeat_count, delay_between):
    print("repeat count =", repeat_count, "[count]")
    print("delay between=", delay_between, "[sec]")
    print("\n")

    count = range(repeat_count)
    for i in count :
        print("running the program: ", i,"/", repeat_count)
        p = subprocess.Popen(program_path,cwd=current_dir)
        return_code = p.wait()

        if return_code == 0 :
            time.sleep(delay_between)
        else:
            print("return code=", return_code, ". Running halted")
            break
        
    return 0
