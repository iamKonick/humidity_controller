// FreeRTOS
#include <FreeRTOS.h>
#include <task.h>

// Input/output
#include <stdio.h>

// GPIO library
#include <bl_gpio.h>

// Define LED pins
#define LED_R_PIN 17
#define LED_G_PIN 14

//define motion sensor pin
#define MOTION_PIN 12


// define outputs
#define ON 1 // high voltage
#define OFF 0 // low voltage

// Pullup/pulldown resistors
#define ENABLE_PULLUP 1
#define DISABLE_PULLUP 0

#define ENABLE_PULLDOWN 1
#define DISABLE_PULLDOWN 0

void task_motion(void *pvParameters){
	  printf("Motion sensor task started\r\n");
	  //led
	  bl_gpio_enable_output(LED_R_PIN, DISABLE_PULLUP, DISABLE_PULLDOWN);
  	  bl_gpio_enable_output(LED_G_PIN, DISABLE_PULLUP, DISABLE_PULLDOWN);
      //motion
	  bl_gpio_enable_input(MOTION_PIN,DISABLE_PULLUP, DISABLE_PULLDOWN);


      uint8_t val;

	  // wait for 100ms
  	  vTaskDelay(100 / portTICK_PERIOD_MS);

  	  /*printf("GPIO%d is set %s with %s pullup %s pulldown\r\n",
        MOTION_PIN,
        false ? "pullup Active" : "pullup not active",
        false ? "pulldown Active" : "pulldown not active"
	  );*/
  		

	  int new, previous = 0,ret;
	  while(1){
	  	ret = bl_gpio_input_get(MOTION_PIN, &val);

		printf("GPIO%d val is %s\r\n",
		     MOTION_PIN,
		     0 == ret ? (val ? "high" : "low") : "Err"
		);

	    // wait for 1s
	    vTaskDelay(1000 / portTICK_PERIOD_MS);

	  }

	
	  
	  // should never happen but would delete the task and free allocated resources
	  vTaskDelete(NULL);

}