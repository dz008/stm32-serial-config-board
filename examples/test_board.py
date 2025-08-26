#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
STM32串口配置板简单测试脚本
STM32 Serial Config Board Simple Test Script

使用方法 / Usage:
python test_board.py [端口号] [波特率]
python test_board.py COM3 115200
python test_board.py /dev/ttyUSB0 115200
"""

import sys
import serial
import time

def test_serial_board(port="COM3", baudrate=115200):
    """
    测试STM32串口配置板基本功能
    Test STM32 serial config board basic functionality
    """
    
    print(f"开始测试STM32串口配置板 / Starting STM32 Serial Config Board Test")
    print(f"端口 / Port: {port}")
    print(f"波特率 / Baudrate: {baudrate}")
    print("-" * 50)
    
    try:
        # 打开串口 / Open serial port
        ser = serial.Serial(port, baudrate, timeout=2)
        print("✓ 串口连接成功 / Serial connection successful")
        
        # 测试数据发送 / Test data transmission
        test_messages = [
            b"Hello STM32!\r\n",
            b"Test Message 1\r\n",
            b"Test Message 2\r\n",
            b"Serial Bridge Test\r\n"
        ]
        
        print("\n发送测试数据 / Sending test data:")
        for i, msg in enumerate(test_messages, 1):
            print(f"  {i}. 发送 / Sending: {msg.decode().strip()}")
            ser.write(msg)
            time.sleep(0.5)
            
            # 检查回应 / Check response
            if ser.in_waiting > 0:
                response = ser.read(ser.in_waiting)
                print(f"     接收 / Received: {response.decode('utf-8', errors='ignore').strip()}")
            else:
                print("     无回应 / No response")
        
        # 性能测试 / Performance test
        print("\n性能测试 / Performance test:")
        start_time = time.time()
        test_data = b"Performance test data " * 10 + b"\r\n"
        
        for i in range(10):
            ser.write(test_data)
            if ser.in_waiting > 0:
                ser.read(ser.in_waiting)
        
        end_time = time.time()
        elapsed_time = end_time - start_time
        data_size = len(test_data) * 10
        
        print(f"  数据量 / Data size: {data_size} bytes")
        print(f"  用时 / Time elapsed: {elapsed_time:.2f} seconds")
        print(f"  速度 / Speed: {data_size/elapsed_time:.0f} bytes/sec")
        
        # 清理并关闭 / Cleanup and close
        ser.close()
        print("\n✓ 测试完成 / Test completed successfully")
        return True
        
    except serial.SerialException as e:
        print(f"✗ 串口错误 / Serial error: {e}")
        return False
    except Exception as e:
        print(f"✗ 其他错误 / Other error: {e}")
        return False

def main():
    """主函数 / Main function"""
    
    # 解析命令行参数 / Parse command line arguments
    if len(sys.argv) >= 2:
        port = sys.argv[1]
    else:
        # 默认端口 / Default port
        import platform
        if platform.system() == "Windows":
            port = "COM3"
        else:
            port = "/dev/ttyUSB0"
    
    if len(sys.argv) >= 3:
        try:
            baudrate = int(sys.argv[2])
        except ValueError:
            print("错误: 无效的波特率 / Error: Invalid baudrate")
            return
    else:
        baudrate = 115200
    
    # 运行测试 / Run test
    success = test_serial_board(port, baudrate)
    
    if success:
        print("\n🎉 所有测试通过! / All tests passed!")
    else:
        print("\n❌ 测试失败 / Test failed")
        print("\n故障排除建议 / Troubleshooting suggestions:")
        print("1. 检查设备连接 / Check device connection")
        print("2. 确认驱动已安装 / Confirm driver is installed")
        print("3. 检查端口号是否正确 / Check if port number is correct")
        print("4. 尝试不同的波特率 / Try different baudrate")

if __name__ == "__main__":
    main()