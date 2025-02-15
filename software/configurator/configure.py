import tkinter as tk
from tkinter import ttk
import rtmidi
import sys

def log_message(direction, message):
    debug_text.insert(tk.END, f"{direction}: {message}\n")
    debug_text.see(tk.END)

def send_midi():
    try:
        if not port_var.get():
            raise Exception("No MIDI port selected.")
        outport = get_port(rtmidi.MidiOut(), port_var.get())
        
        # Send initial message
        message = [0xBA, 0x64, 0x2A]
        outport.send_message(message)
        log_message("Sent", message)
        
        for i in range(len(dropdown_vars)):
            key_type = dropdown_vars[i].get()
            key_value = entry_vars[i].get()
            
            if key_type == "note":
                message_type = 1
            elif key_type == "CC momentary":
                message_type = 2
            elif key_type == "CC toggle":
                message_type = 3
            else:
                continue
            
            message = [185, (message_type * 10) + (i + 1), int(key_value)]
            outport.send_message(message)
            log_message("Sent", message)
        
        # Send final message
        message = [0xBA, 0x64, 0x2B]
        outport.send_message(message)
        log_message("Sent", message)
        
        del outport
        submit_btn.config(state=tk.DISABLED)
    except Exception as e:
        log_message("Error", str(e))

def connect_midi():
    try:
        if not port_var.get():
            raise Exception("No MIDI port selected.")
        outport = get_port(rtmidi.MidiOut(), port_var.get())
        message = [0xBA, 0x64, 0x2C]
        outport.send_message(message)
        log_message("Sent", message)
        
        # Simulating a response (should be replaced by real MIDI input handling)
        response = [0xBA, 0x64, 61]  # Simulated response for testing
        log_message("Received", response)
        identifier = response[2]
        
        for widget in key_widgets:
            widget.grid_remove()
        
        num_keys = 6 if identifier == 61 else 4 if identifier == 41 else 0
        create_key_fields(num_keys)
        submit_btn.config(state=tk.NORMAL)
        
        # Request stored configurations
        message = [0xBA, 0x64, 0x2D]
        outport.send_message(message)
        log_message("Sent", message)
        
        # Simulating reception of stored configurations
        stored_config = [
            [185, 11, 64],
            [185, 21, 127],
            [185, 32, 100],
            [185, 41, 50],
            [185, 13, 80],
            [185, 23, 20]
        ]
        
        for msg in stored_config:
            log_message("Received", msg)
            if msg[0] == 185:
                dropdown_index = (msg[1] % 10) - 1
                message_type = msg[1] // 10
                key_value = msg[2]
                
                if dropdown_index < len(dropdown_vars):
                    if message_type == 1:
                        dropdown_vars[dropdown_index].set("note")
                    elif message_type == 2:
                        dropdown_vars[dropdown_index].set("CC momentary")
                    elif message_type == 3:
                        dropdown_vars[dropdown_index].set("CC toggle")
                    entry_vars[dropdown_index].set(str(key_value))
        
        del outport
    except Exception as e:
        log_message("Error", str(e))

def get_port(client, device_id):
    try:
        dev_id = int(device_id)
        if not dev_id < len(client.get_ports()):
            raise Exception('Device id out of range')
    except ValueError:
        for i, name in enumerate(client.get_ports()):
            if name.lower().startswith(device_id.lower()):
                return client.open_port(i)
        for i, name in enumerate(client.get_ports()):
            if device_id.lower() in name.lower():
                return client.open_port(i)
        raise Exception(f'Device "{device_id}" not found')
    return client.open_port(dev_id)

def create_key_fields(num_keys):
    global dropdown_vars, entry_vars, key_widgets
    dropdown_vars = []
    entry_vars = []
    options = ["note", "CC momentary", "CC toggle"]
    
    for i in range(num_keys):
        var = tk.StringVar()
        dropdown = ttk.Combobox(root, textvariable=var, values=options, state="readonly")
        dropdown.grid(row=2, column=i, padx=10, pady=5)
        dropdown_vars.append(var)
        key_widgets.append(dropdown)

        entry_var = tk.StringVar()
        entry = tk.Entry(root, textvariable=entry_var)
        entry.grid(row=3, column=i, padx=10, pady=5)
        entry_vars.append(entry_var)
        key_widgets.append(entry)

root = tk.Tk()
root.title("Push & Turn configurator")
root.geometry("700x500")

midi_out = rtmidi.MidiOut()
ports = midi_out.get_ports()

port_var = tk.StringVar()
tk.Label(root, text="MIDI Port:").grid(row=0, column=0, padx=10, pady=5)
port_dropdown = ttk.Combobox(root, textvariable=port_var, values=ports, state="readonly")
port_dropdown.grid(row=0, column=1, columnspan=2, padx=10, pady=5)

connect_btn = tk.Button(root, text="Connect", command=connect_midi, state=tk.DISABLED)
connect_btn.grid(row=1, column=1, columnspan=2, pady=10)

submit_btn = tk.Button(root, text="Submit", command=send_midi, state=tk.DISABLED)
submit_btn.grid(row=5, column=1, columnspan=2, pady=10)

port_dropdown.bind("<<ComboboxSelected>>", lambda event: connect_btn.config(state=tk.NORMAL))
key_widgets = []

debug_text = tk.Text(root, height=10, width=70)
debug_text.grid(row=6, column=0, columnspan=6, padx=10, pady=10)

tk.Label(root, text="Debug Log:").grid(row=5, column=0, padx=10, pady=5)

root.mainloop()