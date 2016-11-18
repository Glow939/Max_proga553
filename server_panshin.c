
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mymsgbuf {
   		long mestyp;
 		int a,b; 
		int ppid;
    	} gendata;

int num = 0; // ����� �����

pthread_t thread_id[2]; // ������ ���������������
int n = 0; // ������ thread_id
pthread_mutex_t lock; 

key_t key = 227; // key


void* my_thread(void* arg) {
	//pthread_mutex_lock(&lock);
	
	struct mymsgbuf result = *(struct mymsgbuf*)arg;
	int j = result.mestyp;

	//pthread_mutex_unlock(&lock);
	sleep(result.a + result.b);
	result.a += result.b;

	result.mestyp = result.ppid + 1;
	int desc; // descriptor   
  
	
	if((desc = msgget(key, 0666 | IPC_CREAT)) < 0) { //������ � ������
			printf("key error\n");
			exit(-1);
		}
	msgsnd(desc,(struct msgbuf *) &result,16,0); // ����������	
  	printf("Result is sent to %d. \n",result.ppid);

    	thread_id[j] = 0;
	num--; //���� ����������� 
    	return NULL;
}


int main() {
 
	int desc; //descriptor   
  
         if((desc = msgget(key, 0666 | IPC_CREAT)) < 0) { // ������ � ������
			printf("key error\n");
			exit(-1);
       		}
        int i; 
	thread_id[0] = 0;
	thread_id[1] = 0;	
	pthread_mutex_init(&lock, NULL);

	/* ��������� ������*/
	while(1) {
			if (msgrcv(desc,(struct msgbuf*) &gendata,16,1,0) < 0) 
				{
					printf(" receive error\n");
					msgctl(desc, IPC_RMID, (struct msqid_ds*)NULL); 
					exit(-1);
				}
			if (num < 2) {
				
				if (thread_id[0] == 0) n = 0; else n = 1; //�������� ��������� ����

				
				gendata.mestyp = n; 
      				i = pthread_create(&thread_id[n] ,(pthread_attr_t *)NULL ,my_thread ,&gendata);//���� ����� ������ n
      				if (i) {
          				printf("create error");
          				exit(-1);
      				}
  		
    				printf("message is received from %d\n",gendata.ppid);
				num++; 
			} else 
				{
					gendata.mestyp = gendata.ppid + 1;
					printf("server is busy\n");
					gendata.ppid = 0;
					msgsnd(desc,(struct msgbuf *) &gendata,16,0);
				}
		}
	pthread_mutex_destroy(&lock); 

    	return 0;
}






