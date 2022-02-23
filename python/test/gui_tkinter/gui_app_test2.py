from tkinter import *
from tkinter.messagebox import *

window = Tk()
window.title("GUI app test")
window.geometry("400x200")

def callBackButton1():
    showinfo("callBackButton1","Hi")
    subWindow = Tk()
    subWindow.geometry("100x100")
    subWindow.mainloop()

button1 = Button(window, text="Button1", command=callBackButton1)
button1.place(x=10, y=10)

button2 = Button(window, text="Button2", command=callBackButton1)
button2.place(x=10, y=50)

window.mainloop()
