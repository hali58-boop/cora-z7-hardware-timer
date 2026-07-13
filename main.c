#include <stdio.h> 
#include <stdint.h>
#include "xil_printf.h"

// Define memory addresses based on Vivado Block Design
volatile unsigned int *TIMER_CTRL   = (unsigned int *)0x43C00000; // slv_reg0 (Base Address)
volatile unsigned int *TIMER_COUNT_lo = (unsigned int *)0x43C00004; // slv_reg1 (+4 bytes)
volatile unsigned int *TIMER_COUNT_hi = (unsigned int *)0x43C00008; // slv_reg2 (+4 bytes)
volatile unsigned int *BUTTONS     = (unsigned int *)0x41200000; 

int main()
{
    // Send standard clear screen terminal command
    xil_printf("\033[2J\033[H"); 
    xil_printf("--- Cora Z7 Live Stopwatch ---\n\r");

    while(1) {
        // 1. Read the physical buttons
        unsigned int btn_state = *BUTTONS;
        uint32_t count_lo = *TIMER_COUNT_lo;
        uint32_t count_hi = *TIMER_COUNT_hi;

        // Shift the 'hi' bits 32 spaces to the left, and OR them with the 'lo' bits
        uint64_t total_ticks = ((uint64_t)count_hi << 32) | count_lo;
        // 2. Control the Timer based on the buttons
        if (btn_state == 1) {        // Button 0
            *TIMER_CTRL = 0;         // PAUSE
        } 
        else if (btn_state == 2) {   // Button 1
            *TIMER_CTRL = 1;         // START
        }
        else if (btn_state == 3) {   // Both Buttons
            *TIMER_CTRL = 2;         // RESET
        }

        // 3. Read the live timer hardware
        uint64_t current_ticks = total_ticks; 
        
        // 4. Do the math to get seconds
        uint64_t full_seconds = current_ticks / 50000000ULL;
        uint32_t nanoseconds = (current_ticks % 50000000ULL) * 20;
                
        // 5. Print it to the live terminal
        // Print 0.00000000 format
        printf("\033[2;1HLive Time: %llu.%08lu seconds\033[K", full_seconds, nanoseconds);
        fflush(stdout);
    }
    
    return 0;
}