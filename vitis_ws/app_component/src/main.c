#include "xparameters.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xil_types.h"
#include <stdio.h>
 
// Get device IDs from xparameters.h
#define BTN_ID XPAR_AXI_GPIO_BUTTONS_BASEADDR
#define LED_ID XPAR_AXI_GPIO_LED_BASEADDR
#define BTN_CHANNEL 1
#define LED_CHANNEL 1
#define BTN_MASK 0b1111
#define LED_MASK 0b1111

 
int main() {
	XGpio_Config *cfg_ptr;
	XGpio led_device, btn_device;
	u32 data;
 
	xil_printf("Entered function main\r\n");
 
	// Initialize LED Device
	cfg_ptr = XGpio_LookupConfig(LED_ID);
	XGpio_CfgInitialize(&led_device, cfg_ptr, cfg_ptr->BaseAddress);
 
	// Initialize Button Device
	cfg_ptr = XGpio_LookupConfig(BTN_ID);
	XGpio_CfgInitialize(&btn_device, cfg_ptr, cfg_ptr->BaseAddress);
 
	// Set Button Tristate
	XGpio_SetDataDirection(&btn_device, BTN_CHANNEL, BTN_MASK);
 
	// Set Led Tristate
	XGpio_SetDataDirection(&led_device, LED_CHANNEL, 0);
 
    const char8 * hello = "Hi\n";

    long long int clock_split_short = 200000;
    long long int clock_split_long = 600000;



    long long int current_split = 0;


    int previous_state = 0;
    int current_state = 0;

    int flag_short = 0;
    int flag_long = 0;

    int counter = 0;


	while (1) {
        current_split += 1;

		data = XGpio_DiscreteRead(&btn_device, BTN_CHANNEL);
		data &= BTN_MASK;
		
		if (data != 0) {
            current_state = 1;
             if (current_split > 10 && current_split < clock_split_long && current_state != previous_state){
                data = LED_MASK;
                //printf(hello);
                //printf("Short press　%lld\n", current_split);
                //if (current_state != previous_state){
                // current_split = 0;
                //}
                //printf("Current state: %d Previous state: %d\n", current_state, previous_state);
                flag_short = 1;
                counter += 1;
                   
             } else if (current_split > clock_split_short && current_split > clock_split_long){
                //printf("Long press %lld\n", current_split);
                //printf("Current state: %d Previous state: %d\n", current_state, previous_state);  
                 current_split = 0;
                flag_long = 1;
                counter += 1;
             } 
                
        } else {
            data = 0;
            current_state = 0;
            
            if (counter > 1){
                printf("I\n");
            }
            else if (counter == 1){
                printf("o\n");
            }
            counter = 0;
            //previous = false;
            if (current_split >= clock_split_short){
                current_split = 0;
            }
            
        }
            
        XGpio_DiscreteWrite(&led_device, LED_CHANNEL, data);
        previous_state = current_state;
        
	}
}