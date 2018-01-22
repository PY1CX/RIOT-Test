#include <stdio.h>
#include <string.h>

#include "msg.h"
#include "shell.h"

#include "net/af.h"
#include "net/protnum.h"
#include "net/ipv6/addr.h"
#include "net/sock/udp.h"
#include "fmt.h"
#include "xtimer.h"

static const shell_command_t shell_commands[] = {
    { NULL, NULL, NULL }
};

uint8_t buf[128];

static char stack[THREAD_STACKSIZE_DEFAULT];

void *lopback_task(void *arg)
{   
    (void) arg;

    sock_udp_ep_t local = SOCK_IPV6_EP_ANY;
    sock_udp_t sock;
    local.port = 12345; //from struct sock_udp_ep_t local

    if (sock_udp_create(&sock, &local, NULL, 0) < 0) {
        puts("Error creating UDP sock");
    }
    while (1) {
        sock_udp_ep_t remote;
        ssize_t res;
        if ((res = sock_udp_recv(&sock, buf, sizeof(buf), SOCK_NO_TIMEOUT,
                                 &remote)) >= 0) {
            buf[res] = '\0';
            printf("Received: %s\n", buf);
            
            xtimer_sleep(1);
            
            if (sock_udp_send(&sock, buf, res, &remote) < 0) {
                puts("Error sending reply");
            }
            else    
            {   
                puts("Reply sent");
            }
        }
    }

    return 0;
    
}

int main(void)
{
    
    thread_create(stack, sizeof(stack),
                        THREAD_PRIORITY_MAIN - 1,
                        THREAD_CREATE_STACKTEST,
                        lopback_task,
                        NULL, "Loopback UDP");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(shell_commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
