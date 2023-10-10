#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include"structures.h"
#include"user.h"
#include"user.c"
#define MAX_SIZE 1024


struct product pdt;

int main(void)
{
    struct sockaddr_in server ;
	int socket_descriptor;
	socket_descriptor = socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(8108);
	connect(socket_descriptor,(struct sockaddr*)&server,sizeof(server));
    printf("Welcome to our shop\n");
    int choice,user_id;
    printf("1.Admin\n2.User\n");
    printf("Enter your choice number\n");
    scanf("%d",&choice);
    write(socket_descriptor,&choice,sizeof(int));
    if(choice==1)
    {
        while(1)
        {
            printf("1.SignUp\n2.Login\n");
            int ch_login;
            printf("Enter your choice:");
            scanf("%d",&ch_login);
            write(socket_descriptor,&ch_login,sizeof(int));
            char name[MAX_SIZE],password[MAX_SIZE];
            printf("Enter username:");
            scanf("%s",name);
            write(socket_descriptor,name,sizeof(name));
            printf("Enter password:");
            scanf("%s",password);
            write(socket_descriptor,password,sizeof(password));
            int res;
            read(socket_descriptor,&res,sizeof(int));
            if(res==1){
                break;
            }
            else{
                continue;
            }
        }
        int admin_choice;
        while(admin_choice!=5){
            printf("1.Add a Product\n2. Delete a Product\n3.Update the quantity\n4.Update the price\n5.Logout\n");
            printf("Enter your choice number:");
            scanf("%d",&admin_choice);
            write(socket_descriptor,&admin_choice,sizeof(int));
            struct product pdt;
            int id,quantity,flag;
            float price;
            switch(admin_choice)
            {
                case 1:
                    printf("Enter productId,product name,price and quantity\n");
                    scanf("%d",&pdt.productid);
                    scanf("%s",pdt.product);
                    scanf("%f",&pdt.price);
                    scanf("%d",&pdt.quantity);
                    write(socket_descriptor,&pdt,sizeof(struct product));
                    
                    read(socket_descriptor,&flag,sizeof(int));
                    if(flag==0)
                        printf("Added succesfully\n");
                    else
                        printf("Add Failed\n");
                    break;
                case 2:
                    printf("Enter the product id for which you want to update the quantity\n");
                    scanf("%d",&id);
                    write(socket_descriptor,&id,sizeof(int));
                    
                    read(socket_descriptor,&flag,sizeof(int));
                    if(flag==0)
                        printf("Delete successfull\n");
                    else
                        printf("Delete failed\n");
                    break;
                case 3:
                    printf("Enter the product id for which you want to update the quantity\n");
                    scanf("%d",&id);
                    write(socket_descriptor,&id,sizeof(int));
                    printf("Enter the new quanity\n");
                    scanf("%d",&quantity);
                    write(socket_descriptor,&quantity,sizeof(int));
                    
                    read(socket_descriptor,&flag,sizeof(int));
                    if(flag==0)
                        printf("Updated quantity successfully\n");
                    else
                        printf("Update failed\n");
                    break;
                case 4:
                    printf("Enter the product id for which you want to update the price\n");
                    scanf("%d",&id);
                    write(socket_descriptor,&id,sizeof(int));
                    printf("Enter the new price\n");
                    scanf("%f",&price);
                    write(socket_descriptor,&price,sizeof(float));
                    
                    read(socket_descriptor,&flag,sizeof(int));
                    if(flag==0)
                        printf("Updated price successfully\n");
                    else
                        printf("Update failed\n");
                    break;
                case 5:
                    display(socket_descriptor);
                    exit;
                    break;
                default:
                    printf("Please enter correct choice\n");
            }
        }
    }
    else
    {
        int user_choice;
        printf("1.SignUp\n2.Login\n");
        int ch_login;
        printf("Enter your choice:");
        scanf("%d",&ch_login);
        write(socket_descriptor,&ch_login,sizeof(int));
        while(1)
        {
            if(ch_login==1){
                user_id=user_signup(socket_descriptor);
                printf("Signed In succesfully\n");
                break;
            }
            else if(ch_login==2){
                char name[MAX_SIZE],password[MAX_SIZE];
                printf("Enter username:");
                scanf("%s",name);
                printf("Enter password:");
                scanf("%s",password);
                user_id=user_login(socket_descriptor,name,password);
                if(user_id==0)
                {
                    printf("Login Failed! Try again\n");
                }
                else
                {
                    
                    printf("Logged In successfully\n");
                    break;
                }
            }
            else
            {
                printf("Please enter correct choice\n");
            }
        }
        while(user_choice!=6)
        {
            printf("1.Display all products\n2.Display your cart\n3.Add to cart\n4.Edit the cart\n5.Move to payment\n6.Logout\n");
            printf("Enter your choice:");
            scanf("%d",&user_choice);
            // write(socket_descriptor,&user_choice,sizeof(int));
            switch(user_choice)
            {
                case 1:
                    display(socket_descriptor);
                    break;
                case 2:
                    
                    displaycart(socket_descriptor,user_id);
                    break;
                case 3:
                    
                    addtocart(socket_descriptor,user_id);
                    break;
                case 4:
                    editcart(socket_descriptor,user_id);
                    break;
                case 5:
                    make_payment(socket_descriptor,user_id);
                    exit;
                    break;
                case 6:
                    exit;
                    break;
                default:
                    printf("Enter the correct choice\n");
                    break;
            }
        }
    }
}