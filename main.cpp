// USB host keyboard and LCD demo

#include "mbed.h"
//#include "rtos.h"
#include "USBHostKeyboard.h"
#include "uLCD_4DGL.h"
//#include "USBHostKeyboard.h"

// LED to demonstrate multi-threading
DigitalOut led(LED1);

// Graphic LCD - TX, RX, and RES pins
uLCD_4DGL uLCD(p9,p10,p11);

// Callback function from thread
void onKey(uint8_t key) {
    led = !led;
    uLCD.printf("%c", key);
}

void keyboard_task(void const *) {
    USBHostKeyboard keyboard;
    
    while(1) {
        
        // Try to connect a USB keyboard
        uLCD.printf("Waiting...\n");
        while(!keyboard.connect()) {
            Thread::wait(500);
        }
        uLCD.printf("Connected!\n");
        
        // When connected, attach handler called on key board event
        keyboard.attach(onKey);
        
        //Wait until the keyboard is disconnected
        while(keyboard.connected()) {
            Thread::wait(500);
        }
        uLCD.printf("\nDisconnected!\n");
    }
}

int main() {
    
    // Initialize LCD
    uLCD.baudrate(115200);
    uLCD.background_color(BLACK);
    uLCD.cls();
    uLCD.locate(0,0);
    
    // Create a thread that runs a function (keyboard_task)
    
    Thread keyboardTask(keyboard_task, NULL, osPriorityNormal, 256 * 4);
    
    // Flash an LED forever
    while(1) {
        //led=!led;
        Thread::wait(500);
    }
}



