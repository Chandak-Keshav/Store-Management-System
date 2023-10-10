#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include"structures.h"
#include"admin.h"
#include"admin.c"
#include"user.h"
#include"user.c"
#define MAX_SIZE 1024

void serv_user_signup(int clientSockfd)
{
    int fd;
    int count=0;
    struct UserCredentials user,no_of_users;
    read(clientSockfd,&user,sizeof(struct UserCredentials));
    fd=open("user_auth_details",O_RDWR);
    if(fd==-1)
    {
        printf("File descriptor issues\n");
    }
    while(read(fd,&no_of_users,sizeof(struct UserCredentials))>0)
    {
        count++;
    }
    printf("Username=%s\nPassword=%s\n",user.name,user.password);
    user.id=count+1;
    write(fd,&user,sizeof(struct UserCredentials));
    write(clientSockfd,&user.id,sizeof(int));
    close(fd);
}

int serv_user_login(int clientSockfd)
{
    int fd;
    char name[MAX_SIZE];
    char password[MAX_SIZE];
    struct UserCredentials entered_user;
    read(clientSockfd,&entered_user,sizeof(struct UserCredentials));
    strcpy(name,entered_user.name);
    strcpy(password,entered_user.password);
    fd=open("user_auth_details",O_RDONLY,0777);
    struct UserCredentials user;
    while(read(fd,&user,sizeof(struct UserCredentials))>0)
    {
        if(strcmp(name,user.name)==0)
        {
            if(strcmp(password,user.password)==0)
            {
                write(clientSockfd,&user.id,sizeof(int));
                printf("Username=%s\nPassword=%s\n",user.name,user.password);
                return user.id;
            }
        }
    }
    int val=0;
    write(clientSockfd,&val,sizeof(int));
    close(fd);
    return 0;
}








void *handleClient(void *arg)
{
    int clientSockfd = *(int *)arg;
    int choice;
    int status = read(clientSockfd, &choice, sizeof(int));
    if (status == -1)
    {
        perror("read");
        return NULL;
    }
    if(choice==1)
    {
        int ch_login;
        //admin ke saare process
        while(1)
        {
            read(clientSockfd,&ch_login,sizeof(int));
            char name[MAX_SIZE];
            char password[MAX_SIZE];
            read(clientSockfd,name,sizeof(name));
            // printf("%s\n",name);
            read(clientSockfd,password,sizeof(password));
            // printf("%s\n",password);
            if(ch_login==1){
                admin_signup(name,password);
                int x=1;
                write(clientSockfd,&x,sizeof(int));
                // printf("Signed In succesfully\n");
                break;
            }
            else if(ch_login==2){
                int flag=admin_login(name,password);
                if(flag==1)
                {
                    int x=1;
                    write(clientSockfd,&x,sizeof(int));
                    // printf("Logged In successfully\n");
                    break;
                }
                else
                {
                    int x=0;
                    write(clientSockfd,&x,sizeof(int));
                    // printf("Login Failed! Try again\n");
                }
            }
            else
            {
                int x=0;
                write(clientSockfd,&x,sizeof(int));
                // printf("Please enter correct choice\n");
            }
        }
        int admin_choice=0;
        while(admin_choice!=5){
            read(clientSockfd,&admin_choice,sizeof(int));
            struct product pdt;
            int flag;
            int id,quantity;
            float price;
            switch(admin_choice)
            {
                case 1:
                    read(clientSockfd,&pdt,sizeof(struct product));
                    flag=add(pdt);
                    write(clientSockfd,&flag,sizeof(int));
                    break;
                case 2:
                    read(clientSockfd,&id,sizeof(int));
                    flag=delete(id);
                    write(clientSockfd,&flag,sizeof(int));
                    break;
                case 3:
                    read(clientSockfd,&id,sizeof(int));
                    read(clientSockfd,&quantity,sizeof(int));
                    if(quantity<0)
                        flag=1;
                    else if(quantity==0)
                        flag=delete(id);
                    else
                        flag=update_quantity(id,quantity);
                    write(clientSockfd,&flag,sizeof(int));
                    break;
                case 4:
                    read(clientSockfd,&id,sizeof(int));
                    read(clientSockfd,&price,sizeof(float));
                    if(price<0.0)
                        flag=1;
                    else
                        flag=update_price(id,price);
                    write(clientSockfd,&flag,sizeof(int));
                    break;
                case 5:
                    exit;
                    break;
                default:
                    break;
            }
        }
    }
    else if(choice==2)
    {//user ke saare process
        int login_ch,ret;
        while(1){
            read(clientSockfd,&login_ch,sizeof(int));
            if(login_ch==1)
            {
                serv_user_signup(clientSockfd);
                break;
            }
            else
            {
                ret=serv_user_login(clientSockfd);
                if(ret!=0)
                    break;
            }
        }
    }
    close(clientSockfd);
    return NULL;
}

int main()
{
    // ! create a raw socket
    // socket(int domain, int type, int protocol)
    // AF_INET - family of protocols to be used
    // SOCK_STREAM - type of socket = TCP
    // UDP = SOCK_DGRAM
    // 0 - protocol - 0 for default
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("socket");
        return 1;
    }
    printf("Socket created successfully\n");

    // ! create a socket address structure
    struct sockaddr_in serverAddr = {
        .sin_addr.s_addr = INADDR_ANY, // ! INADDR_ANY = allow any address on the machine
        .sin_family = AF_INET,         // ! AF_INET = IPv4
        .sin_port = htons(8108)        // ! htons = host to network short - since we are using intel architecture
    };

    // ! bind the socket to the address
    int status = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    if (status == -1)
    {
        perror("bind");
        return 1;
    }
    printf("Socket binded successfully\n");

    // ! listen for connections
    int backlog = 5;
    status = listen(sockfd, backlog); // ! backlog = max number of pending connections in the queue

    while (1)
    {
        // ! accept a connection
        struct sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        // ! accept = blocks until a connection is made
        printf("Listening for connections on port 8008\n");
        int clientSockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSockfd == -1)
        {
            perror("accept");
            return 1;
        }
        printf("Connection accepted\n");

        // ! create a thread to handle the request
        pthread_t thread;
        status = pthread_create(&thread, NULL, handleClient, &clientSockfd);
        if (status != 0)
        {
            perror("pthread_create");
            return 1;
        }
        pthread_detach(thread); // ! detach the thread so that it can be cleaned up automatically
    }
    return 0;
}