#include "RVStarArduino.h"

// #define I2C_STRUCT

static const uint32_t GPIO_CLK_ARR[] = {
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOA,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOA,
    RCU_GPIOA,
    RCU_GPIOA,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC,
    RCU_GPIOC
};
static const uint32_t GPIO_PORT_ARR[] = {
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOA,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOB,
    GPIOA,
    GPIOA,
    GPIOA,
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOC,
    GPIOC
};
static const uint32_t GPIO_PIN_ARR[] = {
    GPIO_PIN_11,
    GPIO_PIN_10,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_8,
    GPIO_PIN_1,
    GPIO_PIN_0,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_7,
    GPIO_PIN_6,
    GPIO_PIN_5,
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5
};
static const uint8_t GPIO_MODE_ARR[] = {
    GPIO_MODE_IN_FLOATING, // 浮动输入模式 Floating input mode
    GPIO_MODE_OUT_PP,      // 推挽输出模式 Push-pull output mode
    GPIO_MODE_IPU,         // 上拉输入模式 Pull-up input mode
};

static const uint32_t ADC_GPIO_PIN_ARR[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5
};
static const uint8_t ADC_CHANNEL_ARR[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15
};

// Arduino Pin 3  => RV-STAR TIMER2_CH3
// Arduino Pin 5  => RV-STAR TIMER2_CH3/TIMER0_CH2_ON
// Arduino Pin 6  => RV-STAR TIMER2_CH2
// Arduino Pin 9  => RV-STAR TIMER3_CH2
// Arduino Pin 10 => RV-STAR TIMER3_CH3
// Arduino Pin 11 => RV-STAR TIMER2_CH1/TIMER0_CH0_ON
static const uint32_t TIMER_GPIO_CLK_ARR[] = {
    0,
    0,
    0,
    RCU_GPIOC,
    0,
    RCU_GPIOB,
    RCU_GPIOB,
    0,
    0,
    RCU_GPIOB,
    RCU_GPIOB,
    RCU_GPIOA
};
static const uint32_t TIMER_GPIO_PORT_ARR[] = {
    0,
    0,
    0,
    GPIOC,
    0,
    GPIOB,
    GPIOB,
    0,
    0,
    GPIOB,
    GPIOB,
    GPIOA
};
static const uint32_t TIMER_GPIO_PIN_ARR[] = {
    0,
    0,
    0,
    GPIO_PIN_9,
    0,
    GPIO_PIN_1,
    GPIO_PIN_0,
    0,
    0,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_7
};
static const uint32_t TIMER_CLK_ARR[] = {
    0,
    0,
    0,
    RCU_TIMER2,
    0,
    RCU_TIMER0,
    RCU_TIMER2,
    0,
    0,
    RCU_TIMER3,
    RCU_TIMER3,
    RCU_TIMER2
};
static const uint32_t TIMER_ARR[] = {
    0,
    0,
    0,
    TIMER2,
    0,
    TIMER0,
    TIMER2,
    0,
    0,
    TIMER3,
    TIMER3,
    TIMER2
};
static const uint32_t TIMER_CH_ARR[] = {
    0,
    0,
    0,
    TIMER_CH_3,
    0,
    TIMER_CH_3,
    TIMER_CH_2,
    0,
    0,
    TIMER_CH_2,
    TIMER_CH_3,
    TIMER_CH_1
};
static const uint32_t TIMER_FLAG_ARR[] = {
    0,
    0,
    0,
    TIMER_FLAG_CH3,
    0,
    TIMER_FLAG_CH3,
    TIMER_FLAG_CH2,
    0,
    0,
    TIMER_FLAG_CH2,
    TIMER_FLAG_CH3,
    TIMER_FLAG_CH1
};

static void initADCFunc(uint8_t pin);
static void initTimerPWMFunc(uint8_t pin);

// I2CStruct Wire = {
//     .a = 0,
//     .b = 1,
//     .begin = initI2CFuncUint8
// };

static void initADCFunc(uint8_t pin) {
    // uint32_t adc_gpio_pin = ADC_GPIO_PIN_ARR[pin];
    // uint8_t  adc_channel  = ADC_CHANNEL_ARR[pin];

    // 启用模数转换器时钟
    // Enable adc clock
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_periph_clock_enable(RCU_GPIOC);
    // 配置模数转换器时钟频率： 108MHz / 8 = 13.5MHz（超过14MHz精度会降低）
    // Configure adc clock rate: 108MHz / 8 = 13.5MHz(over 14Mhz, the accuracy
    // will be reduced)
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV8);

    // 初始化模数转换器GPIO参数（模式为模拟输入）
    // Initialize adc gpio parameter(mode is analog input)
    // gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, adc_gpio_pin);
    gpio_init(GPIOC, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ,
              GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
              GPIO_PIN_5);

    // 复位模数转换器
    // Reset adc
    adc_deinit(ADC0);
    // 配置模数转换器模式（独立模式）
    // Configure adc mode(independent mode)
    adc_mode_config(ADC0, ADC_MODE_FREE);
    // 禁用模数转换器扫描模式（只使用单次模式）
    // Disable adc scan mode(use single mode only)
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    // 禁用模数转换器连续模式（只使用单次模式）
    // Disable adc contineous mode(use single mode only)
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    // 配置模数转换器数据对齐（右对齐）
    // Configure adc data alignment(right alignment)
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    // 配置模数转换器通道的长度
    // Configure adc channel length
    adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, 1);
    // 配置模数转换器的规则通道（采样时间为239.5周期）
    // Configure adc regular channel(the sampling time is 239.5 cycles)
    // adc_regular_channel_config(ADC0, 0, adc_channel, ADC_SAMPLETIME_239POINT5);
    // 配置模数转换器触发源参数（规则通道采用软件触发）
    // Configure adc external trigger source(software trigger for regular channel)
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL,
                                       ADC0_1_EXTTRIG_REGULAR_NONE);
    // 配置模数转换器触发器参数
    // Configure adc external trigger(Arduino)
    adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, ENABLE);
    // 配置模数转换器分辨率（默认采用10bit）
    // Configure adc resolution(the default is 10bit)
    adc_resolution_config(ADC0, ADC_RESOLUTION_10B);

    // 启用模数转换器
    // Enable adc
    adc_enable(ADC0);
    // 校准模数转换器并复位校准
    // Calibrate adc and reset calibration
    adc_calibration_enable(ADC0);
}

static void initTimerPWMFunc(uint8_t pin) {
    uint32_t timer_gpio_clk  = TIMER_GPIO_CLK_ARR[pin];
    uint32_t timer_gpio_port = TIMER_GPIO_PORT_ARR[pin];
    uint32_t timer_gpio_pin  = TIMER_GPIO_PIN_ARR[pin];
    uint32_t timer_clk       = TIMER_CLK_ARR[pin];
    uint32_t timer           = TIMER_ARR[pin];
    uint32_t timer_ch        = TIMER_CH_ARR[pin];

    // 启用定时器时钟
    // Enable timer clock
    rcu_periph_clock_enable(timer_gpio_clk);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(timer_clk);

    // 初始化定时器GPIO参数（模式为推挽输出）
    // Initialize timer gpio parameter(mode is push-pull)
    gpio_init(timer_gpio_port, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,
              timer_gpio_pin);

    // 复位定时器
    // Reset timer
    timer_deinit(timer);

    timer_parameter_struct    timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;

    // 初始化定时器初始参数结构
    // Initialize timer init parameter struct
    timer_struct_para_init(&timer_initpara);
    // 配置定时器初始参数
    // Configure timer init parameter
    timer_initpara.prescaler         = 107;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 254;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(timer, &timer_initpara);

    // 初始化定时器通道输出参数结构
    // Initialize timer channel output parameter struct
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    // 配置定时器通道输出参数（PWM模式）
    // Configure timer channel output parameter(PWM mode)
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    // 启用定时器通道输出
    // Enable timer output
    timer_channel_output_config(timer, timer_ch, &timer_ocinitpara);

    // 配置定时器通道为PWM0模式
    // Configure timer channel in PWM0 mode
    timer_channel_output_pulse_value_config(timer, timer_ch, 255);
    timer_channel_output_mode_config(timer, timer_ch, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(timer, timer_ch,
                                       TIMER_OC_SHADOW_DISABLE);

    // 启用定时器自动重加载功能
    // Enable timer auto reload shadow function
    timer_auto_reload_shadow_enable(timer);
    // 启用定时器
    // Enable timer
    timer_enable(timer);
}

int analogRead(uint8_t pin) {
    uint8_t adc_channel = ADC_CHANNEL_ARR[pin];

    // 第一次使用需要初始化模数转换器功能
    // The first use needs to initialize adc function
    if (!adc_flag_get(ADC0, ADC_FLAG_STRC)) {
        initADCFunc(pin);
    }
    // 配置模数转换器的规则通道（采样时间为239.5周期）
    // Configure adc regular channel(the sampling time is 239.5 cycles)
    adc_regular_channel_config(ADC0, 0, adc_channel, ADC_SAMPLETIME_239POINT5);
    // 启用模数转换器软件触发转换
    // Enable adc software trigger conversion
    adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
    // 等待模数转换器软件转换结束
    // Wait for adc software conversion to end
    while (!adc_flag_get(ADC0, ADC_FLAG_EOC));

    return adc_regular_data_read(ADC0);
}

void analogWrite(uint8_t pin, int val) {
    uint32_t timer      = TIMER_ARR[pin];
    uint32_t timer_ch   = TIMER_CH_ARR[pin];
    uint32_t timer_flag = TIMER_FLAG_ARR[pin];

    // 如果定时器通道未初始化，则先初始化定时器通道为PWM模式
    // If timer channel is not initialized, initialize timer channel to PWM mode
    if (!timer_flag_get(timer, timer_flag)) {
        initTimerPWMFunc(pin);
    }

    // 配置定时器通道输出脉冲值
    // Configure timer channel output pulse value
    timer_channel_output_pulse_value_config(timer, timer_ch, val);
}

int digitalRead(uint8_t pin) {
    uint32_t gpio_port = GPIO_PORT_ARR[pin];
    uint32_t gpio_pin  = GPIO_PIN_ARR[pin];
    return gpio_input_bit_get(gpio_port, gpio_pin);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    uint32_t gpio_port = GPIO_PORT_ARR[pin];
    uint32_t gpio_pin  = GPIO_PIN_ARR[pin];
    gpio_bit_write(gpio_port, gpio_pin, val);
}

void pinMode(uint8_t pin, uint8_t mode) {
    uint32_t gpio_clk  = GPIO_CLK_ARR[pin];
    uint32_t gpio_port = GPIO_PORT_ARR[pin];
    uint32_t gpio_pin  = GPIO_PIN_ARR[pin];
    uint8_t  gpio_mode = GPIO_MODE_ARR[mode];

    rcu_periph_clock_enable(gpio_clk);
    gpio_init(gpio_port, gpio_mode, GPIO_OSPEED_50MHZ, gpio_pin);
    gpio_bit_write(gpio_port, gpio_pin, RESET);
}

void delay(unsigned long ms) {
    uint64_t ticks_start = 0;
    uint64_t ticks_delta = 0;
    uint64_t ticks_delay = (SOC_TIMER_FREQ * (uint64_t)ms) / 1000;

    ticks_start = SysTimer_GetLoadValue();

    do {
        ticks_delta = SysTimer_GetLoadValue() - ticks_start;
    }
    while (ticks_delta < ticks_delay);
}

void delayMicroseconds(unsigned int us) {
    delay(us / 1000);
}

unsigned long micros(void) {
    uint32_t ms = millis();
    uint32_t us = ms * 1000;
    return us;
}

unsigned long millis(void) {
    uint64_t ticks_delta = SysTimer_GetLoadValue() - 0;
    uint32_t ms = (uint32_t)(ticks_delta * 1000 / SOC_TIMER_FREQ);
    return ms;
}
