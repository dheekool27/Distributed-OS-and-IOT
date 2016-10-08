#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

syscall sendMsg(pid32 pid,umsg32 msg)

{
           umsg32 umsg;
           intmask mask;
           struct procent *prptr;
           mask = disable();
           if (isbadpid(pid)){          
                   restore(mask);
                   return SYSERR;
            }
            prptr = &proctab[pid];

            umsg=msg;

               if((((prptr->tail)+1)%10 == prptr->head) || (prptr->tail==MAX_MSGS-1 && prptr->head==-1) )
               {
                restore(mask);
                return SYSERR;
              }
            

                            prptr->tail=((prptr->tail)+1)% 10;

                            prptr->msgbuffer[prptr->tail]=umsg;
                            
                            if(prptr->prstate == PR_RECV)
                              {
                                ready(pid);
                               }

                restore(mask);
                return OK;

}

uint32  sendMsgs(pid32 pid,umsg32* msgs,uint32 msg_count)

{          uint32 success;
           uint32 n=msg_count;
           intmask mask;
           struct procent *prptr;
           mask = disable();
           if (isbadpid(pid)){          
                   restore(mask);
                   return SYSERR;
            }
            prptr = &proctab[pid];

            
          if(((MAX_MSGS-1-(prptr->tail)+(prptr->head))%MAX_MSGS )< msg_count)
                          success=MAX_MSGS-1-((prptr->tail)-(prptr->head));
             else
                            success=msg_count;

               if((((prptr->tail)+1)%10 == prptr->head)||(prptr->tail==MAX_MSGS-1 && prptr->head==-1))
               {
                restore(mask);
                return SYSERR;
              }
              
              n=0;
                           while(((prptr->tail)+1)%10 != prptr->head && n<msg_count)
                           {

                            prptr->tail=((prptr->tail)+1)% 10;

                            prptr->msgbuffer[prptr->tail]=msgs[n];
n++;                            
} 

                            if(prptr->prstate == PR_RECV)
                              {
                                ready(pid);
                               }
              restore(mask);
             return success%MAX_MSGS;
}
 uint32 sendnMsg(uint32 pid_count, pid32* pids, umsg32 msg)
{
intmask mask;
struct procent *prptr;
uint32 success = 0,i;
pid32 pid;

mask = disable();

resched_cntl(DEFER_START);
for(i=0;(i < pid_count) && (i < NPROC); i++)
{
pid = pids[i];
if(!isbadpid(pid))
{
prptr = &proctab[pid];

if(((prptr->tail+1)%MAX_MSGS)!=prptr->head)
{prptr->tail=((prptr->tail)+1)%MAX_MSGS;


if(prptr->prstate==PR_RECV)
{ 
ready(pid); // Ready the process as expected count is acheived
}	
success++;
}
//else
//{
//kprintf("message not sent to Process %d \n",pid);
//}
}
}

resched_cntl(DEFER_STOP);
restore(mask);
return success;	
}
