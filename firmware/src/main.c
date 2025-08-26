/**
 * @file main.c
 * @brief STM32 Serial Configuration Board Main Application
 * @author STM32 Serial Config Board Project
 * @version 1.0.0
 * @date 2025
 * 
 * 主要功能：
 * - UART通信配置和转发
 * - RS485通信控制
 * - USB串口桥接
 * - LED状态指示
 * 
 * Main Features:
 * - UART communication configuration and forwarding
 * - RS485 communication control  
 * - USB serial bridge
 * - LED status indication
 */

#include "main.h"
#include "stm32f1xx_hal.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC

#define RS485_DE_RE_PIN GPIO_PIN_2
#define RS485_DE_RE_PORT GPIOA

/* UART配置 / UART Configuration */
#define UART_BAUDRATE 115200
#define UART_TIMEOUT 1000

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1; // USB串口 / USB Serial (PA9/PA10)
UART_HandleTypeDef huart2; // RS485串口 / RS485 Serial

/* 缓冲区 / Buffers */
uint8_t uart1_rx_buffer[256];
uint8_t uart2_rx_buffer[256];
uint8_t uart1_tx_buffer[256];
uint8_t uart2_tx_buffer[256];

/* 状态变量 / Status Variables */
volatile uint8_t uart1_rx_complete = 0;
volatile uint8_t uart2_rx_complete = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
void RS485_SetMode(uint8_t mode);
void LED_Toggle(void);
void LED_SetState(uint8_t state);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief 主函数 / Main function
 */
int main(void)
{
    /* HAL库初始化 / HAL Library initialization */
    HAL_Init();

    /* 系统时钟配置 / Configure the system clock */
    SystemClock_Config();

    /* GPIO初始化 / Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();

    /* 设置RS485为接收模式 / Set RS485 to receive mode */
    RS485_SetMode(0);

    /* LED指示启动完成 / LED indicates startup complete */
    LED_SetState(1);
    HAL_Delay(500);
    LED_SetState(0);

    /* 启动UART接收 / Start UART reception */
    HAL_UART_Receive_IT(&huart1, uart1_rx_buffer, 1);
    HAL_UART_Receive_IT(&huart2, uart2_rx_buffer, 1);

    /* 主循环 / Main loop */
    while (1)
    {
        /* UART1数据转发到UART2 (USB to RS485) */
        if (uart1_rx_complete)
        {
            uart1_rx_complete = 0;
            
            /* 设置RS485为发送模式 / Set RS485 to transmit mode */
            RS485_SetMode(1);
            
            /* 发送数据 / Transmit data */
            HAL_UART_Transmit(&huart2, uart1_rx_buffer, 1, UART_TIMEOUT);
            
            /* 设置RS485为接收模式 / Set RS485 to receive mode */
            RS485_SetMode(0);
            
            /* 重新启动接收 / Restart reception */
            HAL_UART_Receive_IT(&huart1, uart1_rx_buffer, 1);
            
            /* LED指示数据传输 / LED indicates data transmission */
            LED_Toggle();
        }

        /* UART2数据转发到UART1 (RS485 to USB) */
        if (uart2_rx_complete)
        {
            uart2_rx_complete = 0;
            
            /* 发送数据到USB / Send data to USB */
            HAL_UART_Transmit(&huart1, uart2_rx_buffer, 1, UART_TIMEOUT);
            
            /* 重新启动接收 / Restart reception */
            HAL_UART_Receive_IT(&huart2, uart2_rx_buffer, 1);
            
            /* LED指示数据传输 / LED indicates data transmission */
            LED_Toggle();
        }

        /* 简单的心跳指示 / Simple heartbeat indication */
        static uint32_t last_heartbeat = 0;
        if (HAL_GetTick() - last_heartbeat > 1000)
        {
            last_heartbeat = HAL_GetTick();
            LED_Toggle();
        }
    }
}

/**
 * @brief 系统时钟配置 / System Clock Configuration
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief GPIO初始化 / GPIO Initialization
 */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO端口时钟使能 / GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* LED引脚配置 / Configure LED pin */
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    /* RS485 DE/RE引脚配置 / Configure RS485 DE/RE pin */
    HAL_GPIO_WritePin(RS485_DE_RE_PORT, RS485_DE_RE_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = RS485_DE_RE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RS485_DE_RE_PORT, &GPIO_InitStruct);
}

/**
 * @brief USART1初始化 / USART1 Initialization
 * 用于USB串口通信 / Used for USB serial communication
 */
static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = UART_BAUDRATE;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief USART2初始化 / USART2 Initialization  
 * 用于RS485通信 / Used for RS485 communication
 */
static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = UART_BAUDRATE;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief 设置RS485模式 / Set RS485 Mode
 * @param mode: 0=接收模式 / Receive mode, 1=发送模式 / Transmit mode
 */
void RS485_SetMode(uint8_t mode)
{
    if (mode)
    {
        /* 发送模式 / Transmit mode */
        HAL_GPIO_WritePin(RS485_DE_RE_PORT, RS485_DE_RE_PIN, GPIO_PIN_SET);
    }
    else
    {
        /* 接收模式 / Receive mode */
        HAL_GPIO_WritePin(RS485_DE_RE_PORT, RS485_DE_RE_PIN, GPIO_PIN_RESET);
    }
    HAL_Delay(1); /* 小延时确保模式切换 / Small delay to ensure mode switch */
}

/**
 * @brief LED切换 / LED Toggle
 */
void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

/**
 * @brief LED设置状态 / LED Set State
 * @param state: 0=关闭 / OFF, 1=开启 / ON
 */
void LED_SetState(uint8_t state)
{
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief UART接收完成回调 / UART Receive Complete Callback
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        uart1_rx_complete = 1;
    }
    else if (huart->Instance == USART2)
    {
        uart2_rx_complete = 1;
    }
}

/**
 * @brief 错误处理函数 / Error Handler
 */
void Error_Handler(void)
{
    /* 错误指示 - 快速闪烁LED / Error indication - fast LED blinking */
    while (1)
    {
        LED_Toggle();
        HAL_Delay(100);
    }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief 断言失败报告 / Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* 用户可以添加自己的实现来报告文件名和行号 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */