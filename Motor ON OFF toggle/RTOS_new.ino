//#include"FreeRTOS.h"
//#include"task.h"
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

const int ENA = 19;  // setting pin number
const int IN1 = 18; 
const int IN2 = 5; 

TaskHandle_t Task1_Handle = NULL; // Task Handle (refer task.h)

void Task1(void* parameter)
{
  while(1)
  {
    digitalWrite(ENA, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    vTaskDelay(5000 / portTICK_PERIOD_MS); 
    digitalWrite(ENA, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    vTaskDelay(5000 / portTICK_PERIOD_MS); 
  }
}

void setup() {
  // LED set as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
     
 //xTaskCreate() for vanilla RTOS    //(refer task.h)
 xTaskCreatePinnedToCore(Task1,                 // Function
          "Task1",                  // Name of Task
          5024,                      // Stack size (words for FreeRTOS and bytes for ESP32). 1 word = 2 byte
          (void*) 0,                // argument passed to function Task1. here we are typecasting to void
          tskIDLE_PRIORITY+1,       // Task priority -Higher the number higher the priority (ranges from 0 to 24)
                                    // #define tskIDLE_PRIORITY ((UBaseType_t)0U) in task.h. ie., bydefault the value is set to 0
                                    // to increase the priority -- tskIDLE_PRIORITY+1
          &Task1_Handle,              // Task Handle
          app_cpu
        );
   //vTaskStartScheduler(); // for vanilla FreeRTOS
}

void loop() {

}
