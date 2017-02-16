/*
    Simple udp server
*/
#include<stdio.h>
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define BUFLEN     512
#define IN_PORT    8889
#define OUT_PORT   8888
 
void die(char *s)
{
    perror(s);
    exit(1);
}
 
int main(int argc, char* argv[])
{
    struct sockaddr_in si_me, si_other;
     
    int s, recv_len;
	socklen_t slen = sizeof(si_other);
    char buf[BUFLEN];
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(IN_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
    //bind socket to port
    if( bind(s, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    }
	printf("\n # IP-Tables stopped?\n\n");
     
    //keep listening for data
    while(1)
    {
        printf("Waiting for data on port %d.. ", IN_PORT);
        fflush(stdout);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
        {
            die("recvfrom()");
        }
         
        //print details of the client/peer and the data received
        printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n" , buf);
         
        //now reply the client with the same data
        if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == -1)
        {
            die("sendto()");
        }
    }
 
    close(s);
    return 0;
}
