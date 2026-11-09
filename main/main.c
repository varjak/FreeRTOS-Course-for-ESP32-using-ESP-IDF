#include <stdio.h>
#include "./main/Day_1_Day_2_Introduction_to_RTOS_and_FreeRTOS/day1and2.h"
#include "./main/Day_3_Scheduling_and_Core_Affinity/day3.h"
#include "./main/Day_4_Creating_and_Deleting_Tasks_in_FreeRTOS/day4.h"
#include "./main/Day_5_Task_States_and_Priorities_in_FreeRTOS/day5.h"
#include "./main/Day_6_Using_vTaskDelay_and_vTaskDelayUntil/day6.h"
#include "./main/Day_7_Blinking_Two_LEDs_with_Two_Tasks/day7.h"
#include "./main/Day_8_Monitoring_Queue_Usage/day8.h"
#include "./main/Day_9_Using_Queues_with_ISRs/day9.h"
#include "./main/Day_10_Binary_Semaphores/day10.h"
#include "./main/Day_11_Counting_Semaphores/day11.h"
#include "./main/Day_12_Mutexes_and_Recursive_Mutexes/day12.h"
#include "./main/Day_13_Avoiding_Priority_Inversion/day13.h"
#include "./main/Day_14_Hands_on_Mini_Project_UART_Logger_with_Mutex/day14.h"
#include "./main/Day_15_Event_Groups/day15.h"

#include "./main/Day_17_Stream_Buffers_and_Message_Buffers/day17.h"
#include "./main/Day_18_Software_Timers/day18.h"
#include "./main/Day_19_Memory_Management_in_FreeRTOS/day19.h"
#include "./main/Day_20_Interrupt_Management_in_FreeRTOS/day20.h"
#include "./main/Day_21_Idle_Task_and_Daemon_Task/day21.h"
#include "./main/Day_22_Tick_Rate_Tick_Interrupt_and_Tickless_Idle/day22.h"
#include "./main/Day_23_Debugging_FreeRTOS_Applications/day23.h"
#include "./main/Day_24_Power_Management_with_FreeRTOS_on_ESP32/day24.h"
#include "./main/Day_25_Inter_Task_Communication_Putting_It_All_Together/day25.h"
#include "./main/Day_26_Real_Time_Debugging_with_FreeRTOS_Trace/day26.h"
#include "./main/Day_27_Handling_Errors_and_Watchdogs/day27.h"
#include "./main/Day_28_Real_Time_Performance_Optimizations/day28.h"
#define DAY 22


void app_main() {
    switch (DAY) {
        case 1: case 2: day1and2_run(); break;
        case 3: day3_run(); break;
        case 4: day4_run(); break;
        case 5: day5_run(); break;
        case 6: day6_run(); break;
        case 7: day7_run(); break;
        case 8: day8_run(); break;
        case 9: day9_run(); break;
        case 10: day10_run(); break;
        case 11: day11_run(); break;
        case 12: day12_run(); break;
        case 13: day13_run(); break;
        case 14: day14_run(); break;
        case 15: day15_run(); break;
        case 17: day17_run(); break;
        case 18: day18_run(); break;
        case 19: day19_run(); break;
        case 20: day20_run(); break;
        case 21: day21_run(); break;
        case 22: day22_run(); break;
        case 23: day23_run(); break;
        case 24: day24_run(); break;
        case 25: day25_run(); break;
        case 26: day26_run(); break;
        case 27: day27_run(); break;
        case 28: day28_run(); break;
        default: printf("Could not find code to run for specified day.");
    }
}
