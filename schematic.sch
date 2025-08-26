# 嘉立创EDA 标准原理图示例
[STM32F103C8T6]
- VCC, GND
- PA9 (TX), PA10 (RX)
- Boot, Reset
[CH340G]
- USB D+, D-
- TXD, RXD
- 3.3V, GND
[RS485接口]
- MAX485
- A/B端口，DE/RE控制
[电源管理]
- AMS1117-3.3V
- 电源接口 (USB/插座)
[其他]
- 杜邦串口接口
- 指示灯, 按键
- 相关电容电阻
# 主要连线说明
STM32 TX/RX ↔ CH340 ↔ 杜邦口 ↔ RS485芯片
USB供电 ↔ AMS1117 ↔ 3.3V/5V供电
