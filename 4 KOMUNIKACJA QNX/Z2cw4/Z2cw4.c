#include <stdlib.h>
#include <stdio.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <process.h>
#include <errno.h>

#define SIZE 24

 typedef struct {
        int  typ;            // Typ komunikatu
        int  od;              // Numer procesu
        char tekst[SIZE];    // Tekst komunikatu
 } kom_t;


 //jest problem jesli argv[2]> argv[1]
int main(int argc, char *argv[]) {
	int server_chid;
	int server_pid ;
	int flag = 0;

	int counter = 0;


	   kom_t msg;

	    kom_t rmsg;




		int rcvid;



		        int iloscProc = atoi(argv[1]);
		        int iloscKomunikatow = atoi(argv[2]);

		        int licznik = 0;
		        int pid = getpid();
		        printf("getpid = %d", pid);
		        int status;



		        for ( ; licznik < iloscProc && pid != 0; licznik++ ){


		        	if(flag == 0){

			        	server_pid = getpid();
			        	printf("i set server_pid : %d\n", server_pid);

			        	server_chid = ChannelCreate(0); //PUT CODE HERE to create a channel
			        			if(-1 == server_chid) {                //was there an error creating the channel?
			        				perror("ChannelCreate()");  //look up the errno code and print
			        				exit(EXIT_FAILURE);
			        			}

			        printf("i create channel, id : %d\n", server_chid);
			        			flag = 1;


		        	}

		        	pid = fork();

		           printf("licznik : %d\n", licznik);





		        }

		        kom_t serverReply;
		        if(pid != 0) {

		        //nie wchodzi do tego else!
		        printf("in pid=! 0\n");

		        		        	while(1) {
		        		        //	if(counter == iloscKomunikatow ){
		        		        	//		break;
		        		        	//       }
		        		        	        printf("parent proces try to receive msg from channel : %d\n",server_chid);
		        		        			rcvid = MsgReceive(server_chid, &rmsg, sizeof(rmsg), NULL); 	//PUT CODE HERE to receive msg from client
		        		        			if(rcvid == -1) {            //was there an error receiving msg?
		        		        				perror("MsgReceive");     //look up errno code and print
		        		        				break;                    //try receiving another msg
		        		        			}
		        		        			printf("------msgReceive from %d, type : %d, and text : %s\n" ,rmsg.od, rmsg.typ, rmsg.tekst);




		        		        			status = MsgReply(rcvid, EOK, &serverReply, sizeof(serverReply) );//PUT CODE HERE TO reply to client with checksum

		        		        			//if(-1 == status) {
		        		        			//	perror("MsgReply");
		        		        		//	}
		        	     		        	   if(rmsg.typ == 0){

		        	     		        		  counter++;
		        	     		        		  printf("\ncounter type 0 = %d\n", counter);



				        		        			if(counter == iloscKomunikatow ){
				        		        				printf("break");
				        		        				ChannelDestroy(server_chid);
				        		        				break;
				        		        		       }


		        			        		    }





		        		        		}

		        		        	//sleep(2);
		        		        	printf("!!!!!parent process id :%d, his child: %d, parent id: %d\n ",getpid(), pid, getppid() );
		        }






int once = 0;
int i =0;
kom_t servResp;
		        if(pid == 0){
		        	int coid;         //Connection ID to server

		        	for(i = 0; i <  iloscKomunikatow; i++){


		        	sleep(1);


		        	//tutaj logika procesow potomnych
                     printf("child process id:%d his child id : %d, parent id: %d\n ",getpid(), pid, getppid() );
if(once == 0){
                     printf("attempting to establish connection with server pid: %d, chid %d\n", server_pid, server_chid);


                     coid = ConnectAttach(ND_LOCAL_NODE, server_pid, server_chid, _NTO_SIDE_CHANNEL, 0);
                     	if(-1 == coid) {   //was there an error attaching to server?
                    		perror("ConnectAttach"); //look up error code and print
                     		exit(EXIT_FAILURE);
                     	}
                     //	printf("connect attach, id = %d", coid);
                     	once = 1;
}




                     	msg.od = getpid();
                     	msg.typ = 1;//czyli ostatnie wiadomosc
                     	if(i == iloscKomunikatow - 1){
                     		msg.typ = 0;
                     	}
              //       	char *komunikat = "Komunikat od procesu";
                     	strcpy(msg.tekst, "komunikat od procesu ");
                        printf("->sending message nr %d, type: %d from %d ",i , msg.typ ,msg.od);
                     	status = MsgSend(coid, &msg, sizeof(msg), &servResp, sizeof(servResp) );
                     		if(-1 == status) {   //was there an error sending to server?
                     			perror("MsgSend");
                     			exit(EXIT_FAILURE);
                     		}



                     //		printf("received checksum=%d from server\n", rmsg.typ);
                     		printf("MsgSend return status: %d\n", status);
                     	}


		        	//sleep(2);
		        	printf("now exit process %d ",getpid());
                     		exit(getpid());

		        }

		        if(counter == iloscKomunikatow){
		        	printf("???czekam na exit\n");
		        while((pid=wait(&status))!=-1)
		        printf("\nProces %d zakonczony, status %d\n",pid,WEXITSTATUS(status));
		       //WIFEXITED zwraca 1 wiec jest poprawnie zakonczony
		        }


		        return EXIT_SUCCESS;
		}


