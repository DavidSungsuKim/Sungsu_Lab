import sys

def myfunc(args):
    num_of_args = len(args)
    print("number of arguments=", num_of_args)
    print(args)

    for arg in args:
        print(arg)
  
myfunc(sys.argv)
