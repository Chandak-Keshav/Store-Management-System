#ifndef ADMIN
#define ADMIN
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<pthread.h>
#include"structures.h"
#define MAX_SIZE 1024

int add(struct product pdt);
int delete(int id);
int update_price(int id,float price);
int update_quantity(int id,int quantity);
void admin_signup(char name[MAX_SIZE],char password[MAX_SIZE]);
int admin_login(char name[MAX_SIZE],char password[MAX_SIZE]);
#endif
