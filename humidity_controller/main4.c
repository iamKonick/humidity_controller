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
  
  /* Set up LED task */
  static StackType_t led_stack[STACK_SIZE];
  static StaticTask_t led_task;

  //print task
  static StackType_t print_stack[STACK_SIZE];
  static StaticTask_t print_task;
  
  //ADC task
  static StackType_t adc_stack[STACK_SIZE];
  static StaticTask_t adc_task;
  
  /* Start up LED task */
  extern void task_led(void *pvParameters); 
  /* This function is defined in led.c but add prototype here so that the compiler knows we define this in another file.
  You could also use a header file for this */

  extern void task_print(void *pvParameters); //for simple print operation

  extern void task_adc(void *pvParameters); //for simple print operation

  // Create led the task 
  xTaskCreateStatic(
    task_led, /* name of the function implementing the task -> defined in led.c */
    (char*)"led", /* human readable name of the task */
    STACK_SIZE, /* Stack size */
    NULL, /* parameters for the function -> not required here */
    15, /* Task priority */
    led_stack, /* Stack to use for the task */
    &led_task /* Task handle (could be referenced in API calls later, e.g. for changing its priority )*/    
  );


  //print task
  xTaskCreateStatic(
    task_print, // name of the function implementing the task -> defined in led.c 
    (char*)"print", // human readable name of the task 
    STACK_SIZE, // Stack size 
    NULL, // parameters for the function -> not required here 
    15, // Task priority 
    print_stack, // Stack to use for the task 
    &print_task // Task handle (could be referenced in API calls later, e.g. for changing its priority ) 
  ); 

  
  //adc
  xTaskCreateStatic(
    task_adc, // name of the function implementing the task -> defined in led.c 
    (char*)"adc", // human readable name of the task 
    STACK_SIZE, // Stack size 
    NULL, // parameters for the function -> not required here 
    15, // Task priority 
    adc_stack, // Stack to use for the task 
    &adc_task // Task handle (could be referenced in API calls later, e.g. for changing its priority ) 
  ); 

  
  /* Also start the task (task creation only creates the task control block) */
  vTaskStartScheduler();
}