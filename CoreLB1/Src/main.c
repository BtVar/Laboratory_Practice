#include "C:\Users\parni\Git_repos\Laboratory_Practice\CoreLB1\Inc\init.h"


volatile bool mode_button = 0;
volatile short int led_button = 0;

volatile int frequency = 75000;
volatile short int frequency_button = 0;

int main(void)
{
    
    GPIO_Init();

    // раскомментировать для add_task()
    /*
    CLEAR_BIT(GPIOB->MODER, GPIO_MODER_MODER12_0); //input mode
    SET_BIT(GPIOB->PUPDR, GPIO_PUPDR_PUPDR12_0);   // pull-up
    */
    
    while(1)
    {
        main_task(&mode_button, &led_button);
        //add_task(&frequency_button, &led_button, &frequency);
    }
}