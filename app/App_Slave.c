//
// Created by zramd on 13/05/2026.
//

#include "App.h"
#include "../Mcal/Button/Button.h"
#include "../Mcal/RCC/RCC.h"
#include "../Mcal/Usart/Usart.h"
#include "../Motor/Motor.h"
#include "../Mcal/Timer/Timer.h"
#include "../Mcal/IPC/Ipc.h"
#include "../Mcal/Spi/Spi.h"

static volatile Ipc_Packet_t my_elevator_state;
static volatile boolean my_pending_requests[5] = {FALSE, FALSE, FALSE, FALSE, FALSE};
static uint16 door_timer = 0;

static void App_Slave_Run_FSM(void) {
    if (g_Emergency_Flag) {
        my_elevator_state.state = ELEV_EMERGENCY;
        Motor_SetSpeed(MOTOR_REST);
        return;
    }

    my_elevator_state.current_floor = g_Current_Floor_Sensor;

    switch (my_elevator_state.state) {
        case ELEV_IDLE:
            for (int i=1; i<=4; i++) {
                if (my_pending_requests[i]) {
                    if (i == my_elevator_state.current_floor) {
                        my_pending_requests[i] = FALSE;
                        my_elevator_state.state = ELEV_DOORS_OPEN;
                        door_timer = 0;
                    } else if (i > my_elevator_state.current_floor) {
                        my_elevator_state.state = ELEV_MOVING_UP;
                        Motor_SetSpeed(MOTOR_HIGH_SPEED);
                    } else {
                        my_elevator_state.state = ELEV_MOVING_DOWN;
                        Motor_SetSpeed(MOTOR_HIGH_SPEED);
                    }
                    break;
                }
            }
            break;

        case ELEV_MOVING_UP:
            if (my_pending_requests[my_elevator_state.current_floor]) {
                my_pending_requests[my_elevator_state.current_floor] = FALSE;
                Motor_SetSpeed(MOTOR_REST);
                my_elevator_state.state = ELEV_DOORS_OPEN;
                door_timer = 0;
            }
            break;

        case ELEV_MOVING_DOWN:
            if (my_pending_requests[my_elevator_state.current_floor]) {
                my_pending_requests[my_elevator_state.current_floor] = FALSE;
                Motor_SetSpeed(MOTOR_REST);
                my_elevator_state.state = ELEV_DOORS_OPEN;
                door_timer = 0;
            }
            break;

        case ELEV_DOORS_OPEN:
            my_elevator_state.door_status = 1;
            door_timer++;
            if (door_timer >= 40) { // 2 seconds
                my_elevator_state.door_status = 0;
                my_elevator_state.state = ELEV_IDLE;
            }
            break;
        default:
            break;
    }
}

// 50ms Timer Tick for Slave
static void App_Slave_50ms_Tick(void) {
    // 1. Process last rx_packet if valid
    if (spi_transfer_complete) {
        ENTER_CRITICAL();
        spi_transfer_complete = 0;
        EXIT_CRITICAL();

        if (rx_packet.header == IPC_HEADER_BYTE && rx_packet.checksum == IPC_CalculateChecksum(&rx_packet)) {
            // Master assigned us a floor?
            if (rx_packet.target_floor > 0 && rx_packet.target_floor <= 4) {
                my_pending_requests[rx_packet.target_floor] = TRUE;
            }
        }
    }

    // 2. Add local cabin requests to my_pending_requests
    for(int i=1; i<=4; i++) {
        if(g_CabinRequests[i]) {
            my_pending_requests[i] = TRUE;
            g_CabinRequests[i] = FALSE;
        }
    }

    // 3. FSM Update
    App_Slave_Run_FSM();

    // 4. Pre-load SPI register for the NEXT Master transaction
    Spi2_Slave_Preload((Ipc_Packet_t*)&my_elevator_state);

    // 5. Telemetry output
    if (my_elevator_state.state == ELEV_EMERGENCY) {
        Usart1_TransmitString_Async("SLAVE: EMERGENCY!\r\n", 19);
    } else {
        Usart1_TransmitString_Async("SLAVE: STATUS OK\r\n", 18);
    }
}

void App_Slave_Init(void) {
    Rcc_Init();
    Usart1_Init();
    Button_Init();
    Motor_Init();
    Spi2_Init_Slave(); // Initialize as SLAVE

    my_elevator_state.state = ELEV_IDLE;
    my_elevator_state.current_floor = 4; // Start on top floor

    Spi2_Slave_Preload((Ipc_Packet_t*)&my_elevator_state);

    // Timer 3: FSM Scheduler (50ms)
    Timer_Init(TIM_INSTANCE_3, TIM_PRESCALER_1MS_TICK, 49);
    Timer_StartPeriodic(TIM_INSTANCE_3, 50, App_Slave_50ms_Tick);

    Usart1_TransmitString("Elevator Slave Node Initialized. FSM Scheduler running @ 50ms\r\n");
}

void App_Slave_Run(void) {
    // Empty - Handled by Timer 3 and EXTI Interrupts!
}