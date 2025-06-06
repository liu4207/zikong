# diyidai 🚗

本项目是基于嵌入式系统的智能设备设计，采用高效的控制算法和硬件架构，集成了先进的电源管理、通信协议和传感器控制，支持精确的自动化控制与数据处理。设计包括基于 STM32 的嵌入式硬件和软件平台，适用于智能家居、机器人和自动化控制等应用。

---

## 🧩 项目特点

- ✅ **高效的 PID 控制算法**：项目核心使用 **PID 控制** 算法实现精确的反馈控制，确保系统的稳定性与快速响应，尤其在处理自动调光和其他精密控制任务时表现优异。
  
- ✅ **自适应电源管理**：通过精确的电压控制和调节，结合 **Buck 变换器**，实现对输入电源的高效转换，保证了系统的低功耗运行和稳定性。

- ✅ **先进的通信协议解耦设计**：系统采用高效的 **UART 协议**，实现了数据通信、任务调度与传感器控制的完美协作。

- ✅ **光伏 MPPT 技术**：集成 **最大功率点跟踪（MPPT）** 技术，实现对光伏电池最大化能量的提取，有效提升系统整体效率，适用于绿色能源领域。

- ✅ **精密感光与自动调光功能**：系统能够根据环境光照强度自动调节输出光源，保证最佳照明效果，同时实现 **精准感光**，最大化使用能量。

- ✅ **基于 STM32 的嵌入式控制系统**：STM32F103 处理器实现高效的任务调度和稳定的运行，适用于资源有限的嵌入式平台。

---

## 📂 工程结构

| 文件夹       | 说明                              |
|--------------|-----------------------------------|
| `DebugConfig/` | 调试配置文件                        |
| `Library/`    | 库文件，包括标准外设驱动            |
| `Listings/`   | 代码清单                            |
| `Objects/`    | 物体检测与识别算法                  |
| `Start/`      | 系统启动文件                        |
| `System/`     | 系统核心文件   |
| `User/`       | 用户层代码                          |
| `Project.uvprojx` | Keil 工程项目文件                  |

---

## 🛠️ 开发环境

- **开发板**：STM32F103C8T6
- **IDE**：Keil MDK-ARM 5.38a
- **编译工具链**：ARMCC

---

## 🚗 项目亮点总结

- ✅ **PID 控制算法**：核心控制采用 **PID 闭环控制**，确保精确的调节与稳定性，广泛应用于光照调节、速度控制等自动化任务。

- ✅ **自适应 Buck 变换器**：结合 **Buck 变换器** 高效转换电源，提高了系统的能效比和稳定性，适用于低功耗应用。

- ✅ **精准感光和自动调光功能**：光传感器的精确数据处理实现了自动调光功能，根据外界光照条件自动调节系统输出，最大化使用能源。

- ✅ **MPPT 控制技术**：集成 **光伏最大功率点跟踪（MPPT）** 技术，提高了光伏系统的能量采集效率，在阳光变化条件下确保最大输出。


---

## 🚀 快速开始

2. **打开 Keil 项目** `MDK-ARM/` 中的 `.uvprojx` 文件，使用 Keil 编译并下载程序。
3. **启动控制系统**：上传后即可通过串口或调试工具测试调光功能和控制系统。

---

## 🖥️ 串口协议

接入小米控制模块，可以使用米家app远程控制开关
接入蓝牙模块。可以使用蓝牙通信控制开关

---

## 📸 效果展示


---



## 👨‍💻 作者

刘珅凯 @ 重庆大学明月班  
联系：2323453274@qq.com

---

## 📄 License

MIT License

Copyright (c) 2025 Liu Shenkai

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
