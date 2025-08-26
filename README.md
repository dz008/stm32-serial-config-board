# STM32串口配置板 / STM32 Serial Configuration Board

[English](#english) | [中文](#中文)

## 中文

### 项目简介
基于STM32F103C8T6的多功能串口配置板，支持USB转串口、RS485通讯和多种接口转换。

### 主要功能
- **STM32F103C8T6微控制器**：主控芯片，可编程配置
- **USB转串口**：CH340G芯片，支持USB到UART转换
- **RS485接口**：MAX485芯片，支持长距离通讯
- **电源管理**：AMS1117-3.3V稳压器，支持USB和外部供电
- **多种接口**：杜邦线接口，方便连接外部设备
- **指示和控制**：LED指示灯和按键

### 硬件规格
- **主控**：STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **存储**：64KB Flash, 20KB RAM
- **通讯接口**：
  - USB Type-A/Micro USB
  - UART (杜邦线接口)
  - RS485 (A/B差分信号)
- **电源**：3.3V/5V，支持USB供电
- **尺寸**：待定（根据PCB设计）

### 接线说明
```
STM32 UART (PA9/PA10) ↔ CH340G ↔ USB接口
                       ↔ 杜邦线接口 ↔ RS485芯片(MAX485)
USB/外部电源 ↔ AMS1117稳压器 ↔ 3.3V系统供电
```

### 使用方法
1. **硬件连接**：通过USB线连接电脑，或使用外部电源供电
2. **驱动安装**：安装CH340G USB转串口驱动
3. **固件烧录**：使用ST-Link或串口下载固件到STM32
4. **配置使用**：根据需要配置串口参数和RS485通讯

### 开发环境
- **IDE**：推荐使用STM32CubeIDE、Keil MDK或PlatformIO
- **调试器**：ST-Link/V2或兼容调试器
- **编程语言**：C/C++（基于HAL库）

### 文件结构
```
├── README.md          # 项目说明文档
├── schematic.sch      # 原理图文件
├── firmware/          # STM32固件代码
├── docs/              # 详细文档
├── hardware/          # 硬件设计文件
└── examples/          # 使用示例
```

---

## English

### Project Overview
A multifunctional serial configuration board based on STM32F103C8T6, supporting USB-to-serial, RS485 communication, and various interface conversions.

### Key Features
- **STM32F103C8T6 Microcontroller**: Main control chip with programmable configuration
- **USB-to-Serial**: CH340G chip for USB to UART conversion
- **RS485 Interface**: MAX485 chip for long-distance communication
- **Power Management**: AMS1117-3.3V regulator, supports USB and external power
- **Multiple Interfaces**: Dupont wire connectors for easy external device connection
- **Indicators & Controls**: LED indicators and buttons

### Hardware Specifications
- **MCU**: STM32F103C8T6 (ARM Cortex-M3, 72MHz)
- **Memory**: 64KB Flash, 20KB RAM
- **Communication Interfaces**:
  - USB Type-A/Micro USB
  - UART (Dupont wire interface)
  - RS485 (A/B differential signals)
- **Power**: 3.3V/5V, USB powered
- **Size**: TBD (based on PCB design)

### Wiring Description
```
STM32 UART (PA9/PA10) ↔ CH340G ↔ USB Interface
                       ↔ Dupont Interface ↔ RS485 Chip (MAX485)
USB/External Power ↔ AMS1117 Regulator ↔ 3.3V System Power
```

### Usage Instructions
1. **Hardware Connection**: Connect to computer via USB cable or use external power
2. **Driver Installation**: Install CH340G USB-to-serial driver
3. **Firmware Upload**: Use ST-Link or serial port to upload firmware to STM32
4. **Configuration**: Configure serial parameters and RS485 communication as needed

### Development Environment
- **IDE**: Recommended STM32CubeIDE, Keil MDK, or PlatformIO
- **Debugger**: ST-Link/V2 or compatible debugger
- **Programming Language**: C/C++ (based on HAL library)

### File Structure
```
├── README.md          # Project documentation
├── schematic.sch      # Schematic file
├── firmware/          # STM32 firmware code
├── docs/              # Detailed documentation
├── hardware/          # Hardware design files
└── examples/          # Usage examples
```

### Getting Started
1. Clone this repository
2. Open the firmware project in your preferred IDE
3. Compile and upload the firmware to your STM32 board
4. Connect the board via USB and start using it

### Contributing
Contributions are welcome! Please feel free to submit issues and pull requests.

### License
This project is open source. Please check the LICENSE file for details.