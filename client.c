/*
    Simple udp client
*/
#include<stdio.h>
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<arpa/inet.h>
#include<sys/socket.h>
 
#define BUFLEN     512
#define IN_PORT    8888
#define OUT_PORT   8889
 
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
	
	if (argc != 2) {
		printf("Usage: %s [DESTINATION-IP]\n", argv[0]);
		exit(0);
	}
    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(IN_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(OUT_PORT);
	if (inet_pton(AF_INET, argv[1], &si_other.sin_addr.s_addr) != 1) {
		printf("Invalid IP: %s\n", argv[1]);
		exit(1);
	}
     
    //create a UDP socket
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        die("socket");
    }
     
    //bind socket to port
    if( bind(s, (struct sockaddr*)&si_me, sizeof(si_me) ) == -1)
    {
        die("bind");
    	close(s);
    }

	const char* content = "Hallo, wie geht's?";
	//now reply the client with the same data
	if (sendto(s, content, strlen(content), 0, (struct sockaddr*) &si_other, slen) == -1)
	{
		die("sendto()");
	    close(s);
	}
	printf("Waiting for reply from %s on port %d.. ", argv[1], IN_PORT);
	fflush(stdout);
	 
	//try to receive some data, this is a blocking call
	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)) == -1)
	{
		die("recvfrom()");
    	close(s);
	}
	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
	printf("Data: %s\n" , buf);
	fflush(stdout);
 
    close(s);
    return 0;
}
