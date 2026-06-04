#ifndef BUTTONS_H
#define BUTTONS_H

#define N_BUTTONS 3

int  buttons_init(void);
void buttons_close(void);

int  buttons_fd(int idx);
int  buttons_owns_fd(int fd);

void buttons_handle_event(int idx);

#endif
