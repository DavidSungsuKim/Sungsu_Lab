"""
내가 생각하는 최선의 예제. 크기조정이나 다른 버튼 추가작업은 별도로 해야함.
- list box
- 다중선택
- 스크롤바
"""
import tkinter as tk

def on_select(event):  
    # 이부분 안동작함.
    # ctrl_pressed = (event.state & 0x4) != 0
    # if ctrl_pressed:
    #     print("Ctrl key pressed")
            
    # 선택된 항목의 인덱스 가져오기
    selected_indices = listbox.curselection()
    selected_items = [listbox.get(index) for index in selected_indices]
    # 선택된 항목 출력
    print("Selected items:", selected_items)

# Tkinter 윈도우 생성
root = tk.Tk()

# Listbox 생성 (다중 선택 활성화)
listbox = tk.Listbox(root, selectmode=tk.MULTIPLE)
listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# 스크롤바 생성
scrollbar = tk.Scrollbar(root, orient=tk.VERTICAL)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

# Listbox과 스크롤바 연결
listbox.config(yscrollcommand=scrollbar.set)
scrollbar.config(command=listbox.yview)

# Listbox에 항목 추가
for i in range(100):
    listbox.insert(tk.END, f"Item {i}")

# Listbox에 선택 변경 이벤트 바인딩
listbox.bind("<<ListboxSelect>>", on_select)

# Tkinter 이벤트 루프 시작
root.mainloop()


# import tkinter as tk

# def on_select(event):
#     selected_indices = listbox.curselection()
#     selected_items = [listbox.get(index) for index in selected_indices]
#     print("Selected items:", selected_items)

# # Tkinter 윈도우 생성
# root = tk.Tk()

# # Listbox 생성 (다중 선택 활성화)
# listbox = tk.Listbox(root, selectmode=tk.MULTIPLE)
# listbox.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# # 스크롤바 생성
# scrollbar = tk.Scrollbar(root, orient=tk.VERTICAL)
# scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

# # Listbox과 스크롤바 연결
# listbox.config(yscrollcommand=scrollbar.set)
# scrollbar.config(command=listbox.yview)

# # Listbox에 항목 추가
# for i in range(100):
#     listbox.insert(tk.END, f"Item {i}")

# # Listbox에 선택 변경 이벤트 바인딩
# listbox.bind("<Button-1>", on_select)

# # Tkinter 이벤트 루프 시작
# root.mainloop()


"""
컨트롤 키가 눌렸는지 확인하는 코드... 내가 원하는 동작방식은 아님.
"""
# import tkinter as tk

# def on_select(event):
#     # 사용자가 컨트롤 키를 눌렀는지 확인
#     ctrl_pressed = (event.state & 0x4) != 0
#     if ctrl_pressed:
#         print("Ctrl key pressed")

#     # selected_indices = listbox.curselection()
#     # selected_items = [listbox.get(index) for index in selected_indices]
#     # # 선택된 항목 출력
#     # print("Selected items:", selected_items)

# # Tkinter 윈도우 생성
# root = tk.Tk()

# # Listbox 생성 (다중 선택 활성화)
# listbox = tk.Listbox(root, selectmode=tk.MULTIPLE)
# listbox.pack()

# # Listbox에 항목 추가
# for i in range(10):
#     listbox.insert(tk.END, f"Item {i}")

# # Listbox에 선택 변경 이벤트 바인딩
# listbox.bind("<Button-1>", on_select)

# # Tkinter 이벤트 루프 시작
# root.mainloop()




# import tkinter

# window=tkinter.Tk()
# window.title("-")
# window.geometry("640x480+100+100")
# window.resizable(False, False)

# listbox = tkinter.Listbox(window, selectmode='extended', height=0)

# for i in range(1,100):
#     string = str(i) + "번"
#     listbox.insert(0, string)
# # listbox.insert(1, "2번")
# # listbox.insert(2, "2번")
# # listbox.insert(3, "2번")
# # listbox.insert(4, "3번")
# listbox.delete(1, 2)
# listbox.pack()

# window.mainloop()





# import tkinter

# window=tkinter.Tk()
# window.title("YUN DAE HEE")
# window.geometry("640x400+100+100")
# window.resizable(False, False)

# frame=tkinter.Frame(window)

# scrollbar=tkinter.Scrollbar(frame)
# scrollbar.pack(side="right", fill="y")

# listbox=tkinter.Listbox(frame, yscrollcommand = scrollbar.set)
# for line in range(1,1001):
#    listbox.insert(line, str(line) + "/1000")
# listbox.pack(side="left")

# scrollbar["command"]=listbox.yview

# frame.pack()

# window.mainloop()