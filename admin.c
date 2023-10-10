#include"admin.h"

struct product pdt;

void admin_signup(char name[MAX_SIZE],char password[MAX_SIZE])
{
    int fd;
    struct Admin ad;
    strcpy(ad.name,name);
    strcpy(ad.password,password);
    fd=open("admin_details",O_RDWR|O_CREAT,0777);
    if(fd==-1)
        printf("Some error occurred\n");
    lseek(fd,0,SEEK_END);
    write(fd,&ad,sizeof(struct Admin));
    printf("Username=%s\nPassword=%s\n",ad.name,ad.password);
    close(fd);
}

int admin_login(char name[MAX_SIZE],char password[MAX_SIZE])
{
    int fd;
    fd=open("admin_details",O_RDONLY);
    struct Admin ad;
    strcpy(ad.name,name);
    strcpy(ad.password,password);
    while(read(fd,&ad,sizeof(struct Admin)))
    {
        if(strcmp(name,ad.name)==0)
        {
            if(strcmp(password,ad.password)==0)
            {
                printf("Username=%s\nPassword=%s\n",ad.name,ad.password);
                return 1;
            }
            else
                return 0;
        }
    }
    close(fd);
    return 0;
}
int add(struct product pdt)
{
    int fd;
    fd=open("product_list",O_RDWR|O_CREAT,0777);
    if(fd==-1){
        printf("Some error occurred. Try again\n");
        return 1;
    }
    lseek(fd,0,SEEK_END);
    write(fd,&pdt,sizeof(struct product));
    close(fd);
    return 0;
}

int delete(int id) {
    int fd = open("product_list", O_RDWR);
    if (fd == -1) {
        printf("Some error occurred. Try again\n");
        return 1;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }
    int flag = 0;
    while (read(fd, &pdt, sizeof(struct product))) {
        if (pdt.productid == id) {
            flag = 1;
            pdt.productid=-1;
            break;
        }
    }
    if (flag == 0) {
        printf("No such product exist");
        close(fd);
        return 1;
    } else {
        lseek(fd, -sizeof(struct product), SEEK_CUR);
        struct product new_pdt;
        new_pdt.productid = -1;
        new_pdt.quantity = 0;
        new_pdt.price = 0.0;
        strcpy(new_pdt.product, "");
        write(fd, &new_pdt, sizeof(new_pdt));
    }

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int update_quantity(int id, int quantity) {
    int fd = open("product_list", O_RDWR);
    if (fd == -1) {
        printf("Some error occurred. Try again\n");
        return 1;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }
    int f=0;
    while (read(fd, &pdt, sizeof(struct product))>0) {
        if (pdt.productid == id) {
            f=1;
            break;
        }
    }
    if(f==0)
        return 1;
    lseek(fd, -sizeof(struct product), SEEK_CUR);
    struct product new_pdt;
    new_pdt.productid = id;
    new_pdt.quantity = quantity;
    new_pdt.price = pdt.price;
    strcpy(new_pdt.product, pdt.product);
    write(fd, &new_pdt, sizeof(new_pdt));

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int update_price(int id, float price) {
    int fd = open("product_list", O_RDWR);
    if (fd == -1) {
        printf("Some error occurred. Try again\n");
        return 1;
    }

    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start = 0;
    fl.l_len = 0;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }
    int f=0;
    while (read(fd, &pdt, sizeof(struct product))) {
        if (pdt.productid == id) {
            f=1;
            break;
        }
    }
    if(f==0)
        return 1;
    lseek(fd, -sizeof(struct product), SEEK_CUR);
    struct product new_pdt;
    new_pdt.productid = id;
    new_pdt.quantity = pdt.quantity;
    new_pdt.price = price;
    strcpy(new_pdt.product, pdt.product);
    write(fd, &new_pdt, sizeof(new_pdt));

    fl.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLK, &fl) == -1) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

