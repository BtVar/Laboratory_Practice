#include "C:\program_microcontrols\Repos\Laboratory_Practice\Core\Inc\init.h"


void GPIO_Init(void)
{
    // инициализируем USER_LED (зеленый) с помощью прямого обращения к регистрам

    *(uint32_t *)(0x40023800UL + 0x30UL) |= 0x01UL;      // Включение тактирования порта GPIOА
    *(uint32_t *)(0x40020000UL + 0x00UL) |= 0x400UL;     // Настройка работы 5-го пина GPIOA в режиме вывода сигнала (Output mode)
    *(uint32_t *)(0x40020000UL + 0x04UL) |= 0x00UL;      // Настройка на PushPull работу 5-го пина GPIOA (Output Push-Pull)
    *(uint32_t *)(0x40020000UL + 0x08UL) |= 0x400UL;     // Настройка скорости работы 5-го пина GPIOA на среднюю
    *(uint32_t *)(0x40020000UL + 0x0CUL) |= 0x00UL;      // Отключение PU/PD резисторов для 5-го пина GPIOA
    *(uint32_t *)(0x40020000UL + 0x10UL) |= 0x200000UL;  // выключаем светодиод

    // инициализируем 1-ый светодиод c помощью макросов

    SET_BIT(RCC_GPIO_EN, RCC_GPIOC_EN);                  // Включение тактирования порта GPIOС
    SET_BIT(GPIOC_MODER, MODER_GPIOC_8_OUTP);            // Настройка работы 8-го пина GPIOС в режиме вывода сигнала (Output mode)
    SET_BIT(GPIOC_OTYPER, OTYPER_GPIOC_8_PUPL);          // Настройка на PushPull работу 8-го пина GPIOС (Output Push-Pull)
    SET_BIT(GPIOC_OSPEEDR, OSPEEDR_GPIOC_8_MED);         // Настройка скорости работы 8-го пина GPIOС на среднюю
    SET_BIT(GPIOC_PUPDR, PUPDR_GPIOC_8_NPUPD);           // Отключение PU/PD резисторов для 8-го пина GPIOС
    SET_BIT(GPIOC_BSRR, BSRR_GPIOC_8_OFF);               // выключаем светодиод

    // инициализируем 2-ой светодиод

    // Тактирование порта GPIOС уже включено 
    SET_BIT(GPIOC->MODER, GPIO_MODER_MODER6_0);          // Настройка работы 6-го пина GPIOС в режиме вывода сигнала (Output mode)
    CLEAR_BIT(GPIOC->OTYPER, GPIO_OTYPER_OT6);           // Настройка на PushPull работу 6-го пина GPIOC (Output Push-Pull)
    SET_BIT(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR6_0);   // Настройка скорости работы 6-го пина GPIOC на среднюю
    CLEAR_BIT(GPIOC->PUPDR, GPIO_PUPDR_PUPDR6);          // Отключение PU/PD резисторов для 6-го пина GPIOC
    SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);                 // выключаем светодиод

    // инициализируем светодиодом с параллельно подключенной кнопкой

    SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);          // Включение тактирования порта GPIOB
    SET_BIT(GPIOB->MODER, GPIO_MODER_MODER12_0);         // Настройка работы 12-го пина GPIOB в режиме вывода сигнала (Output mode)
    CLEAR_BIT(GPIOB->OTYPER, GPIO_OTYPER_OT12);          // Настройка на PushPull работу 12-го пина GPIOB (Output Push-Pull)
    SET_BIT(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR12_0);  // Настройка скорости работы 12-го пина GPIOB на среднюю
    CLEAR_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR12);         // Отключение PU/PD резисторов для 12-го пина GPIOB
}

// выбор включенного светодиода
void SetLEDState(bool ledA5, bool ledC6, bool ledC8)
{
    if (ledA5)
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5);
    else
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5);

    if (ledC6)
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS6);
    else
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR6);

    if (ledC8)
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BS8);
    else
        SET_BIT(GPIOC->BSRR, GPIO_BSRR_BR8);
}

// основная задача
void main_task(volatile bool *mode_button, volatile short int *led_button)
{
        // переключение режима работы по кнопке на PC13
        if (!READ_BIT(GPIOC->IDR, GPIO_IDR_ID13))
        {
            *mode_button = !(*mode_button);
            SetLEDState(0,0,0);
            for (int i = 0; i <= 550000; i++);
        }
        // режим 1: включен светодиод PB12
        if (!(*mode_button))
        {
            SET_BIT(GPIOB->MODER, GPIO_MODER_MODER12_0);
            SET_BIT(GPIOB->BSRR, GPIO_BSRR_BS12);
        }
        // режим 2: управление светодиодами по кнопке PB12
        else
        {
            // включаем кнопку PB12
            CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER12_0);
            SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR12_0);

            // считываем нажатия кнопки и меняем состояние светодиодов
            if (!READ_BIT(GPIOB->IDR, GPIO_IDR_ID12))
            {
                (*led_button) += 1;
                for (int i = 0; i <= 550000; i++);
            }
            if (*led_button == 0)
            {
                SetLEDState(0,0,0);
            }
            if (*led_button == 1)
            {
                SetLEDState(1,0,0);
            }
            if (*led_button == 2)
            {
                SetLEDState(1,1,0);
            }
            if (*led_button == 3)
            {
                SetLEDState(1,1,1);
            }
            if (*led_button >= 4)
            {
                *led_button = 0;
            }
        }
}

// дополнительная задача
void add_task(volatile short int *frequency_button, volatile short int *led_button, volatile int *frequency)
{
    // переключение частоты мигания по кнопке на PC13
    if (!READ_BIT(GPIOC->IDR, GPIO_IDR_ID13))
        {
            (*frequency_button) += 1;
            for (int i = 0; i <= 550000; i++);
            if (*frequency_button == 0)
            {
                *frequency = 75000;
            }
            else if (*frequency_button == 1)
            {
                *frequency = 150000;
            }
            else if (*frequency_button == 2)
            {
                *frequency = 300000;
            }
            else
            {
                *frequency_button = 0;
                *frequency = 75000;
            }
        }

    // считываем нажатия кнопки и меняем светодиоды
    if (!READ_BIT(GPIOB->IDR, GPIO_IDR_ID12))
        {
            (*led_button) += 1;
            for (int i = 0; i <= 550000; i++);
        }
    if (*led_button == 0)
        {
            SetLEDState(0,0,0);
        }
    else if (*led_button == 1)
        {
            SetLEDState(1,0,0);
            for (int i = 0; i <= *frequency; i++);
            SetLEDState(0,0,0);
            for (int i = 0; i <= *frequency; i++);
        }
    else if (*led_button == 2)
        {
            SetLEDState(1,1,0);
            for (int i = 0; i <= *frequency; i++);
            SetLEDState(0,0,0);
            for (int i = 0; i <= *frequency; i++);
        }
    else if (*led_button == 3)
        {
            SetLEDState(1,1,1);
            for (int i = 0; i <= *frequency; i++);
            SetLEDState(0,0,0);
            for (int i = 0; i <= *frequency; i++);
        }
    else
        {
            *led_button = 0;
        }
}