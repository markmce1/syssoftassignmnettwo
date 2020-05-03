#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
//#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<ctype.h>

int main(int argc, char *argv[])
{
    int SID;
    uid_t uid = getuid();
    gid_t gid = getgid();

    printf("%d\n", uid); 

    printf("%d\n", gid); 

    struct sockaddr_in server;
    //Create socket
    SID =  socket(AF_INET, SOCK_STREAM, 0);
    if(SID == -1)
    {
        printf("Error creating socket");
    }else{
        printf("Socket created");
    }
    //set sockaddr_in variables
    server.sin_port =htons ( 8081 );//port to connect on
    server.sin_addr.s_addr=inet_addr("127.0.0.1");//server ip
    server.sin_family = AF_INET;//ipv4 protocol

    //connect to server
    if(connect(SID, (struct sockaddr *)&server, sizeof(server))<0)
    {
        printf("connect failed. Error");
        return 1;
    }

    printf("connected to server ok\n");
    char root[] ="Root/"; //remove name root as it will need to be in the root directory
    char sales[] = "Sales/";
    char promotions[] ="Promotions/";
    char offers[] = "Offers/";
    char marketing[] ="Marketing/";
    ///sending file craic
    FILE *f;
    int words =0;
    char buffer[1024];
    char c;
    int serverpath;
    char enteredfilename[200];
    char file_name[300];
    printf("Whats the file name?\n");
    scanf("%s", enteredfilename);
    
    do{
        printf("\n Enter the number of the file destinations\n1. Root\n2. Sales\n3. Promotions\n4. Offers\n5. Marketing\n6. Exit\n");

        scanf("%d", &serverpath);
        switch (serverpath)
        {
            case 1://root
                strcat(root, enteredfilename);
                strcpy(file_name,root);
                break;
            case 2://sales
                strcat(sales, enteredfilename);
                strcpy(file_name,sales);
                break; 
            case 3://promotions
                strcat(promotions, enteredfilename);
                strcpy(file_name,promotions); 
                break;   
            case 4://offers
                strcat(offers, enteredfilename);
                strcpy(file_name,offers);
                break;
            case 5://marketing
                strcat(marketing, enteredfilename);
                strcpy(file_name,marketing);
                break;
            case 6://exit
                printf("Client terminated");
                exit(1);
            default:
                printf("Please enter a number between 1-6");
                break;
        }
    }while((serverpath != 1) && (serverpath != 2) && (serverpath != 3) && (serverpath != 4) && (serverpath != 5) && (serverpath != 6));



    f = fopen(enteredfilename, "r");//r just means we are reading it
    while((c = getc(f))!= EOF)
    {
        fscanf(f, "%s", buffer);
        if(isspace(c) || c=='\t')
        words++;
    }
    write(SID, uid, sizeof(uid_t));
    write(SID, gid, sizeof(gid_t));
    write(SID, file_name, 255);
    write(SID, &words, sizeof(int));
    rewind(f);

    char ch;
    while (ch != EOF)
    {
        fscanf(f, "%s", buffer);
        write(SID, buffer,255);
        ch = fgetc(f);
    }

    printf("The file has been successfully sent");

    close(SID);
    return 0;
} 

