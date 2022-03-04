import time

def get_time_diff_sec(time_old):
    time_diff = ( time.time_ns() - time_old ) / 1000000000
    return time_diff

time_started = time.time_ns()
time.sleep(1)
time_elapsed = get_time_diff_sec(time_started)

print("time_elapsed=", time_elapsed)

