import customtkinter as ctk
from customtkinter import filedialog
import subprocess
import os

ctk.set_appearance_mode("System")
ctk.set_default_color_theme("blue")

path = ''


def get_path():
    global path
    path = filedialog.askopenfilename(filetypes=[("Image files", "*.jpg;*.png")])
    if path != '':
        status_label.configure(text=f"Выбран файл:\n {path}", text_color="white")

def run_exe():
    global path
    exe_file = "LookUpTheSkies.exe"
    args = [path]
    command = f"{exe_file} {' '.join(args)}"
    os.system(command)

    file = open("output_data.txt", "r").readline()
    stars_count = int(file)

    os.remove("output_data.txt")

    output_label.configure(text=f"Обнаружено {stars_count} звезд", text_color="green")

    button_open_result = ctk.CTkButton(window, text="Открыть результат", command=open_result_image)
    button_open_result.pack(pady=10)

def open_result_image():
    image_path = "output.jpg"
    subprocess.run(["start", image_path], shell=True)


def on_closing():
    os.remove("output.jpg")
    window.destroy()


if __name__ == "__main__":
    window = ctk.CTk()
    window.title("LookUpTheSkies")
    window.resizable(False, False)
    window.geometry("370x250+700+340")
    window.protocol("WM_DELETE_WINDOW", on_closing)

    # global status_label
    status_label = ctk.CTkLabel(window, text="Файл не выбран", text_color="grey")
    status_label.pack(pady=10)

    button_get_path = ctk.CTkButton(window, text="Выбрать файл", command=get_path)
    button_get_path.pack(pady=10)

    button_run_exe = ctk.CTkButton(window, text="Начать поиск", command=run_exe)
    button_run_exe.pack()

    output_label = ctk.CTkLabel(window, text="")
    output_label.pack(pady=10)

    window.mainloop()
