#ifndef __JOYSTICK_COMMAND_H
#define __JOYSTICK_COMMAND_H

void joystick_initial(void);
void get_joystck_command(u8 data);
void joystick_command(char* usart_data);

#endif