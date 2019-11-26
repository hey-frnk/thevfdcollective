/**
  ******************************************************************************
  * @file     vfdco_time.h
  * @author   The VFD Collective, Frank from The VFD Collective (Fu Zheng)
  * @version  V1.0
  * @date     26-November-2019
  * @brief    This is the dummy driver to simulate time functionality
  ******************************************************************************
  * @toc      Table of contents, enter to navigate:
  ******************************************************************************
 **/

typedef struct time_event_t{
  unsigned long interval;
  unsigned long previousTime;
} time_event_t;

// Create new timer (updates every p1 milliseconds)
struct time_event_t Time_Event_Init(long interval);

// Reset timer milliseconds to 0
void Time_Event_Reset(time_event_t *input);

// Check if p1 has elapsed. If yes, return 1, else return 0
unsigned char Time_Event_Update(time_event_t *input);
