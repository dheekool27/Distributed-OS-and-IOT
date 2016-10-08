#include<xinu.h>
/*------------------------------------------------------------------------

 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */

 umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
}


umsg32 receiveMsg(void)
{
           intmask mask;
           struct procent *prptr;
           umsg32 msg=3;
           mask = disable();
          

           prptr = &proctab[currpid];
            if(prptr->head == -1 && prptr->tail ==-1)
            {
             prptr->prstate=PR_RECV;
             resched();
           }
                       

         if(prptr->head==prptr->tail)
         { prptr->head=-1; 
	       prptr->tail=-1;}
         else
           {prptr->head=((prptr->head)+1)% MAX_MSGS;
        msg=prptr->msgbuffer[prptr->head];
}
         restore(mask);

                return msg;
}



syscall receiveMsgs(umsg32* msgs,uint32 msg_count)
{
                
           intmask mask;
           struct procent *prptr;
           
           mask = disable();
           uint32 n=msg_count;    

		prptr = &proctab[currpid];

//while((prptr->tail > prptr->head && ((prptr->tail)-(prptr->head) < msg_count)) || (prptr->tail < prptr->head && (MAX_MSGS+(prptr->tail)-(prptr->head))<msg_count))
 while((((prptr->tail) - (prptr->head) +  MAX_MSGS)%MAX_MSGS) < msg_count)
 {
prptr->prstate=PR_RECV;
resched();
}     


while(n>0){
           

		   if(prptr->tail ==-1)
            {
             prptr->prstate=PR_RECV;
             resched();
           }
            if(prptr->head==prptr->tail)
         { prptr->head=-1; prptr->tail=-1;}
          else
            prptr->head=((prptr->head)+1)% MAX_MSGS;

           msgs[prptr->head]=prptr->msgbuffer[prptr->head];

           n--;
}
    restore(mask);
return OK;
}
