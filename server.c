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
        perror("can't establish connection\n");
        return 1;
    }
    else{
        printf("connection from client accepted\n");
    }
    pthread_mutex_lock(&locker);
    //receieving files
    FILE *fp;
    uid_t uid;
    int myUID =1001;
    gid_t gid;
    int i = 1005;
    int ch =0;
    char buffer[1024];
    char file_name[300];
    //somehow read in file_name
    read(cs, file_name, 255);
    fp = fopen(file_name, "a");
    int words;
    read(cs, uid, sizeof(uid_t));//take in from client

    read(cs, gid, sizeof(gid_t));
    int geid, ueid;
    setreuid(myUID, uid);

    /*if(setreuid(myUID, uid)<0){
        printf("cannot change reuid");
        return 1;
    }
    if(setregid(myUID,gid)<0)
    {
        printf("cannot change regid");
        return 1;
    }
    if(seteuid(i)<0)
    {
        printf("cannot change euid");
        return 1; 
    }
    if(setegid(i)<0){
        printf("cannot change euid");
        return 1;
    }*/

    printf("%d\n", uid);
    printf("%d\n", gid);    
    geid = getegid();
    ueid = geteuid();
    printf("%d\n", ueid);
    printf("%d\n", geid);
    read(cs, &words, sizeof(int));

    while(ch !=words)
    {
        read(cs, buffer, 255);
        fprintf(fp, "%s", buffer);
        ch++;
    }
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
