# GD32-GPIO 学习笔记

## 用到的函数

### **1. gpio_init**

**函数原型：**
  
    void gpio_init(uint32_t gpio_periph, uint32_t mode, uint32_t speed, uint32_t pin);

**使用方法：**

    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);

**函数意义：** 初始化一个GPIO，并且配置为输入或者输出模式

**注意事项：** 首先要保证要使用的GPIO的时钟开启

**参数解析：**

**gpio_periph：** 外设地址，例如GPIOA,GPIOB...

**mode：** IO模式，在GD32标准固件库中，共有四种输入模式，四种输出模式，详细列表如下：

| 值                    | 含义         | 官方解释                    |
| --------------------- | ------------ | --------------------------- |
| GPIO_MODE_AIN         | 模拟输入     | analog input mode           |
| GPIO_MODE_IN_FLOATING | 浮空输入     | floating input mode         |
| GPIO_MODE_IPD         | 下拉输入     | pull-down input mode        |
| GPIO_MODE_IPU         | 上拉输入     | pull-up input mode          |
| GPIO_MODE_OUT_OD      | 开漏输出     | GPIO output with open-drain |
| GPIO_MODE_OUT_OD      | 推挽输出     | GPIO output with push-pull  |
| GPIO_MODE_OUT_OD      | 复用开漏输出 | AFIO output with open-drain |
| GPIO_MODE_OUT_OD      | 复用推挽输出 | AFIO output with push-pull  |

**speed：** IO输出速度最大值，详细列表如下：

| 值                | 含义                    | 官方解释                         |
| ----------------- | ----------------------- | -------------------------------- |
| GPIO_OSPEED_10MHZ | 输出速度最大为10MHz     | output max speed 10MHz           |
| GPIO_OSPEED_2MHZ  | 输出速度最大为2MHz      | output max speed 2MHz            |
| GPIO_OSPEED_50MHZ | 输出速度最大为50MHz     | output max speed 50MHz           |
| GPIO_OSPEED_MAX   | 输出速度最大值高于50MHz | output max speed more than 50MHz |

**pin：** 引脚，例如GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2...

### **2. gpio_bit_set**

**函数原型：**
  
    void gpio_bit_set(uint32_t gpio_periph, uint32_t pin);

**函数意义：** 将一个GPIO拉高

    gpio_bit_set(GPIOA,GPIO_PIN_0);//将PA0拉高

### **2. gpio_bit_reset**

**函数原型：**
  
    void gpio_bit_reset(uint32_t gpio_periph, uint32_t pin);

**函数意义：** 将一个GPIO拉低

    gpio_bit_reset(GPIOA,GPIO_PIN_0);//将PA0拉低

## Demo1

Demo1使用了上述三个函数封装了一个Port,利用OOP的特性完成了PA1的Blink功能，Port.c和Port.h放在Framework/GPIO里，后期如果用到其他功能会对Port进行更新，同时也会对GPIO的学习笔记进行补充。
