// Define returned error codes
#define ERROR_UNDEFINED_COMMAND 9999                                                   //Reply "9999" means the command is not valid
#define ERROR_COMMAND_NOT_ACTIVATED 9998                                               //Reply "9998" means the command is not activated (defined in current device)
#define ERROR_INVALID_DIGITAL_PIN 9997                                                 //Reply "9997" means the pin number is not valid for digital read/write
#define ERROR_INVALID_ANALOGIN_PIN 9996                                                //Reply "9996" means the pin number is not valid for analog input read
#define ERROR_INVALID_PWM_PIN 9995                                                     //Reply "9993" means the pin number is not valid for analog output write (PWM)
#define ERROR_SENSOR_READ 9994                                                         //Reply "9995" means the sensor returned a reading error
#define ERROR_UNDEFINED_COMMAND 9993                                                   //Reply "9994" means the sensor reading units are invalid
#define ERROR_SENSOR_READ 9992                                                         //Reply "9992" means the sensor is invalid or not found
#define ERROR_SENSOR_REQUEST_FAIL 9991                                                 //Reply "9991" mesan the sensor function received an invalid request

// Define pins that are available for operations
//Discard SPI pins (2(Unknown), 10(CS), 11(MOSI), 12(MISO), 13(SCK)) - Also discard pins that are analog exclusive (20(A6), 21(A7))

// Digital pins
#define dig_lo_min 3
#define dig_lo_max 9
#define dig_hi_min 13
#define dig_hi_max 19

// Analog pins
#define analog_a0 0
#define analog_a7 7
#define analog_min 14
#define analog_max 21

// PWM pins
#define pwm_3 3
#define pwm_5 5
#define pwm_6 6
#define pwm_9 9

// Expander pins
#define exp_lo_min 0
#define exp_lo_max 7
#define exp_hi_min 10
#define exp_hi_max 17
