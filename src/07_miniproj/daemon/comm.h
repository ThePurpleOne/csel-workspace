#ifndef COMM_H
#define COMM_H

int  comm_init(int port);
void comm_close(int epfd);

int  comm_listen_fd(void);
int  comm_accept_client(int epfd);

int  comm_owns_client(int fd);
void comm_handle_client(int fd, int epfd);

#endif
