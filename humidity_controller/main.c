// FreeRTOS includes
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>

//UART communication
#include <bl_uart.h>


// DMA library
#include <bl_dma.h>




/* Define heap regions */
extern uint8_t _heap_start;
extern uint8_t _heap_size;
extern uint8_t _heap_wifi_start;
extern uint8_t _heap_wifi_size;

static HeapRegion_t xHeapRegions[] =
{
  { &_heap_start, (unsigned int) &_heap_size},
  { &_heap_wifi_start, (unsigned int) &_heap_wifi_size },
  { NULL, 0},
  { NULL, 0}
};


/* Size of the stack for the task */
#define STACK_SIZE 512

void bfl_main(void)
{
  /* Initialize UART
   * Ports: 16+7 (TX+RX)
   * Baudrate: 2 million
   */
  bl_uart_init(0, 16, 7, 255, 255, 2 * 1000 * 1000);
  
   /* (Re)define Heap */
  vPortDefineHeapRegions(xHeapRegions);

  bl_dma_init();
  
  
  
  //switch led task
  static StackType_t switch_stack[STACK_SIZE];
  static StaticTask_t switch_task;
  
  extern void task_switch_led(void *pvParameters); //for simple print operation


  
  //adc
  xTaskCreateStatic(
    task_switch_led, // name of the function implementing the task -> defined in led.c 
    (char*)"switch led", // human readable name of the task 
    STACK_SIZE, // Stack size 
    NULL, // parameters for the function -> not required here 
    15, // Task priority 
    switch_stack, // Stack to use for the task 
    &switch_task // Task handle (could be referenced in API calls later, e.g. for changing its priority ) 
  ); 

  
  /* Also start the task (task creation only creates the task control block) */
  vTaskStartScheduler();
}