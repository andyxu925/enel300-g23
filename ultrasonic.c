#include "ultrasonic.h"

// ------------------------ DWT Microsecond Timer ------------------------
static void DWT_Init(void)
{
    // Enable DWT cycle counter
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;                   // reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t GetMicroseconds(void)
{
    return DWT->CYCCNT / (SystemCoreClock / 1000000);
}

static void delay_us(uint32_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = us * (SystemCoreClock / 1000000);
    while ((DWT->CYCCNT - start) < ticks);
}

// ------------------------ Ultrasonic Functions ------------------------
void Ultrasonic_Init(void)
{
    // Initialize DWT timer for microseconds
    DWT_Init();
}

static void Ultrasonic_Trigger(void)
{
    // Send 10 µs pulse to TRIG
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float Ultrasonic_Read(void)
{
    uint32_t start = 0, stop = 0;

    Ultrasonic_Trigger();  // Send pulse

    // Wait for ECHO to go HIGH
    while(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET);

    start = GetMicroseconds();

    // Wait for ECHO to go LOW
    while(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET);

    stop = GetMicroseconds();

    uint32_t pulse_width = stop - start;                 // µs
    float distance_cm = (pulse_width * 0.0343f) / 2.0f; // cm
    return distance_cm;
}
