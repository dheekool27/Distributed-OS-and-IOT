#include <xinu.h>

sid32 sem_send,sem_recv;


process sender(pid32 p,umsg32 m)
{
wait(sem_send);
kprintf("sending %d to %d\n",m,p);
if(sendMsg(p,m))
kprintf("Message Sent Succesfully to process:%d\n",p);
else
kprintf("Error in sending message");

signal(sem_recv);
}

process receiver()
{
//umsg32 msg1;
wait(sem_recv);
kprintf("in receiver\n");

kprintf("received message is :%d\n",receiveMsg());
kprintf("leaving receiver\n");
signal(sem_send);
}

process multiplemsgsreceiver(umsg32* msg,uint32 msg_count)
{
int i=0;
wait(sem_recv);
kprintf("in multiplemsgs receiver\n");
receiveMsgs(msg,msg_count);

for(i=0;i<msg_count;i++)
{
 kprintf("Received msg %d:%d\n",i,msg[i]);
} 
signal(sem_send);}


process multiplemsgssender(pid32 p,umsg32* msg,uint32 msg_count)
{
uint32 i=0;
wait(sem_send);
kprintf("in multiplemsgssender\n");
  for(i=0;i<msg_count;i++)
  { 
   kprintf("msgs in sender:%d\n",msg[i]);
  }
kprintf("number succesfully sent:%d\n",sendMsgs(p,msg,msg_count));
signal(sem_recv);
}

process msgsendertomultiple(uint32 pid_count, pid32* pids,umsg32 msg)
{
uint32 i=0;
wait(sem_send);
kprintf("sending %d to %d\n",msg,pids[0]);
kprintf("number of processes succesfully sent to:%d\n",sendnMsg(pid_count,pids,msg));

kprintf("leaving multiple sender\n");
signal(sem_recv);

}
process  main(void)
{              umsg32 msg;

pid32 pid1,pid2;
               
               kprintf("The main has started\n");
               umsg32 msgs[2];
umsg32 recvd_msgs[2];
               msg=1; 
               msgs[0]=2;
               msgs[1]=3;
               pid32 pid_list[2];

    
                resched_cntl(DEFER_START);
                sem_send=semcreate(1);
                sem_recv=semcreate(0);
              

pid1=create(receiver,4096,20,"receiver",0); 
         pid_list[0]=pid1;
pid2=create(sender,4096,20,"sender1",0,pid1,msg);
  
resume(pid1);
resume(pid2);
      resched_cntl(DEFER_STOP);
                return OK;
}
