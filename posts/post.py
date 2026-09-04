import PySimpleGUI as sg

layout = [[sg.Text("Create a Post")],
          [sg.Input(key='-INPUT-')]
          ]

window = sg.Window("My Window", layout)

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED:
        break

window.close()
