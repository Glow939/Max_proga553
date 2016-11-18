
#include <string.h>					
#include <stdio.h>//					
#include <stdlib.h>//					
#include <sys/types.h>//				
#include <sys/ipc.h>					
#include <sys/msg.h>					

int main(int argc,char* argv[],char* envp[]) {
	
	int desc; // descriptor  
	key_t key = 227; // key

	int i = 0, ans; 

	
	struct mymsgbuf {
		long mestyp;//type
		int a,b;  
		int ppid;//pid клиента
	} clnt;

	
	if((desc = msgget(key, 0666 | IPC_CREAT)) < 0) { //доступ к пам€ти
		printf("key error\n");
		exit(-1);
	}
		
		clnt.a = atoi(argv[1]);
		clnt.b = atoi(argv[2]);
		clnt.mestyp = 1;
		clnt.ppid = getpid(); 

		
		if (msgsnd(desc,(struct msgbuf *) &clnt, 16, 0) < 0){ // отправл€ем {
			printf("send error\n");
			msgctl(desc, IPC_RMID, (struct msqid_ds*)NULL);
			exit(-1);
		}
		printf("Sent. \n");
		
		if  (msgrcv(desc,(struct msgbuf *) &clnt,16 ,clnt.ppid + 1, 0) < 0) //ждем ответа
			{
				printf("receive error \n");
				msgctl(desc, IPC_RMID, (struct msqid_ds*)NULL);
				exit(-1);
			}
		if (clnt.ppid > 0) {
			ans = clnt.a; 
			printf(" result is %d \n",ans);
			} 
		else printf("server cant answer now \n");
		return 0;
} 
