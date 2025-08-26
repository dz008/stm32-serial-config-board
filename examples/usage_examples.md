# 使用示例 / Usage Examples

## 示例1: USB串口通信测试 / Example 1: USB Serial Communication Test

### Python测试脚本 / Python Test Script

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
STM32串口配置板USB通信测试脚本
STM32 Serial Config Board USB Communication Test Script
"""

import serial
import time
import sys

def test_usb_serial():
    """测试USB串口通信 / Test USB serial communication"""
    
    # 配置串口参数 / Configure serial parameters
    port = 'COM3'  # Windows: COM3, Linux: /dev/ttyUSB0, macOS: /dev/cu.usbserial-xxx
    baudrate = 115200
    timeout = 1
    
    try:
        # 打开串口 / Open serial port
        ser = serial.Serial(port, baudrate, timeout=timeout)
        print(f"串口已打开: {port} @ {baudrate}bps")
        print(f"Serial port opened: {port} @ {baudrate}bps")
        
        # 发送测试数据 / Send test data
        test_messages = [
            b"Hello STM32!\r\n",
            b"Serial Config Board Test\r\n", 
            b"USB to RS485 Bridge\r\n"
        ]
        
        for msg in test_messages:
            print(f"发送: {msg.decode('utf-8', errors='ignore').strip()}")
            print(f"Sending: {msg.decode('utf-8', errors='ignore').strip()}")
            ser.write(msg)
            time.sleep(0.1)
            
            # 读取回应 / Read response
            if ser.in_waiting:
                response = ser.read(ser.in_waiting)
                print(f"接收: {response.decode('utf-8', errors='ignore').strip()}")
                print(f"Received: {response.decode('utf-8', errors='ignore').strip()}")
        
        ser.close()
        print("测试完成 / Test completed")
        
    except serial.SerialException as e:
        print(f"串口错误: {e}")
        print(f"Serial error: {e}")
    except Exception as e:
        print(f"其他错误: {e}")
        print(f"Other error: {e}")

if __name__ == "__main__":
    test_usb_serial()
```

## 示例2: RS485通信测试 / Example 2: RS485 Communication Test

### Arduino测试代码 / Arduino Test Code

```cpp
/*
  STM32串口配置板RS485通信测试
  STM32 Serial Config Board RS485 Communication Test
*/

// 配置参数 / Configuration parameters
#define RS485_BAUDRATE 115200
#define RS485_DE_RE_PIN 2  // DE/RE控制引脚 / DE/RE control pin

void setup() {
  // 初始化串口 / Initialize serial
  Serial.begin(115200);
  Serial1.begin(RS485_BAUDRATE);
  
  // 配置DE/RE控制引脚 / Configure DE/RE control pin
  pinMode(RS485_DE_RE_PIN, OUTPUT);
  digitalWrite(RS485_DE_RE_PIN, LOW);  // 接收模式 / Receive mode
  
  Serial.println("RS485通信测试开始 / RS485 Communication Test Started");
  Serial.println("输入数据将通过RS485发送 / Input data will be sent via RS485");
}

void loop() {
  // USB串口到RS485 / USB Serial to RS485
  if (Serial.available()) {
    String data = Serial.readString();
    
    // 切换到发送模式 / Switch to transmit mode
    digitalWrite(RS485_DE_RE_PIN, HIGH);
    delayMicroseconds(100);
    
    // 发送数据 / Send data
    Serial1.print(data);
    Serial1.flush();
    
    // 切换到接收模式 / Switch to receive mode
    delayMicroseconds(100);
    digitalWrite(RS485_DE_RE_PIN, LOW);
    
    Serial.println("已发送: " + data);
    Serial.println("Sent: " + data);
  }
  
  // RS485到USB串口 / RS485 to USB Serial
  if (Serial1.available()) {
    String data = Serial1.readString();
    Serial.println("RS485接收: " + data);
    Serial.println("RS485 Received: " + data);
  }
}
```

## 示例3: 配置工具 / Example 3: Configuration Tool

### 串口配置工具 / Serial Configuration Tool

```python
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
STM32串口配置板配置工具
STM32 Serial Config Board Configuration Tool
"""

import serial
import tkinter as tk
from tkinter import ttk, messagebox
import threading
import queue

class SerialConfigTool:
    def __init__(self, root):
        self.root = root
        self.root.title("STM32串口配置板工具 / STM32 Serial Config Board Tool")
        self.root.geometry("600x400")
        
        self.serial_port = None
        self.running = False
        self.rx_queue = queue.Queue()
        
        self.create_widgets()
        
    def create_widgets(self):
        # 连接配置 / Connection Configuration
        config_frame = ttk.LabelFrame(self.root, text="连接配置 / Connection Config")
        config_frame.pack(fill="x", padx=10, pady=5)
        
        ttk.Label(config_frame, text="串口 / Port:").grid(row=0, column=0, sticky="w")
        self.port_var = tk.StringVar(value="COM3")
        ttk.Entry(config_frame, textvariable=self.port_var).grid(row=0, column=1, sticky="ew")
        
        ttk.Label(config_frame, text="波特率 / Baudrate:").grid(row=0, column=2, sticky="w")
        self.baudrate_var = tk.StringVar(value="115200")
        ttk.Combobox(config_frame, textvariable=self.baudrate_var, 
                    values=["9600", "115200", "230400", "460800"]).grid(row=0, column=3, sticky="ew")
        
        # 控制按钮 / Control Buttons
        control_frame = ttk.Frame(self.root)
        control_frame.pack(fill="x", padx=10, pady=5)
        
        self.connect_btn = ttk.Button(control_frame, text="连接 / Connect", command=self.toggle_connection)
        self.connect_btn.pack(side="left", padx=5)
        
        ttk.Button(control_frame, text="清除 / Clear", command=self.clear_display).pack(side="left", padx=5)
        
        # 数据显示 / Data Display
        display_frame = ttk.LabelFrame(self.root, text="数据显示 / Data Display")
        display_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        self.data_text = tk.Text(display_frame, height=15)
        scrollbar = ttk.Scrollbar(display_frame, orient="vertical", command=self.data_text.yview)
        self.data_text.configure(yscrollcommand=scrollbar.set)
        self.data_text.pack(side="left", fill="both", expand=True)
        scrollbar.pack(side="right", fill="y")
        
        # 发送区域 / Send Area
        send_frame = ttk.LabelFrame(self.root, text="发送数据 / Send Data")
        send_frame.pack(fill="x", padx=10, pady=5)
        
        self.send_var = tk.StringVar()
        self.send_entry = ttk.Entry(send_frame, textvariable=self.send_var)
        self.send_entry.pack(side="left", fill="x", expand=True, padx=5)
        self.send_entry.bind("<Return>", self.send_data)
        
        ttk.Button(send_frame, text="发送 / Send", command=self.send_data).pack(side="right", padx=5)
        
    def toggle_connection(self):
        if not self.running:
            self.connect()
        else:
            self.disconnect()
            
    def connect(self):
        try:
            self.serial_port = serial.Serial(
                port=self.port_var.get(),
                baudrate=int(self.baudrate_var.get()),
                timeout=0.1
            )
            
            self.running = True
            self.connect_btn.config(text="断开 / Disconnect")
            
            # 启动接收线程 / Start receive thread
            self.rx_thread = threading.Thread(target=self.receive_data, daemon=True)
            self.rx_thread.start()
            
            self.append_text("连接成功 / Connected successfully\n", "green")
            
        except Exception as e:
            messagebox.showerror("错误 / Error", f"连接失败 / Connection failed: {e}")
            
    def disconnect(self):
        self.running = False
        if self.serial_port:
            self.serial_port.close()
            self.serial_port = None
            
        self.connect_btn.config(text="连接 / Connect")
        self.append_text("连接已断开 / Disconnected\n", "red")
        
    def receive_data(self):
        while self.running and self.serial_port:
            try:
                if self.serial_port.in_waiting:
                    data = self.serial_port.read(self.serial_port.in_waiting)
                    self.rx_queue.put(("receive", data.decode('utf-8', errors='ignore')))
                    
            except Exception as e:
                self.rx_queue.put(("error", str(e)))
                break
                
        # 处理接收队列 / Process receive queue
        self.root.after(10, self.process_rx_queue)
        
    def process_rx_queue(self):
        try:
            while True:
                msg_type, data = self.rx_queue.get_nowait()
                if msg_type == "receive":
                    self.append_text(f"<< {data}", "blue")
                elif msg_type == "error":
                    self.append_text(f"错误 / Error: {data}\n", "red")
        except queue.Empty:
            pass
            
        if self.running:
            self.root.after(10, self.process_rx_queue)
            
    def send_data(self, event=None):
        if not self.serial_port or not self.running:
            return
            
        data = self.send_var.get()
        if data:
            try:
                self.serial_port.write((data + "\r\n").encode())
                self.append_text(f">> {data}\n", "black")
                self.send_var.set("")
            except Exception as e:
                self.append_text(f"发送错误 / Send error: {e}\n", "red")
                
    def append_text(self, text, color="black"):
        self.data_text.config(state="normal")
        self.data_text.insert("end", text)
        self.data_text.config(state="disabled")
        self.data_text.see("end")
        
    def clear_display(self):
        self.data_text.config(state="normal")
        self.data_text.delete("1.0", "end")
        self.data_text.config(state="disabled")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialConfigTool(root)
    root.mainloop()
```

## 使用说明 / Usage Instructions

### 1. 硬件连接 / Hardware Connection
1. 通过USB线连接板子到电脑 / Connect board to computer via USB cable
2. 安装CH340G驱动程序 / Install CH340G driver
3. 连接RS485设备到A/B端子 / Connect RS485 devices to A/B terminals

### 2. 软件使用 / Software Usage
1. 运行上述Python脚本 / Run the Python scripts above
2. 选择正确的串口号 / Select correct serial port
3. 配置波特率等参数 / Configure baudrate and other parameters
4. 开始通信测试 / Start communication test

### 3. 故障排除 / Troubleshooting
- **无法识别设备**: 检查CH340G驱动是否正确安装
- **通信失败**: 检查波特率、数据位、停止位设置
- **RS485无法通信**: 检查A/B线是否正确连接，是否需要终端电阻

### 4. 注意事项 / Notes
- 确保电源供应稳定 / Ensure stable power supply  
- 避免短路和反接 / Avoid short circuit and reverse connection
- RS485通信时注意DE/RE控制时序 / Pay attention to DE/RE control timing for RS485