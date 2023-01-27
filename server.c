/*
server.c
Rosso Lorenzo 5B INF
TPSIT
Con thread e utilizzabile da più client
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>


#define ARGS_LEN 3
#define IP_DHCP "0.0.0.0" //indirizzo DHCP
#define MAX_CONN 50
#define MAX_DIM 4096 //4096 è la dimensione massima di un datagramma nelle comunicazioni


//prototipi
int errore(char* funz, int errLev);
void* manageConnection(void* p);
char* ricevi(int conn_id);

typedef struct
{
    char* msg;
    int conn_id;
}param;


int main(int argc, char* argv[]){
    if(argc != ARGS_LEN) {
        printf("USAGE: %s <port> <msg>\n", *argv);
        return -1;
    } else {
        printf("\n1) Tutti i parametri sono presenti\n");
    }
    param *p;
    int port = atoi(argv[1]);
    p->msg = argv[2];

    int socket_id = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_id < 0) {
        errore("socket()", -2);
    } else {
        printf("2) Socket creato con id: %d\n", socket_id);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_aton(IP_DHCP, &server_addr.sin_addr); //0.0.0.0 indirizzo DHCP
    server_addr.sin_port = htons(port);
    for(int i=0; i<8; i++) server_addr.sin_zero[i]=0;

    int rc = bind(socket_id, (struct sockaddr*) &server_addr, (socklen_t)sizeof(server_addr));
    if(rc < 0) {
        errore("bind()", -3);
    } else {
        printf("3) Bind effettuata con successo\n");
    }

    listen(socket_id, MAX_CONN);
    if(rc < 0) {
        errore("listen()", -4);
    } else {
        printf("4) listen effettuata con successo\n");
    }

    while(1){
        struct sockaddr_in client_addr;
        int len_addr = sizeof(struct sockaddr);
        p->conn_id = accept(socket_id, (struct sockaddr*)&client_addr, (socklen_t)&len_addr); //da questo momento si utilizzerà il conn_in e non più il socket_id
        if(p->conn_id < 0) {
            errore("accept()", -5);
        } else {
            printf("5) accept effettuata con id: %d\n", p->conn_id);
        }

        pthread_t thread_id;
        rc = pthread_create(&thread_id, NULL, manageConnection, p);
        if(rc) {
            errore("pthread_create()", -6);
        } else {
            printf("6) Thread creato con id: %d\n", *thread_id);
        }
        pthread_join(thread_id, NULL);
    }
    
    close(socket_id);
    return 0;
}


int errore(char* funz, int errLev){
    printf("%s/%d",funz, errLev);
    exit(errLev);
}


void* manageConnection(void* p){
    param par = *(param*)p;
    free(p);
    //char* msg_r = ricevi(par.conn_id);





    char buffer[MAX_DIM+1];
    int ret = recv(par.conn_id, buffer, MAX_DIM, 0);
    if(ret <= 0) {
        errore("recv()", -6);
    } else {
        printf("7) recv effettuata con successo\n");    
    }
    buffer[ret] = '\0'; //tappo subito dopo la fine della stringa ricevuti

    printf("MESSAGGIO RICEVUTO: %s\n", buffer);

    /* stampa con ip e porta del mittente
    char* client_ip = inet_ntoa(client_addr.sin_addr);
    int client_port = ntohs(client_addr.sin_port);
    printf("[%s:%d] %s\n", client_ip, client_port ,buffer);
    */

    //shutdown(conn_id);






    ret = send(par.conn_id, par.msg, strlen(par.msg), 0);
    if(ret <= 0){
        errore("send()", -7);
    } else {
        printf("8) Send effettuata con successo\n");
    }

    shutdown(par.conn_id, SHUT_RDWR);
    printf("9) Shutdown effettuata con successo\n");
    pthread_exit(NULL);// arriva a return 0 solo se non ci saranno errori nella recv() e nella send()
}


char* ricevi(conn_id){
    char buffer[MAX_DIM+1];
    int ret = recv(conn_id, buffer, MAX_DIM, 0);
    if(ret <= 0) {
        errore("recv()", -6);
    } else {
        printf("7) recv effettuata con successo\n");    
    }
    buffer[ret] = '\0'; //tappo subito dopo la fine della stringa ricevuti

    printf("MESSAGGIO RICEVUTO: %s\n", buffer);

    /* stampa con ip e porta del mittente
    char* client_ip = inet_ntoa(client_addr.sin_addr);
    int client_port = ntohs(client_addr.sin_port);
    printf("[%s:%d] %s\n", client_ip, client_port ,buffer);
    */

    //shutdown(conn_id);
    return strdup(buffer);
}