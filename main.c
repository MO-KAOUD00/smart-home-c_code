#include "STD_TYPES.h"      // Include standard data types header.
#include "BIT_MATH.h"       // Include bit manipulation macros header.
#include "DIO_interface.h"  // Include Digital I/O interface header.
#include "FreeRTOS.h"       // Include FreeRTOS header for task management.
#include "task.h"           // Include task creation and scheduling header.
#include "semphr.h"         // Include FreeRTOS semaphore header.
#include "TWI_interface.h"  // Include Two-Wire Interface (I2C) header.
#include "EEPROM_interface.h" // Include EEPROM interface header.
#include "UART_interface.h" // Include UART interface header.
#include "CLCD_interface.h" // Include character LCD interface header.
#include "SW_interface.h"   // Include switch interface header.
#include <util/delay.h>     // Include delay utility for timing functions.

#define LOG_START_ADDRESS 200       // Define the starting EEPROM address for log entries.
#define LOG_ENTRY_SIZE  sizeof(LogEntry_t) // Define the size of each log entry.

// Global variable for received UART data.
volatile u8 Recived_data = 0;

// Current EEPROM address for logging.
u8 CurrentAddress = LOG_START_ADDRESS;
// System time (volatile since it's modified in tasks).
volatile u32 SystemTime = 0;

// Variables to store the timing data for LEDs and other devices.
u8 Led1On_DataTime;
u8 Led1Off_DataTime;
u8 Led2On_DataTime;
u8 Led2Off_DataTime;
u8 Led3On_DataTime;
u8 Led3Off_DataTime;
u8 Relay1On_DataTime;
u8 Relay1Off_DataTime;
u8 Relay2On_DataTime;
u8 Relay2Off_DataTime;
u8 MotorOn_DataTime;
u8 MotorOff_DataTime;

// Semaphore handle to control access to shared resources (LEDS).
xSemaphoreHandle LEDS_sm;

// Switch configurations using SWITCH_t struct for each switch, defining their port and pin.
SWITCH_t Switch_1 = {PORT_u8D , PIN_2 , PULL_UP};
SWITCH_t Switch_2 = {PORT_u8D , PIN_3 , PULL_UP};
SWITCH_t Switch_3 = {PORT_u8D , PIN_4 , PULL_UP};
SWITCH_t Switch_4 = {PORT_u8D , PIN_5 , PULL_UP};
SWITCH_t Switch_5 = {PORT_u8D , PIN_6 , PULL_UP};
SWITCH_t Switch_6 = {PORT_u8D , PIN_7 , PULL_UP};

// Variable to hold LED state.
u8 led1;

// Task1: Receives data from UART and stores it in the Recived_data variable.
void Task1(void *p)
{
    while(1)
    {
        // Attempt to take the semaphore (wait indefinitely).
        u8 Local_semState = xSemaphoreTake(LEDS_sm , portMAX_DELAY);
        
        // If semaphore is successfully taken, proceed.
        if(Local_semState == pdPASS)
        {
            // Receive data from UART and store it in Recived_data.
            Recived_data = UART_u8ReceiveData();
            // Release the semaphore.
            xSemaphoreGive(LEDS_sm);

            // Delay for a short time (1 tick).
            vTaskDelay(1);
        }
    }
}

// Task2: Controls various output pins (LEDs, relay, motor) based on the received UART data.
void Task2(void *p)
{
    static u32 Counter = 0;  // Counter to keep track of system time.
    
    while(1)
    {
        Counter++;  // Increment the counter.

        // Attempt to take the semaphore (wait indefinitely).
        u8 Local_semState = xSemaphoreTake(LEDS_sm , portMAX_DELAY);
        
        // If semaphore is successfully taken, proceed.
        if(Local_semState == pdPASS)
        {
            // Check the value of Recived_data and control output pins accordingly.
            switch(Recived_data)
            {
                case '0':   // Turn on LED1 (Pin A0).
                    DIO_voidSetPinValue(PORT_u8A, PIN_0, PIN_HIGH);
                    Led1On_DataTime = Counter;
                    EEPROM_voidWriteByte(0, Led1On_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '1':   // Turn off LED1 (Pin A4).
                    DIO_voidSetPinValue(PORT_u8A, PIN_4, PIN_HIGH);
                    Led1Off_DataTime = Counter;
                    EEPROM_voidWriteByte(10, Led1Off_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '2':   // Turn on LED2 (Pin C6).
                    DIO_voidSetPinValue(PORT_u8C, PIN_6, PIN_HIGH);
                    Led2On_DataTime = Counter;
                    EEPROM_voidWriteByte(20, Led2On_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '3':   // Turn off LED2 (Pin D5).
                    DIO_voidSetPinValue(PORT_u8D, PIN_5, PIN_LOW);
                    Led2Off_DataTime = Counter;
                    EEPROM_voidWriteByte(40, Led2Off_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '4':   // Turn on LED3 (Pin D3).
                    DIO_voidSetPinValue(PORT_u8D, PIN_3, PIN_HIGH);
                    Led3On_DataTime = Counter;
                    EEPROM_voidWriteByte(60, Led3On_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '5':   // Turn off LED3 (Pin A7).
                    DIO_voidSetPinValue(PORT_u8A, PIN_7, PIN_HIGH);
                    Led3Off_DataTime = Counter;
                    EEPROM_voidWriteByte(80, Led3Off_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '6':   // Turn on Relay1 (Pin A0).
                    DIO_voidSetPinValue(PORT_u8A, PIN_0, PIN_LOW);
                    Relay1On_DataTime = Counter;
                    EEPROM_voidWriteByte(80, Relay1On_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '7':   // Turn off Relay1 (Pin A4).
                    DIO_voidSetPinValue(PORT_u8A, PIN_4, PIN_LOW);
                    Relay1Off_DataTime = Counter;
                    EEPROM_voidWriteByte(100, Relay1Off_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '8':   // Turn on Relay2 (Pin C6).
                    DIO_voidSetPinValue(PORT_u8C, PIN_6, PIN_LOW);
                    Relay2On_DataTime = Counter;
                    EEPROM_voidWriteByte(120, Relay2On_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case '9':   // Turn off Relay2 (Pin D5).
                    DIO_voidSetPinValue(PORT_u8D, PIN_5, PIN_HIGH);
                    Relay2Off_DataTime = Counter;
                    EEPROM_voidWriteByte(150, Relay2Off_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case 'A':   // Turn on Motor (Pin D5).
                    DIO_voidSetPinValue(PORT_u8D, PIN_5, PIN_LOW);
                    MotorOn_DataTime = Counter;
                    EEPROM_voidWriteByte(200, MotorOn_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                case 'B':   // Turn off Motor (Pin A7).
                    DIO_voidSetPinValue(PORT_u8A, PIN_7, PIN_LOW);
                    MotorOff_DataTime = Counter;
                    EEPROM_voidWriteByte(300, MotorOff_DataTime, EEPROM_DEVICE2);  // Log time to EEPROM.
                    break;
                default:
                    // Default case to handle unexpected data.
                    break;
            }

            // Release the semaphore.
            xSemaphoreGive(LEDS_sm);
            
            // Delay for a short time (1 tick).
            vTaskDelay(1);
        }
    }
}
void Task3(void *p)
{
    // Infinite loop for the task
    while(1)
    {
        // Attempt to take the semaphore to access the shared resource (LEDS_sm)
        u8 Local_semState = xSemaphoreTake(LEDS_sm , portMAX_DELAY);
        if(Local_semState == pdPASS) // If semaphore is taken successfully
        {
            // Switch case based on received UART data
            switch (Recived_data) {
                case '0':
                    // Display "Led one:on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led one:on");
                    break;
                case '1':
                    // Display "Led Two :on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led Two :on");
                    break;
                case '2':
                    // Display "Led Three : on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led Three : on");
                    break;
                case '3':
                    // Display "Relay One : on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Relay One : on");
                    break;
                case '4':
                    // Display "Relay Two:on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Relay Two:on");
                    break;
                case '5':
                    // Display "Motor : on" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Motor : on");
                    break;
                case '6':
                    // Display "Led One : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led One : off");
                    break;
                case '7':
                    // Display "Led Two : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led Two : off");
                    break;
                case '8':
                    // Display "Led Three : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Led Three : off");
                    break;
                case '9':
                    // Display "Relay One : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Relay One : off");
                    break;
                case 'A':
                    // Display "Relay Two : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Relay Two : off");
                    break;
                case 'B':
                    // Display "Motor : off" on the LCD
                    CLCD_voidClearDisplay();
                    CLCD_voidSitPosition(0,0);
                    CLCD_voidSendString("Motor : off");
                    break;
                default:
                    break; // Default case when no relevant data is received
            }

            // Toggle a status LED briefly to indicate activity
            DIO_voidSetPinValue(PORT_u8A,PIN_6,PIN_HIGH);
            _delay_ms(50);
            DIO_voidSetPinValue(PORT_u8A,PIN_6,PIN_LOW);

            // Release the semaphore after the task is complete
            xSemaphoreGive(LEDS_sm);
            vTaskDelay(10); // Delay to prevent rapid task execution
        }
    }
}

void Task4(void *p)
{
    // Variables to store the state of each LED (on/off)
    static u8 LedOneState = 0;
    static u8 LedTwoState = 0;
    static u8 LedThreeState = 0;

    // Infinite loop for the task
    while(1)
    {
        // Check if Switch 3 is pressed
        if(SWITCH_u8IsPressed(Switch_3)){
            // Toggle LED One state and set the pin accordingly
            LedOneState = !LedOneState;
            DIO_voidSetPinValue(PORT_u8A, PIN_0, LedOneState ? PIN_HIGH : PIN_LOW);
        }

        // Check if Switch 5 is pressed
        if(SWITCH_u8IsPressed(Switch_5)){
            // Toggle LED Two state and set the pin accordingly
            LedTwoState = !LedTwoState;
            DIO_voidSetPinValue(PORT_u8A, PIN_4, LedTwoState ? PIN_HIGH : PIN_LOW);
        }

        // Check if Switch 6 is pressed
        if(SWITCH_u8IsPressed(Switch_6)){
            // Toggle LED Three state and set the pin accordingly
            LedThreeState = !LedThreeState;
            DIO_voidSetPinValue(PORT_u8C,PIN_6,LedThreeState ? PIN_HIGH : PIN_LOW);
        }

        vTaskDelay(1); // Small delay to prevent rapid toggling
    }
}

int main(void)
{
    // Set up pin directions for UART communication and other peripherals
    DIO_voidSetPinDirection(PORT_u8D, PIN_0, PIN_IN); // RX pin as input
    DIO_voidSetPinDirection(PORT_u8D, PIN_1, PIN_IN); // TX pin as input
    DIO_voidSetPortDirection(PORT_u8D, PORT_IN);      // Set entire Port D as input
    DIO_voidSetPortDirection(PORT_u8A, PORT_OUT);     // Set Port A as output for LEDs
    DIO_voidSetPortDirection(PORT_u8C, PORT_OUT);     // Set Port C as output
    DIO_voidSetPortDirection(PORT_u8B, PORT_OUT);     // Set Port B as output

    // Set initial values for certain pins (pull-up resistors)
    DIO_voidSetPinValue(PORT_u8D,PIN_2,PIN_HIGH);
    DIO_voidSetPinValue(PORT_u8D,PIN_4,PIN_HIGH);
    DIO_voidSetPinValue(PORT_u8D,PIN_6,PIN_HIGH);
    DIO_voidSetPinValue(PORT_u8D,PIN_7,PIN_HIGH);

    // Set specific pins as output
    DIO_voidSetPinDirection(PORT_u8D, PIN_5, PIN_OUT);
    DIO_voidSetPinDirection(PORT_u8D, PIN_3, PIN_OUT);

    // Initialize UART, LCD, and TWI (I2C) communication
    UART_voidInit();
    CLCD_voidInit();
    TWI_voidMasterInit(1);

    // Create a binary semaphore for shared resource (LEDS_sm)
    vSemaphoreCreateBinary(LEDS_sm);

    // Create tasks with specific priorities
    xTaskCreate(&Task1, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);  // Task 1 (UART communication)
    xTaskCreate(&Task2, NULL, configMINIMAL_STACK_SIZE, NULL, 2, NULL);  // Task 2 (LED control)
    xTaskCreate(&Task3, NULL, configMINIMAL_STACK_SIZE, NULL, 4, NULL);  // Task 3 (LCD display control)
    xTaskCreate(&Task4, NULL, configMINIMAL_STACK_SIZE, NULL, 3, NULL);  // Task 4 (LED toggle control)

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Main loop (this should never be reached as the scheduler takes over)
    while(1);
}

