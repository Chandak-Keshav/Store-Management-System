#define MAX_SIZE 1024
#ifndef STRUCTURE
#define STRUCTURE

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short int *array;
};

struct Admin{
    char name[MAX_SIZE];
    char password[MAX_SIZE];
};

struct UserCredentials{
    int id;
    char name[MAX_SIZE];
    char password[MAX_SIZE];
};
struct CartItems{
    int user_id;
    int product_id;
    char product_name[MAX_SIZE];
    int quantity;
};

struct product
{
    int productid;
    char product[MAX_SIZE];
    float price;
    int quantity;
};

struct log
{
    int user_id;
    char productid;
    float price;
    int quantity;
};

#endif