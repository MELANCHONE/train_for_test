# 嵌入式系统与 STM32 作业答案整理

> 本文根据提供的截图和前面“ARM 开始”的题目整理。存储器、引脚等参数以 STM32F103ZET6 为主要参考，其他子型号应以对应数据手册为准。

## 第一章作业 2：ARM 与嵌入式系统

### 1. ARM 代表什么含义？

ARM 最初代表 Acorn RISC Machine，后来改为 Advanced RISC Machines，中文为“高级精简指令集计算机”。ARM 也可以指 ARM 公司、ARM 指令集架构或基于该架构设计的处理器内核。

### 2. 嵌入式 ARM 典型处理器有哪些？

典型系列包括 ARM7、ARM9、ARM11、Cortex-A、Cortex-R 和 Cortex-M。常见产品有 STM32F1、NXP LPC1700、TI Stellaris/Tiva C 和 Atmel SAM3 等。

### 3. Cortex-M3 处理器有哪些产品？STM32F103 有什么特点？

常见 Cortex-M3 产品包括 ST STM32F1、NXP LPC17xx、TI LM3S 和 Atmel SAM3 系列。

STM32F103 采用 32 位 Cortex-M3 内核，最高主频通常为 72 MHz，集成 Flash、SRAM、GPIO、定时器、USART、SPI、I²C、ADC 等外设，具有实时性好、功耗较低、型号丰富、开发资料较多等特点。

### 4. 简述嵌入式系统的结构。

嵌入式系统由硬件和软件组成。

硬件包括处理器、Flash、RAM、GPIO、定时器、中断控制器、ADC/DAC、通信接口、电源和外部传感器/执行器。

软件包括 Bootloader、设备驱动、操作系统或实时操作系统、中间件和应用程序。

### 5. 什么是片上系统？

SoC（System on Chip，片上系统）是把处理器、存储器、存储器控制器、GPIO、定时器、ADC、通信接口等多个功能模块集成在一块芯片上。它可以减少芯片数量、缩小面积、降低功耗和成本并提高可靠性。

### 6. 什么是 RISC、CISC？其特点是什么？

RISC 是 Reduced Instruction Set Computer，即精简指令集计算机。其指令数量少、长度较固定、执行速度快，常使用流水线和大量寄存器，硬件结构相对简单。ARM 属于典型 RISC 架构。

CISC 是 Complex Instruction Set Computer，即复杂指令集计算机。其指令数量多、功能复杂、长度不固定，一条指令可以完成较复杂的操作，硬件结构相对复杂。传统 x86 属于典型 CISC 架构。

### 7. 什么是哈佛结构？与冯·诺伊曼结构有什么区别？

哈佛结构把程序存储器和数据存储器分开，指令总线和数据总线相互独立，可以同时读取指令和访问数据。

冯·诺伊曼结构把程序和数据存放在同一存储器中，并共用总线，结构较简单，但可能发生指令和数据访问竞争。Cortex-M3 通常采用改进型哈佛结构。

## 第一章作业 1：STM32 基础

### 1. STM32 的数据位数是多少？

STM32 是 32 位微控制器。

### 2. Cortex-M3 系列内核属于什么架构？

Cortex-M3 属于 ARMv7-M 架构，也属于 ARM 精简指令集架构。

### 3. STM32 与 ARM 有什么区别？

ARM 是处理器架构、指令集和处理器内核设计，也是一家公司。STM32 是 ST（意法半导体）生产的微控制器产品系列。ST 将 ARM Cortex-M 内核与 Flash、SRAM、GPIO、定时器、ADC 和通信接口等集成后，形成 STM32F103 等具体芯片。

### 4. STM32 内部的基本构成有哪些？

主要包括 Cortex-M 处理器内核、Flash、SRAM、系统存储器、AHB/APB1/APB2 总线、GPIO、定时器、中断控制器、ADC、DAC、USART、SPI、I²C、CAN、USB、RCC 时钟复位系统、电源管理和 JTAG/SWD 调试接口。

### 5. 说明 STM32F103RCT6 和 STM32F103ZET6 的型号含义

| 字段 | STM32F103RCT6 | STM32F103ZET6 |
|---|---|---|
| STM32 | ST 生产的 32 位 ARM 微控制器 | ST 生产的 32 位 ARM 微控制器 |
| F1 | STM32F1 产品系列 | STM32F1 产品系列 |
| 03 | Cortex-M3 子系列，最高主频通常 72 MHz | Cortex-M3 子系列，最高主频通常 72 MHz |
| R/Z | R：64 脚 | Z：144 脚 |
| C/E | C：256 KB Flash | E：512 KB Flash |
| T | LQFP 封装 | LQFP 封装 |
| 6 | 工业级温度等级，通常 -40℃ 至 85℃ | 工业级温度等级，通常 -40℃ 至 85℃ |

## 第二章存储器与引脚作业 1

### 1. STM32F103ZET6 有多少引脚？GPIO 有多少组？

STM32F103ZET6 采用 144 脚封装，最多约有 112 个 GPIO 有效 I/O，主要分为 GPIOA、GPIOB、GPIOC、GPIOD、GPIOE、GPIOF、GPIOG 7 组。实际可用数量还要扣除电源、复位、晶振、调试和特殊功能引脚。

### 2. 哪种类型的 I/O 引脚可以同时接受 3.3 V 和 5 V 电平？

具有 5 V 容忍功能的 FT 类型 I/O 可以接受 3.3 V 和 5 V 数字输入。模拟输入通常不能直接接 5 V，具体支持 5 V 容忍的引脚要查看数据手册。

### 3. STM32F103XX 的存储器容量、分区和地址范围

以 STM32F103ZET6 为例：Flash 为 512 KB，SRAM 为 64 KB；Cortex-M3 为 32 位地址空间，总地址空间 4 GB，划分为 8 个区域，每个区域 512 MB。

| 区域 | 地址范围 | 作用 |
|---|---|---|
| Code 区 | 0x00000000～0x1FFFFFFF | 程序代码区 |
| SRAM 区 | 0x20000000～0x3FFFFFFF | 片内 SRAM |
| 外设区 | 0x40000000～0x5FFFFFFF | 片上外设寄存器 |
| 外部 RAM 区 | 0x60000000～0x9FFFFFFF | 外部 RAM |
| 外部设备区 | 0xA0000000～0xDFFFFFFF | 外部设备 |
| 私有外设区 | 0xE0000000～0xE00FFFFF | Cortex-M 内核外设 |

STM32F103ZET6 实际 Flash 通常映射在 0x08000000～0x0807FFFF，实际 SRAM 通常映射在 0x20000000～0x2000FFFF。

### 4. GPIOA、RCC、EXTI、SRAM 的地址范围

| 模块 | 地址范围 |
|---|---|
| GPIOA | 0x40010800～0x40010BFF |
| EXTI | 0x40010400～0x400107FF |
| RCC | 0x40021000～0x400213FF |
| SRAM | 0x20000000～0x2000FFFF |

### 5. 512 MB 的地址范围

512 MB 等于 0x20000000 字节。从 0x00000000 开始时，地址范围是：

~~~text
0x00000000 ～ 0x1FFFFFFF
~~~

## 第二章作业 2：时钟树

### 1. 什么是 RCC？

RCC 是 Reset and Clock Control，即复位和时钟控制。它负责选择时钟源、配置时钟树、设置系统时钟、开关外设时钟、管理复位以及配置 PLL、AHB、APB1 和 APB2 分频。

### 2. STM32F103 的主要时钟源和频率

| 时钟源 | 含义 | 典型频率和用途 |
|---|---|---|
| HSI | 高速内部时钟 | 8 MHz，片内 RC 振荡器 |
| HSE | 高速外部时钟 | 常用 8 MHz，实际由外部晶振决定 |
| LSI | 低速内部时钟 | 约 40 kHz，常用于独立看门狗 |
| LSE | 低速外部时钟 | 32.768 kHz，常用于 RTC |
| PLL | 锁相环输出 | 对 HSI 或 HSE 倍频，系统时钟最高通常 72 MHz |

### 3. 系统时钟 SYSCLK 有哪三个来源？

SYSCLK 的三个来源是 HSI、HSE 和 PLLCLK。

常见配置为：

~~~text
8 MHz HSE → PLL × 9 → 72 MHz SYSCLK
~~~

### 4. PLL 锁相环的作用是什么？

PLL 对输入时钟进行倍频或分频，产生较高频率的系统时钟。STM32F103 可以使用 HSI/2 或 HSE 作为输入，倍频后得到最高约 72 MHz 的系统时钟。

### 5. APB2 和 APB1 的最高频率

APB2 最高 72 MHz；APB1 最高 36 MHz。APB1 连接低速外设，因此频率限制低于 APB2。

## 第二章时钟树名词解释

### SYSCLK

System Clock，系统时钟。它是单片机内核运行的主要时钟，来源可以是 HSI、HSE 或 PLLCLK，STM32F103 通常最高配置为 72 MHz。

### HSI

High Speed Internal，高速内部时钟。由内部 RC 振荡器产生，典型频率为 8 MHz，不需要外部晶振，启动快但精度相对较低。

### HSE

High Speed External，高速外部时钟。由外部晶振或外部时钟源提供，常用 8 MHz，精度通常高于 HSI，可作为 PLL 输入。

### LSI

Low Speed Internal，低速内部时钟。由内部 RC 振荡器产生，典型频率约 40 kHz，主要用于独立看门狗，也可作为 RTC 备用时钟。

### LSE

Low Speed External，低速外部时钟。通常使用 32.768 kHz 晶振，主要用于 RTC，频率稳定、功耗低。

### PLL

Phase-Locked Loop，锁相环。用于对输入时钟倍频或分频，产生系统所需的高频时钟。

### AHB

Advanced High-performance Bus，高性能总线。连接 Cortex-M3 内核、Flash、SRAM、DMA 和 APB 总线桥等模块。

### APB1

低速外设总线，连接 USART2、USART3、I²C、SPI2、CAN、TIM2～TIM7 等外设，最高通常为 36 MHz。

### APB2

高速外设总线，连接 GPIO、ADC、USART1、SPI1 和高级定时器等外设，最高通常为 72 MHz。

### HCLK

AHB 总线时钟，由 SYSCLK 分频得到，用于内核相关总线、SRAM、Flash 接口和 DMA 等。

### FCLK

Cortex-M3 内核自由运行时钟，主要用于内核内部运行和调试相关功能，具体关系取决于内核时钟配置。

### PCLK1

APB1 外设时钟，由 HCLK 分频得到，最高通常为 36 MHz。

### PCLK2

APB2 外设时钟，由 HCLK 分频得到，最高通常为 72 MHz。

## 重点速记

- STM32：32 位微控制器；
- Cortex-M3：ARMv7-M 架构；
- STM32F103ZET6：144 脚、512 KB Flash、64 KB SRAM；
- GPIO：最多约 112 个，GPIOA～GPIOG；
- SYSCLK 来源：HSI、HSE、PLL；
- HSI：8 MHz；
- LSI：约 40 kHz；
- LSE：32.768 kHz；
- APB1：最高 36 MHz；
- APB2：最高 72 MHz；
- STM32F103 常见系统时钟：72 MHz；
- 512 MB 地址区：0x00000000～0x1FFFFFFF。

