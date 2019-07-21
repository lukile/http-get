

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//Code client-server multi-process, with multi-thread
void *create_socket(void *socket_connection) {
    char str[4096];

    char* HTTP_HEADER[3];

    char* root;


    char path[1000];
    FILE* myfile;

    root = getenv("PWD");
    strcpy(path, root);

    memset(str,'\0' ,4096);



    read(*(int *)socket_connection, str, 4096);


    HTTP_HEADER[0] = strtok(str, " \n");
    HTTP_HEADER[1] = strtok(NULL, " \t");

    //page que l'on cherche
    HTTP_HEADER[2] = strtok(NULL, " \t");
    strcpy(&path[strlen(root)], HTTP_HEADER[1]);

    printf("%s\n",path);
    myfile = fopen(path, "r");



    memset(str,'\0' ,4096);
    if(myfile!=NULL){
        write(*(int*)socket_connection, "HTTP/1.1 200 OK\n\n", 17);


        while(fread(str, 1, 4096, myfile) != 0){
            //printf("%s\n",str);
            write(*(int*) socket_connection, str, 4096);


        }
    } else {
            write(*(int*)socket_connection, "HTTP/1.1 404 NOT FOUND\n", 23);

    }

    memset(str,'\0' ,4096);

    close(*(int*)socket_connection);

}

int main() {

    int listen_fd, socket_connection;

    pthread_t thread;

    struct sockaddr_in servaddr;
    struct sockaddr cliaddr;
    socklen_t sa_len=sizeof(cliaddr);

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero( &servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(8081);


    if(bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == 0) {
        printf("Success bind\n");
    }
    else{
        printf("Error bind\n");
    }

    listen(listen_fd, 10);

    //conn_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
    while(1) {
        socket_connection = accept(listen_fd, &cliaddr, &sa_len);

        pthread_create(&thread, NULL, create_socket, (void*)&socket_connection);
    }


}

