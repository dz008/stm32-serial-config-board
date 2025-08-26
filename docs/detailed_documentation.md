# STM32串口配置板详细文档 / STM32 Serial Configuration Board Documentation

## 目录 / Table of Contents
1. [项目概述 / Project Overview](#项目概述--project-overview)
2. [硬件规格 / Hardware Specifications](#硬件规格--hardware-specifications)
3. [引脚定义 / Pin Definitions](#引脚定义--pin-definitions)
4. [软件架构 / Software Architecture](#软件架构--software-architecture)
5. [开发环境 / Development Environment](#开发环境--development-environment)
6. [编译和烧录 / Build and Flash](#编译和烧录--build-and-flash)
7. [使用指南 / User Guide](#使用指南--user-guide)
8. [故障排除 / Troubleshooting](#故障排除--troubleshooting)
9. [扩展功能 / Extended Features](#扩展功能--extended-features)

## 项目概述 / Project Overview

STM32串口配置板是基于STM32F103C8T6微控制器的多功能串口转换设备，主要功能包括：
- USB转串口通信
- RS485差分通信
- 多种波特率支持
- 可编程配置
- LED状态指示

The STM32 Serial Configuration Board is a multifunctional serial conversion device based on the STM32F103C8T6 microcontroller, with main features including:
- USB to serial communication
- RS485 differential communication  
- Multiple baudrate support
- Programmable configuration
- LED status indication

## 硬件规格 / Hardware Specifications

### 主控芯片 / Main Controller
- **型号 / Model**: STM32F103C8T6
- **架构 / Architecture**: ARM Cortex-M3
- **主频 / Clock**: 72MHz
- **Flash**: 64KB
- **RAM**: 20KB
- **封装 / Package**: LQFP48

### 通信接口 / Communication Interfaces

#### USB接口 / USB Interface
- **芯片 / Chip**: CH340G
- **接口类型 / Interface**: Micro USB
- **速度 / Speed**: Full Speed (12Mbps)
- **驱动 / Driver**: CH340G通用驱动

#### RS485接口 / RS485 Interface  
- **芯片 / Chip**: MAX485
- **速度 / Speed**: 最高10Mbps / Up to 10Mbps
- **距离 / Distance**: 最远1200米 / Up to 1200m
- **接口 / Interface**: 螺丝端子 / Screw terminal

### 电源规格 / Power Specifications
- **输入电压 / Input Voltage**: 5V (USB) 或 / or 5V (外部 / External)
- **工作电压 / Operating Voltage**: 3.3V
- **功耗 / Power Consumption**: < 100mA
- **稳压器 / Regulator**: AMS1117-3.3V

## 引脚定义 / Pin Definitions

### STM32F103C8T6引脚配置 / STM32F103C8T6 Pin Configuration

| 引脚 / Pin | 功能 / Function | 连接 / Connection | 备注 / Notes |
|------------|-----------------|-------------------|--------------|
| PA9 | USART1_TX | CH340G RXD | USB串口发送 / USB serial TX |
| PA10 | USART1_RX | CH340G TXD | USB串口接收 / USB serial RX |
| PA2 | USART2_TX | MAX485 DI | RS485数据输入 / RS485 data input |
| PA3 | USART2_RX | MAX485 RO | RS485数据输出 / RS485 data output |
| PA1 | GPIO_Output | MAX485 DE/RE | RS485收发控制 / RS485 TX/RX control |
| PC13 | GPIO_Output | LED | 状态指示灯 / Status LED |
| PA0 | GPIO_Input | BOOT按键 / BOOT button | 启动模式选择 / Boot mode selection |
| NRST | Reset | 复位按键 / Reset button | 系统复位 / System reset |

### 外部接口定义 / External Interface Definition

#### J1: USB接口 / USB Interface
| 引脚 / Pin | 信号 / Signal | 说明 / Description |
|------------|---------------|-------------------|
| 1 | VCC | +5V电源 / +5V Power |
| 2 | D- | USB数据负 / USB Data - |
| 3 | D+ | USB数据正 / USB Data + |
| 4 | GND | 地 / Ground |

#### J2: UART接口 / UART Interface  
| 引脚 / Pin | 信号 / Signal | 说明 / Description |
|------------|---------------|-------------------|
| 1 | GND | 地 / Ground |
| 2 | 3V3 | 3.3V电源输出 / 3.3V Power Output |
| 3 | TX | 串口发送 / Serial TX |
| 4 | RX | 串口接收 / Serial RX |

#### J3: RS485接口 / RS485 Interface
| 引脚 / Pin | 信号 / Signal | 说明 / Description |
|------------|---------------|-------------------|
| 1 | A | RS485差分信号A / RS485 Differential A |
| 2 | B | RS485差分信号B / RS485 Differential B |

#### J4: 调试接口 / Debug Interface (SWD)
| 引脚 / Pin | 信号 / Signal | 说明 / Description |
|------------|---------------|-------------------|
| 1 | 3V3 | 3.3V电源 / 3.3V Power |
| 2 | SWDIO | SWD数据 / SWD Data |
| 3 | SWCLK | SWD时钟 / SWD Clock |
| 4 | GND | 地 / Ground |

## 软件架构 / Software Architecture

### 系统架构图 / System Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   USB Host      │    │   STM32F103     │    │   RS485 Device  │
│   (Computer)    │◄──►│   Controller    │◄──►│   (Remote)      │
└─────────────────┘    └─────────────────┘    └─────────────────┘
        │                       │                       │
        │                       │                       │
   CH340G Driver           Firmware Logic          MAX485 Transceiver
```

### 软件模块 / Software Modules

1. **HAL驱动层 / HAL Driver Layer**
   - STM32F1xx HAL库 / STM32F1xx HAL Library
   - 硬件抽象层 / Hardware Abstraction Layer

2. **应用层 / Application Layer**  
   - 主控制逻辑 / Main control logic
   - 数据转发 / Data forwarding
   - 协议转换 / Protocol conversion

3. **外设驱动 / Peripheral Drivers**
   - UART驱动 / UART driver
   - GPIO控制 / GPIO control
   - 中断处理 / Interrupt handling

### 数据流图 / Data Flow Diagram

```
USB数据 / USB Data
      ↓
  CH340G转换 / CH340G Conversion
      ↓
  UART1接收 / UART1 Reception
      ↓
  STM32处理 / STM32 Processing
      ↓
  UART2发送 / UART2 Transmission  
      ↓
  MAX485转换 / MAX485 Conversion
      ↓
  RS485输出 / RS485 Output
```

## 开发环境 / Development Environment

### 推荐工具链 / Recommended Toolchain

1. **IDE选择 / IDE Options**
   - STM32CubeIDE (推荐 / Recommended)
   - Keil MDK-ARM
   - IAR Embedded Workbench
   - VSCode + PlatformIO

2. **编译器 / Compiler**
   - GCC ARM Embedded
   - ARM Compiler 6

3. **调试器 / Debugger**
   - ST-Link/V2
   - J-Link
   - DAP-Link

### 环境配置 / Environment Setup

#### Windows环境 / Windows Environment
```bash
# 安装STM32CubeIDE / Install STM32CubeIDE
# 下载地址 / Download: https://www.st.com/zh/development-tools/stm32cubeide.html

# 安装Git / Install Git
# 下载地址 / Download: https://git-scm.com/download/win

# 安装CH340G驱动 / Install CH340G Driver
# 下载地址 / Download: http://www.wch.cn/downloads/CH341SER_EXE.html
```

#### Linux环境 / Linux Environment
```bash
# 安装编译工具链 / Install toolchain
sudo apt update
sudo apt install gcc-arm-none-eabi gdb-multiarch openocd

# 安装STM32CubeProgrammer / Install STM32CubeProgrammer
# 下载地址 / Download: https://www.st.com/zh/development-tools/stm32cubeprog.html

# 克隆项目 / Clone project
git clone https://github.com/dz008/stm32-serial-config-board.git
cd stm32-serial-config-board
```

#### macOS环境 / macOS Environment
```bash
# 使用Homebrew安装工具链 / Install toolchain via Homebrew
brew install --cask gcc-arm-embedded
brew install openocd

# 安装CH340G驱动 / Install CH340G Driver
# 下载地址 / Download: http://www.wch.cn/downloads/CH341SER_MAC_ZIP.html
```

## 编译和烧录 / Build and Flash

### 使用Makefile编译 / Build with Makefile

```bash
# 进入固件目录 / Enter firmware directory
cd firmware

# 编译项目 / Build project
make

# 清理编译文件 / Clean build files
make clean

# 烧录到设备 / Flash to device (需要st-flash工具 / requires st-flash tool)
make flash
```

### 使用STM32CubeIDE编译 / Build with STM32CubeIDE

1. 打开STM32CubeIDE / Open STM32CubeIDE
2. 导入项目 / Import project: File → Import → Existing Projects into Workspace
3. 选择项目目录 / Select project directory
4. 点击编译 / Click build: Project → Build All
5. 连接ST-Link调试器 / Connect ST-Link debugger
6. 点击调试 / Click debug: Run → Debug As → STM32 MCU C/C++ Application

### 烧录方法 / Flash Methods

#### 使用ST-Link / Using ST-Link
```bash
# 使用STM32CubeProgrammer / Using STM32CubeProgrammer
STM32_Programmer_CLI -c port=SWD -w firmware/build/stm32_serial_config_board.bin 0x08000000 -v -rst

# 使用st-flash工具 / Using st-flash tool
st-flash write firmware/build/stm32_serial_config_board.bin 0x8000000
```

#### 使用串口烧录 / Using Serial Flash
```bash
# 将BOOT0引脚拉高，按复位按键进入BootLoader模式
# Pull BOOT0 pin high, press reset button to enter BootLoader mode

# 使用stm32flash工具 / Using stm32flash tool
stm32flash -w firmware/build/stm32_serial_config_board.bin -v -g 0x0 /dev/ttyUSB0
```

## 使用指南 / User Guide

### 初次使用 / First Time Usage

1. **硬件准备 / Hardware Preparation**
   - 确保板子焊接完整 / Ensure board is properly soldered
   - 检查电源连接 / Check power connections
   - 准备USB线和RS485设备 / Prepare USB cable and RS485 devices

2. **驱动安装 / Driver Installation**
   - 下载并安装CH340G驱动程序 / Download and install CH340G driver
   - 连接USB线到电脑 / Connect USB cable to computer
   - 确认设备管理器中出现串口设备 / Confirm serial device appears in device manager

3. **固件烧录 / Firmware Flashing**
   - 使用ST-Link或串口下载固件 / Use ST-Link or serial to flash firmware
   - 验证LED指示灯工作状态 / Verify LED indicator status

### 配置参数 / Configuration Parameters

默认配置 / Default Configuration:
- **波特率 / Baudrate**: 115200
- **数据位 / Data bits**: 8
- **停止位 / Stop bits**: 1  
- **校验位 / Parity**: None
- **流控制 / Flow control**: None

### 状态指示 / Status Indication

| LED状态 / LED Status | 含义 / Meaning |
|---------------------|----------------|
| 常亮 / Solid ON | 系统正常运行 / System running normally |
| 快速闪烁 / Fast blinking | 数据传输中 / Data transmission |
| 慢速闪烁 / Slow blinking | 心跳指示 / Heartbeat indication |
| 常灭 / OFF | 系统未启动或错误 / System not started or error |

## 故障排除 / Troubleshooting

### 常见问题 / Common Issues

#### 1. 设备无法识别 / Device Not Recognized
**问题 / Problem**: 电脑无法识别USB设备
**解决方案 / Solution**:
- 检查USB线连接 / Check USB cable connection
- 安装或更新CH340G驱动 / Install or update CH340G driver
- 尝试不同的USB端口 / Try different USB ports
- 检查设备管理器错误信息 / Check device manager error messages

#### 2. 无法通信 / Communication Failed
**问题 / Problem**: 串口数据无法发送或接收
**解决方案 / Solution**:
- 检查波特率设置 / Check baudrate settings
- 确认串口参数配置 / Confirm serial parameters
- 检查硬件连接 / Check hardware connections
- 使用串口调试工具测试 / Test with serial debugging tools

#### 3. RS485通信异常 / RS485 Communication Error
**问题 / Problem**: RS485设备无法通信
**解决方案 / Solution**:
- 检查A/B线连接极性 / Check A/B wire polarity
- 确认终端电阻设置 / Confirm termination resistor settings
- 检查DE/RE控制时序 / Check DE/RE control timing
- 验证RS485设备地址和协议 / Verify RS485 device address and protocol

#### 4. LED不亮 / LED Not Working
**问题 / Problem**: 状态LED无法正常显示
**解决方案 / Solution**:
- 检查LED焊接 / Check LED soldering
- 确认限流电阻值 / Confirm current limiting resistor value
- 检查GPIO配置 / Check GPIO configuration
- 测量LED电压 / Measure LED voltage

### 调试方法 / Debugging Methods

#### 1. 使用串口调试 / Serial Debugging
```c
// 在代码中添加调试输出 / Add debug output in code
printf("Debug: UART data received: %s\n", data);
```

#### 2. 使用SWD调试 / SWD Debugging
- 连接ST-Link调试器 / Connect ST-Link debugger
- 设置断点进行单步调试 / Set breakpoints for step debugging
- 观察变量和寄存器状态 / Monitor variables and register status

#### 3. 使用逻辑分析仪 / Logic Analyzer
- 监控UART信号时序 / Monitor UART signal timing
- 分析RS485差分信号 / Analyze RS485 differential signals
- 检查DE/RE控制时序 / Check DE/RE control timing

## 扩展功能 / Extended Features

### 可扩展功能 / Expandable Features

1. **多协议支持 / Multi-protocol Support**
   - Modbus RTU/ASCII
   - DNP3协议 / DNP3 Protocol
   - 自定义协议 / Custom Protocol

2. **高级配置 / Advanced Configuration**
   - 波特率自适应 / Auto baudrate detection
   - 数据格式自动识别 / Auto data format recognition
   - 错误检测和恢复 / Error detection and recovery

3. **数据处理 / Data Processing**
   - 数据缓存和队列 / Data buffering and queuing
   - 协议转换 / Protocol conversion
   - 数据过滤和路由 / Data filtering and routing

4. **远程管理 / Remote Management**
   - Web配置界面 / Web configuration interface
   - 远程固件更新 / Remote firmware update
   - 状态监控和报警 / Status monitoring and alerting

### 硬件扩展 / Hardware Expansion

1. **接口扩展 / Interface Expansion**
   - 增加CAN接口 / Add CAN interface
   - 增加以太网接口 / Add Ethernet interface
   - 增加WiFi模块 / Add WiFi module

2. **存储扩展 / Storage Expansion**
   - 外部Flash存储 / External Flash storage
   - SD卡存储 / SD card storage
   - EEPROM配置存储 / EEPROM configuration storage

3. **电源管理 / Power Management**
   - 低功耗模式 / Low power mode
   - 电源监控 / Power monitoring
   - 备用电源 / Backup power

### 开源贡献 / Open Source Contribution

欢迎为项目贡献代码和改进建议：
Welcome to contribute code and improvement suggestions:

1. **Fork项目 / Fork Project**
2. **创建特性分支 / Create Feature Branch**
3. **提交更改 / Commit Changes**
4. **创建Pull Request / Create Pull Request**

项目许可证 / Project License: 请查看LICENSE文件 / Please check LICENSE file

---

更多技术支持和文档更新，请访问项目主页：
For more technical support and documentation updates, please visit the project homepage:
https://github.com/dz008/stm32-serial-config-board