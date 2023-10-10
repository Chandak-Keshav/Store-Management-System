#ifndef USER
#define USER
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#define MAX_SIZE 1024
#include"structures.h"

void addtocart(int socket_descriptor,int user_id);
void display(int socket_descriptor);
void displaycart(int socket_descriptor,int user_id);
void editcart(int socket_descriptor,int user_id);
int user_signup(int socket_descriptor);
int user_login(int socket_descriptor,char name[MAX_SIZE],char password[MAX_SIZE]);
void reduceQuantity(int socket_descriptor,int id,int quantity);
void make_payment(int socket_descriptor,int user_id);

#endif