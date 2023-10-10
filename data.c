#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int admin_fd, product_fd, user_auth_fd, cart_fd;

    // Create admin_details file
    admin_fd = open("admin_details", O_WRONLY | O_CREAT, 0777);
    if (admin_fd == -1) {
        perror("Error creating admin_details file");
        return 1;
    }
    close(admin_fd);

    // Create product_list file
    product_fd = open("product_list", O_WRONLY | O_CREAT, 0777);
    if (product_fd == -1) {
        perror("Error creating product_list file");
        return 1;
    }
    close(product_fd);

    // Create user_auth_details file
    user_auth_fd = open("user_auth_details", O_WRONLY | O_CREAT, 0777);
    if (user_auth_fd == -1) {
        perror("Error creating user_auth_details file");
        return 1;
    }
    close(user_auth_fd);

    // Create cart_details file
    cart_fd = open("cart_details", O_WRONLY | O_CREAT, 0777);
    if (cart_fd == -1) {
        perror("Error creating cart_details file");
        return 1;
    }
    close(cart_fd);
    int log_fd;
    log_fd = open("log_details", O_WRONLY | O_CREAT, 0777);
    if (log_fd == -1) {
        perror("Error creating cart_details file");
        return 1;
    }
    close(log_fd);

    printf("Data files created successfully.\n");

    return 0;
}
