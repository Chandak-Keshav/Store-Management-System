#include"user.h"
int user_count=0;
int user_signup(int socket_descriptor)
{
    int count=0,user_id;
    struct UserCredentials user;
    printf("Enter your name:");
    scanf("%s",user.name);
    printf("Enter your password:");
    scanf("%s",user.password);
    write(socket_descriptor,&user,sizeof(struct UserCredentials));
    read(socket_descriptor,&user_id,sizeof(int));
    return user_id;
}

int user_login(int socket_descriptor,char name[MAX_SIZE],char password[MAX_SIZE])
{
    struct UserCredentials user;
    strcpy(user.name,name);
    strcpy(user.password,password);
    write(socket_descriptor,&user,sizeof(struct UserCredentials));
    int user_id;
    read(socket_descriptor,&user_id,sizeof(int));
    printf("%d\n",user_id);
    return user_id;
}

void addtocart(int socket_descriptor,int user_id)
{
    int fd1,fd2;
    struct CartItems cart_item;
    fd1=open("cart_details",O_RDWR);
    fd2=open("product_list",O_RDONLY);
    if(fd1==-1)
    {
        printf("Failed to open cart_details file\n");
        return;
    }
    if(fd2==-1)
    {
        printf("Failed to open product_list file\n");
        return;
    }
    printf("Enter the product name:");
    scanf("%s",cart_item.product_name);
    printf("Enter the quantity:");
    scanf("%d",&cart_item.quantity);
    lseek(fd2,0,SEEK_SET);
    struct product pdt;
    int f=0;
    while(read(fd2,&pdt,sizeof(struct product))>0)
    {
        if(strcmp(cart_item.product_name,pdt.product)==0){
            f=1;
            cart_item.product_id=pdt.productid;
            break;
        }
    }
    if(f==0)
    {
        printf("No such products found\n");
        return;
    }
    cart_item.user_id=user_id;
    printf("Checking\n");
    printf("Cart user id=%d\t Cart product id=%d\n",cart_item.user_id,cart_item.product_id);
    lseek(fd1,0,SEEK_END);
    write(fd1,&cart_item,sizeof(struct CartItems));
    close(fd1);
    close(fd2);
}

void display(int socket_descriptor)
{
    int fd;
    fd=open("product_list",O_RDONLY,0777);
    lseek(fd,0,SEEK_SET);
    struct product pdt;
    while(read(fd,&pdt,sizeof(struct product))>0)
    {
        if(pdt.productid!=-1)
            printf("Product Id=%d\tProduct Name=%s\tPrice=%0.2f\tQuantity=%d\n",pdt.productid,pdt.product,pdt.price,pdt.quantity);
    }
    close(fd);
}

void editcart(int socket_descriptor,int user_id)
{
    char name[MAX_SIZE];
    int quantity;
    int fd=open("cart_details",O_RDWR);
    if(fd==-1)
    {
        printf("Failed to open cart_details file\n");
        return;
    }
    struct CartItems cart;
    printf("Enter the product name for which you want to update the quantity\n");
    scanf("%s",name);
    printf("Enter the new quantity\n");
    scanf("%d",&quantity);
    int f=0,c=0;
    if(quantity<0)
    {
        printf("Enter a positive quantity\n");
        return;
    }
    while(read(fd,&cart,sizeof(struct CartItems))>0)
    {
        c=1;
        if(user_id==cart.user_id)
        {
            if(strcmp(cart.product_name,name)==0)
            {
                f=1;
                break;
            }
        }
    }
    if(c==0)
    {
        printf("There is nothing in your cart\n");
        return;
    }
    if(f==0){
        printf("Could not find the product in your cart\n");
        return;
    }
    lseek(fd,-sizeof(struct CartItems),SEEK_CUR);
    cart.quantity=quantity;
    write(fd,&cart,sizeof(struct CartItems));
    printf("Cart Updated\n");
    close(fd);
}

void displaycart(int socket_descriptor,int user_id)
{
    int fd=open("cart_details",O_RDONLY);
    if(fd==-1)
        printf("Failed to open cart_details file\n");
    struct CartItems cart;
    while(read(fd,&cart,sizeof(struct CartItems))>0)
    {
        if(user_id==cart.user_id)
        {
            printf("Product Id:%d\tProduct Name:%s\tQuantity:%d\n",cart.product_id,cart.product_name,cart.quantity);
        }
    }
    close(fd);
}

void reduceQuantity(int socket_descriptor,int id,int quantity)
{
    struct product pdt;
    int fd=open("product_list",O_RDWR);
    if(fd==-1)
        printf("Some error occurred. Try again\n");
    while(read(fd,&pdt,sizeof(struct product))>0)
    {
        if(pdt.productid==id)
        {
            break;
        }
    }
    lseek(fd,-sizeof(struct product),SEEK_CUR);
    struct product new_pdt;
    if(pdt.quantity-quantity>0)
    {
        new_pdt.productid=id;
        new_pdt.quantity=pdt.quantity-quantity;
        new_pdt.price=pdt.price;
        strcpy(new_pdt.product,pdt.product);
    }
    else if(pdt.quantity==quantity)
    {
        new_pdt.productid=-1;
        new_pdt.quantity=0;
        new_pdt.price=0.0;
        strcpy(new_pdt.product,"");
    }
    else
        return;
    write(fd,&new_pdt,sizeof(new_pdt));
    close(fd);
}

void generateLog(int user_id,int product_id,int quantity,float price)
{
    int fd=open("log_details",O_WRONLY);
    if(fd==-1)
    {
        perror("open");
        return;
    }
    struct log l;
    l.user_id=user_id;
    l.productid=product_id;
    l.quantity=quantity;
    l.price=price;
    lseek(fd,0,SEEK_END);
    write(fd,&l,sizeof(struct log));
    close(fd);
}

void printLog(int user_id)
{
    int fd=open("log_details",O_RDONLY);
    if(fd==-1)
    {
        perror("open");
        return;
    }
    struct log l;
    int sl=1;
    lseek(fd,0,SEEK_SET);
    while(read(fd,&l,sizeof(struct log))>0)
    {
        if(l.user_id==user_id){
            if(l.productid!=-1){
                printf("Sl.No=%d\tProduct Id=%d\tQuantity=%d\tPrice=%0.2f\t Tot.Price=%0.2f\n",sl,l.productid,l.quantity,l.price,l.quantity*l.price);
                sl++;
                l.productid=-1;
                lseek(fd,-sizeof(struct log),SEEK_CUR);
                write(fd,&l,sizeof(struct log));
            }
        }
    }
}

void deleteCart(int user_id,int product_id)
{
    int fd=open("cart_details",O_RDWR);
    struct CartItems cart;
    while(read(fd,&cart,sizeof(struct CartItems))>0)
    {
        if(cart.user_id==user_id && cart.user_id!=-1)
        {
            cart.user_id=-1;
            cart.product_id=-1;
            lseek(fd,-sizeof(struct CartItems),SEEK_CUR);
            write(fd,&cart,sizeof(struct CartItems));
        }
    }
    close(fd);
}
void make_payment(int socket_descriptor, int user_id) {
    int fd1 = open("cart_details", O_RDONLY);
    int fd2 = open("product_list", O_RDWR);
    if (fd1 == -1 || fd2 == -1) {
        printf("Failed to open files. Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0666);
    if (sem_id == -1) {
        printf("Failed to create semaphores. Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Initialize the semaphores
    union semun arg;
    unsigned short init_vals[] = {1, 1}; // Set both semaphores to 1
    arg.array = init_vals;
    if (semctl(sem_id, 0, SETALL, arg) == -1) {
        printf("Failed to initialize semaphores. Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sembuf wait_op = {0, -1, SEM_UNDO}; // Semaphore wait operation
    struct sembuf signal_op = {0, 1, SEM_UNDO}; // Semaphore signal operation

    // Lock the product_list file
    if (semop(sem_id, &wait_op, 1) == -1) {
        printf("Failed to lock product_list file. Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int sl=1;
    printf("Press enter to continue:");
    getchar();
    getchar();
    char log[MAX_SIZE];
    struct CartItems cart;
    struct product prod;
    float total_price = 0.0;
    while (read(fd1, &cart, sizeof(struct CartItems)) > 0) {
        int quan=cart.quantity;
        if (cart.product_id == -1)
            continue;
        if (user_id == cart.user_id && cart.user_id != -1) {
            lseek(fd2,0,SEEK_SET);
            while (read(fd2, &prod, sizeof(struct product)) > 0) {
                if (strcmp(cart.product_name,prod.product)!=0)
                    continue;
                if (prod.quantity < 0)
                    continue;
                if (cart.quantity <= prod.quantity) {
                    printf("Sl.No=%d\tProduct Id=%d\tQuantity=%d\tPrice=%0.2f\t Tot.Price=%0.2f\n",sl,prod.productid,quan,prod.price,quan*prod.price);
                    sl++;
                    reduceQuantity(socket_descriptor, cart.product_id, cart.quantity);
                    deleteCart(user_id,cart.product_id);
                    total_price += quan * prod.price;
                } else {
                    printf("We have only %d quantity of %s.\nDo you want to buy (1-Yes, 2-No)\n", prod.quantity,
                           prod.product);
                    int yes_no;
                    scanf("%d", &yes_no);
                    if (yes_no == 1) {
                        printf("Sl.No=%d\tProduct Id=%d\tQuantity=%d\tPrice=%0.2f\t Tot.Price=%0.2f\n",sl,prod.productid,prod.quantity,prod.price,prod.quantity*prod.price);
                        sl++;
                        reduceQuantity(socket_descriptor, cart.product_id, prod.quantity);
                        deleteCart(user_id,cart.product_id);
                        total_price += prod.quantity * prod.price;
                    }
                }
            }
        }
    }

    // Unlock the product_list file
    if (semop(sem_id, &signal_op, 1) == -1) {
        printf("Failed to unlock product_list file. Error: %s\n", strerror(errno));
    }

    printf("Total Price=%0.2f\n", total_price);
    close(fd1);
    close(fd2);
}
