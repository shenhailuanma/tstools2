/* vim: set tabstop=8 shiftwidth=8: */
//=============================================================================
// Name: udp.h
// Purpose: UDP access
// To build: gcc -std-c99 -c udp.c
//=============================================================================

#ifndef _UDP_H
#define _UDP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>                  // for inet_ntoa(), inet_addr(), etc
#include <unistd.h>                     // for close()
#include <fcntl.h>                      // for fcntl(), O_NONBLOCK, etc
#include <sys/select.h>                 // for select(), etc

/*============================================================================
 * Struct Declaration
 ===========================================================================*/
typedef struct
{
        int sock;
        struct sockaddr_in remote;
}
UDP;

/*============================================================================
 * Public Function Declaration
 ===========================================================================*/
UDP *udp_open(char *addr, unsigned short port);
void udp_close(UDP *udp, char *addr);
size_t udp_read(UDP *udp, char *buf);

#ifdef __cplusplus
}
#endif

#endif // _UDP_H

/*****************************************************************************
 * End
 ****************************************************************************/
