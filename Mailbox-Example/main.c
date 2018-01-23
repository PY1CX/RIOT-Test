/*
 *   RIOT-OS Mailbox usage
 *   Learning how to RIOT
 *   Author: Felipe Navarro
 *   Date  : 23/01/2018
 *
 *   Description: This file shows basic usage of the Mailbox module of RIOT-OS
 *   using blocking mode.
 *
 */

#include <inttypes.h>
#include <stdio.h>

#include "thread.h" 
#include "xtimer.h"
#include "mbox.h"

static kernel_pid_t rcv_pid;
static char rcv_stack[THREAD_STACKSIZE_DEFAULT + THREAD_EXTRA_STACKSIZE_PRINTF];

mbox_t m;
 
static void *rcv(void *arg)
{
    msg_t msg;
 
    (void)arg;

    while (1)
    {   
        mbox_get(&m, &msg);    
        printf("Received %i \n", msg.content.value);
    }

    return 0;
}
 
int main(void)
{
    msg_t msg;
 
    msg.content.value = 0;
    rcv_pid = thread_create(rcv_stack, sizeof(rcv_stack),
                                THREAD_PRIORITY_MAIN, 0, rcv, NULL, "rcv");

    /*
    * Init the Mailbox M (first argument) with the queue rcv_queue with size RCV_QUEUE_SIZE
    */
    mbox_init( &m , &msg, 8);

    while (1)
        {
            mbox_put( &m, &msg);   
            msg.content.value++;
            printf("Sent %i \n", msg.content.value);
            xtimer_sleep(1);
        }
    return 0;
}
