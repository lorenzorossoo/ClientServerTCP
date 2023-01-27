/*
Rosso Lorenzo
5B INF
Client
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG 4096

int errore(char* funz, int errLev);

int main(int argc, char* argv[]){
    if(argc!=4){
        printf("USAGE: %d <ip><port><msg>", argc);
        return -1;
    } else {
        printf("\n1) Tutti i parametri sono presenti.\n");
    }

    char* ip = argv[1];
    int port = atoi(argv[2]);
    char* msg = argv[3];
    int rc;

    int socket_id;
    socket_id=socket(PF_INET,SOCK_STREAM,0);
    if(socket_id <= 0) {
        errore("socket()", -2);
    } else {
        printf("2) Socket creato con id: %d\n", socket_id);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(port);
    for(int i=0; i<8; i++) serverAddr.sin_zero[i]=0;
    inet_aton(ip, &serverAddr.sin_addr);
    
    rc = connect(socket_id,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(rc < 0) {
        errore("connect()", -3);
    } else {
        printf("3) Connessione creata\n");
    }

    int len = strlen(msg);
    rc = send(socket_id, msg, len, 0);
    if(rc <= 0){
        errore("send()", -4);
    } else {
        printf("4) Messaggio inviato con successo\n");
    }
    

    char buf[MAX_MSG];
    rc = recv(socket_id,buf,MAX_MSG,0);
    if(rc <= 0){
        errore("recv()", -5);
    } else {
        buf[strlen(buf)]='\0';
        printf("5) Messaggio ricevuto con successo: %s\n", buf);
    }

    return 0;
}

int errore(char* funz, int errLev){
    printf("%s/%d",funz, errLev);
    exit(errLev);
}