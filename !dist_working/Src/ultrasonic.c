#include "ultrasonic.h"

static void dwt_init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t get_us(void)
{
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < ticks)
        ;
}

void ultrasonic_init(void)
{
    dwt_init();
}

void ultrasonic_trig(void)
{
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_GPIO_Port, TRIG_Pin, GPIO_PIN_RESET);
}

float ultrasonic_echo(void)
{
    uint32_t start = 0;
    uint32_t end = 0;
    uint32_t period = 0;

    ultrasonic_trig();

    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_RESET)
        ;
    start = get_us();

    while (HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin) == GPIO_PIN_SET)
        ;
    end = get_us();

    period = end - start;
    return (period * 0.0343f) / 2.0f;
}