#include <xinu.h>

sid32 sem_send,sem_recv;


process sender(pid32 p,umsg32 m)
{
wait(sem_send);
kprintf("sending %d to %d\n",m,p);
if(sendMsg(p,m))
kprintf("Message Sent Succesfully to process:%u\n",p);
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
               umsg32 msg1;
pid32 pid1,pid2,pid3,pid4,pid5,pid6;
               uint32 i=0; 
               kprintf("The main has started\n");
               umsg32 msgs[2];
umsg32 recvd_msgs[2];
               msg=1; 
               msgs[0]=2;
               msgs[1]=3;
               pid32 pid_list[2];

pid_list[1]=pid4;
    
                resched_cntl(DEFER_START);
                sem_send=semcreate(1);
                sem_recv=semcreate(0);
              
           pid6=create(msgsendertomultiple,4096,20,"msgnsender1",3,2,&pid_list,msg);           
               pid5=create(multiplemsgssender,4096,20,"nmsgsender2",3,pid4,&msgs,2);
               pid4=create(multiplemsgsreceiver,4096,20,"nmsgreceiver1",2,&msg1,2);
               pid1=create(multiplemsgsreceiver,4096,20,"nmsgreceiver2",2,&msg1,2);
               resume(pid6);
               resume(pid5);
               resume(pid4);
               resume(pid1);
      resched_cntl(DEFER_STOP);
                return OK;
}
