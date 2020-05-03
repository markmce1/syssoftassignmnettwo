#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>


pthread_mutex_t locker;

int main(int argc, char *argv[])
{
    int iret1;
    //pthread_t thread1;
    int s;
    int cs;
    int connSize;
    int READSIZE;
    struct sockaddr_in server, client;

    //create socket
    s=socket(AF_INET, SOCK_STREAM,0);
    if(s==-1)
    {
        printf("could not create socket\n");
    }else{
        printf("Socket succesfully created\n");
    }

    //set sockaddr_in variables
    server.sin_port = htons( 8081 ); // SET THE PROT FOR COMMS
    server.sin_family = AF_INET; //USE IPV4
    server.sin_addr.s_addr = INADDR_ANY;
    //WHEN IDADDR_ANY IS SPECIFIED IN THE BIND CALL, THE
    //SOCKET WILL BE BOUND TO ALL LOCAL INTERNFACES

    //bind
    if( bind(s,(struct  sockaddr *)&server, sizeof(server))<0)
    {
        perror("Bind issue!\n");
        return 1;
    }else{
        printf("Bind complete\n");
    }
    //listen for a connection
    listen(s,3);
    //accept and incoming connection
    printf("waiting for incoming connection from client>> ");
    connSize=sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    cs = accept(s,(struct sockaddr *)&client, (socklen_t*)&connSize);
    if ( cs < 0)
    {
        perror("can't establish connection");
        return 1;
    }
    else{
        printf("connection from client accepted");
    }
    //my attempt at multi threading
    /*while((cs = accept(s,(struct sockaddr *)&client, (socklen_t*)&connSize))){
        puts("Connection accepted");
        if(pthread_create(&thread1, NULL, filereceiver, (void*) &cs)<0){
            perror("could not create thread");
            return 1;
        }
        else {
            printf("success");
        }
        pthread_join(thread1, NULL);
        puts("Handler assigned");
    }*/
     pthread_mutex_lock(&locker);

    //receieving files
    FILE *fp;
    int myUID =0; 
    char cuid[5];
    char cgid[5];
    char buffer[1024];
    char file_name[255];
    char enteredfilename[255]; 
    read(cs, cuid, 5);//take in from client 1
    read(cs, cgid, 5);//2
    read(cs, file_name, 255);//3
    read(cs,enteredfilename,255);//4
    fp = fopen(file_name, "a");

    uid_t gid,uid;

    printf("\nthis is gid: %s",cgid);
    printf("\nthis is uid: %s\n",cuid);

    uid = atoi(cuid);
    gid = atoi(cgid);

    if(setregid(gid,myUID)<0)
    {
        printf("%s: cannot change euid\n");
    }

    if(setegid(gid)<0)
    {
        printf("%s: cannot change euid\n");
    }

    if(setreuid(uid, myUID)<0)
    {
        printf("%s: cannot change euid\n");
    }
    if(seteuid(uid)<0)    
    {
        printf("%s: cannot change euid\n");
    }
    

    int i;
    i = getegid();
    printf("\nthis is gid: %d",i);
    i=geteuid();
    printf("\nthis is uid: %d\n",i);
    

    char st[255] = "cp ";
    strcat(st, enteredfilename);
    strcat(st,file_name);
    system(st);
    printf("The file has been received\n");
    pthread_mutex_unlock(&locker);

    close(cs);
    close(s);

   
    if(READSIZE ==0)
    {
        puts("Client disconnected\n");
        fflush(stdout);
    }
    else if(READSIZE == -1)
    {
        perror("Read error");
    }
    return 0;

}
