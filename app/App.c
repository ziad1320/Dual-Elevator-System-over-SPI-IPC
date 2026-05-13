//
// Created by Khalaf on 16/04/2026.
//

#include "App.h"
#include "../Mcal/Button/Button.h"
#include "../Mcal/RCC/RCC.h"
#include "../Mcal/Usart/Usart.h"
#include "../Motor/Motor.h"
#include "../Mcal/Timer/Timer.h"
#include "../Mcal/IPC/Ipc.h"
#include "../Mcal/Spi/Spi.h"
#include "../Mcal/Dispatcher/Dispatcher.h"

// Shared Volatile Flags
static volatile Ipc_Packet_t my_elevator_state;
static volatile Ipc_Packet_t slave_state;

static volatile boolean my_pending_requests[5] = {FALSE, FALSE, FALSE, FALSE, FALSE};
static uint8 spi_timeout_counter = 0;
static boolean slave_offline = FALSE;
static uint16 door_timer = 0;

static void App_Run_FSM(void) {
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
            if (door_timer >= 40) { // 2 seconds (40 * 50ms)
                my_elevator_state.door_status = 0;
                my_elevator_state.state = ELEV_IDLE;
            }
            break;
        default:
            break;
    }
}

// Fired every 50ms by hardware timer
static void App_50ms_Tick(void) {
    // 1. Process last rx_packet if valid
    if (spi_transfer_complete) {
        ENTER_CRITICAL();
        spi_transfer_complete = 0;
        EXIT_CRITICAL();
        
        if (rx_packet.header == IPC_HEADER_BYTE && rx_packet.checksum == IPC_CalculateChecksum(&rx_packet)) {
            slave_state = rx_packet;
            spi_timeout_counter = 0; // Reset timeout
        }
    }

    // 2. SPI Fault Tolerance
    spi_timeout_counter++;
    if (spi_timeout_counter > 4) { // > 200ms
        slave_offline = TRUE;
    } else {
        slave_offline = FALSE;
    }

    // 3. Add local cabin requests to my_pending_requests
    for(int i=1; i<=4; i++) {
        if(g_CabinRequests[i]) {
            my_pending_requests[i] = TRUE;
            g_CabinRequests[i] = FALSE;
        }
    }

    // 4. Algorithm Update (Hallway calls)
    my_elevator_state.target_floor = 0; // Reset assignment target
    for(int floor=1; floor<=4; floor++) {
        if (g_HallwayUpRequests[floor] || g_HallwayDownRequests[floor]) {
            uint8 is_up = g_HallwayUpRequests[floor] ? 1 : 0;

            if (slave_offline || slave_state.state == ELEV_EMERGENCY) {
                my_pending_requests[floor] = TRUE;
                g_HallwayUpRequests[floor] = FALSE;
                g_HallwayDownRequests[floor] = FALSE;
            } else {
                uint8 winner = Dispatch_Call((Ipc_Packet_t*)&my_elevator_state, (Ipc_Packet_t*)&slave_state, floor, is_up);
                if (winner == 1) { // Master wins
                    my_pending_requests[floor] = TRUE;
                    g_HallwayUpRequests[floor] = FALSE;
                    g_HallwayDownRequests[floor] = FALSE;
                } else if (winner == 2) { // Slave wins
                    my_elevator_state.target_floor = floor; // Assign to Slave via SPI
                    g_HallwayUpRequests[floor] = FALSE;
                    g_HallwayDownRequests[floor] = FALSE;
                }
            }
        }
    }

    // 5. Run FSM Update
    App_Run_FSM();

    // 6. Kick off next SPI transaction
    my_elevator_state.header = IPC_HEADER_BYTE;
    my_elevator_state.checksum = IPC_CalculateChecksum((Ipc_Packet_t*)&my_elevator_state);
    Spi2_Start_Exchange((Ipc_Packet_t*)&my_elevator_state);

    // 7. Telemetry output
    if (my_elevator_state.state == ELEV_EMERGENCY) {
        Usart1_TransmitString_Async("MASTER: EMERGENCY!\r\n", 20);
    } else {
        Usart1_TransmitString_Async("MASTER: STATUS OK\r\n", 19);
    }
}

void App_Init(void) {
    Rcc_Init();
    Usart1_Init();
    Button_Init();
    Motor_Init();
    Spi2_Init_Master();

    Usart1_TransmitString("Hardware Initialized...\r\n");

    my_elevator_state.state = ELEV_IDLE;
    my_elevator_state.current_floor = 1;
    my_elevator_state.door_status = 0;

    // Timer 3: FSM Scheduler & SPI Communication (50ms)
    Timer_Init(TIM_INSTANCE_3, TIM_PRESCALER_1MS_TICK, 49);
    Timer_StartPeriodic(TIM_INSTANCE_3, 50, App_50ms_Tick);

    Usart1_TransmitString("Elevator Master Node Initialized. FSM Scheduler running @ 50ms\r\n");
}

void App_Run(void) {
    // Empty - Everything is handled by Timer 3 scheduler and EXTI Interrupts!
}