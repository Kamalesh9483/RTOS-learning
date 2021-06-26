// Using one core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// String to print in serial monitor
const char text[] = "Hi this is Kamalesh";

// Creating task handles
static TaskHandle_t task1Handle = NULL;
static TaskHandle_t task2Handle = NULL;

// Tasks function
// task1 to print to serial monitor with low priority
void task1(void* parameter)
{
  int txtLength = strlen(text); // to get length of string
  // print text to serial
  while (1)
  {
    Serial.println(); // Serial. print() prints only the number or string, and
    // Serial. println() prints it with a newline character.
    for (int i = 0; i < txtLength; i++)
    {
      Serial.print(text[i]);
    }
    Serial.println();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task2 to print serial monitort wih high priority
void task2(void* parameter)
{
  while (1)
  {
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// Main function to run priotity one on core 1
void setup() {
  // serial slowed down to watch preemption
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("Free Rtos Demo");

  //to check in which core a task is running, we will use the xPortGetCoreID function

  Serial.print("Setup and loop task runnning on core");
  Serial.print(xPortGetCoreID());
  Serial.print("with priority");
  //Serial.println(uTaskPriorityGet(NULL));

  // Task1
  xTaskCreatePinnedToCore(task1,                // Function
                          "Task 1",             // Function name
                          1024,                 // size - bytes in case of ESP32, words in case of vanila RTOS
                          (void*) 0,            // Argument passed to task1 function
                          tskIDLE_PRIORITY + 1, // setting priority to 1
                          &task1Handle,         // Task1 handle
                          app_cpu);             // only applicable to ESP2 not to vanila RTOS


  // Task2
  xTaskCreatePinnedToCore(task2,                // Function
                          "Task 2",             // Function name
                          1024,                 // size - bytes in case of ESP32, words in case of vanila RTOS
                          (void*) 0,            // Argument passed to task1 function
                          tskIDLE_PRIORITY + 2, // setting priority to 1
                          &task2Handle,         // Task1 handle
                          app_cpu);             // only applicable to ESP2 not to vanila RTOS
}


void loop() {
  // suspending higher priority task on regular intervals
  for (int i = 0; i < 3; i++)
  {
    vTaskSuspend(task2Handle);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(task2Handle);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }

  // Deleting low priority tasks
  if (task1Handle != NULL)
  {
    vTaskDelete(task1Handle);
    task1Handle = NULL;
  }
}
