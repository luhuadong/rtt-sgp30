# rtt-sgp30
Support SGP30 Multi-Pixel Gas Sensor which can ouput TVOC and eCO2 data



## 1、介绍

sgp30 软件包是 SGP30 气体传感器的驱动软件包。SGP30 是一款单一芯片上具有多个传感元件的金属氧化物气体传感器，内集成 4 个气体传感器，具有完全校准的空气质量输出信号，可用于检测 TVOC 和 eCO2。

SGP30 模块支持 I2C 接口，地址固定为 0x58。



### 1.1 目录结构

| 名称     | 说明                           |
| -------- | ------------------------------ |
| docs     | 文档目录                       |
| examples | 例子目录（提供两种操作示例）   |
| inc      | 头文件目录                     |
| src      | 源代码目录（提供两种驱动接口） |

驱动源代码提供两种接口，分别是自定义接口，以及 RT-Thread 设备驱动接口（open/read/control/close）。



### 1.2 许可证

sgp30 软件包遵循 Apache license v2.0 许可，详见 `LICENSE` 文件。



### 1.3 依赖

- RT-Thread 4.0+
- 使用动态创建方式需要开启动态内存管理模块
- 使用 sensor 设备接口需要开启 sensor 设备驱动框架模块



## 2、获取 sgp30 软件包

使用 sgp30 package 需要在 RT-Thread 的包管理器中选择它，具体路径如下：

```
RT-Thread online packages --->
    peripheral libraries and drivers --->
        [*] sensors drivers  --->
            [*] SGP30: air sensor by Sensirion for detect TVOC and CO2.
```

然后让 RT-Thread 的包管理器自动更新，或者使用 `pkgs --update` 命令更新包到 BSP 中。



## 3、使用 dhtxx 软件包

### 3.1 版本说明

| 版本   | 说明                                           |
| ------ | ---------------------------------------------- |
| v0.8.0 | 支持自定义接口和 sensor 框架，支持气体浓度测量 |
| latest | 进一步优化                                     |

目前处于公测阶段，建议开发者使用 latest 版本。



### 3.2 配置选项

- 是否使用示例程序（`PKG_USING_SGP30_SAMPLE`）
- 是否使用动态内存
- 是否使用 sensor 框架



## 4、注意事项

1. 为传感器对象提供静态创建和动态创建两种方式，如果使用动态创建，请记得在使用完毕释放对应的内存空间。
2. 由于 SGP30 模块包含一个 TVOC 传感器和一个 eCO2 传感器（实际上还有 H2 和 Ethanol 传感器），因此在 sensor 框架中会注册两个设备，内部提供1位 FIFO 缓存进行同步，缓存空间在调用 `rt_device_open` 函数时创建，因此 read 之前务必确保两个设备都开启成功。
3. 由于使用 I2C 接口进行操作，因此注册时需指定具体的 I2C 总线名称，对应的句柄存放在 user_data 中。
4. SGP30 传感器上电后大约15秒才能正常检测，此前的数据一直是 TVOC 为 0，eCO2 为 400。



## 5、相关文档

见 docs 目录。



## 6、联系方式

- 维护：luhuadong@163.com
- 主页：<https://github.com/luhuadong/rtt-sgp30>