// Replace PUT_USERID_HERE with your actual BYU CS user id, which you can find
// by running `id -u` on a CS lab machine.
#define USERID 1823692630

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>


int verbose = 0;

void print_bytes(unsigned char *bytes, int byteslen);

int main(int argc, char *argv[]) {
	int level  = atoi(argv[3]);
	int id  = USERID;
	unsigned short seed = atoi(argv[4]);
	unsigned int nonce;
	unsigned int noncehold;
	unsigned char n = 1;
	unsigned char buf[8];
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	struct sockaddr_in address;
	struct sockaddr_in6 address6;
	struct sockaddr_in ipv4addr_remote;
	struct sockaddr_in6  ipv6addr_remote;
	int sfd = 0;
	unsigned char nonceArray[4];
	unsigned char recturnArray [135];
	int first = 0;
	int op = 5;
	int ipv6 = 0;
	unsigned short opParam;
	unsigned char treasure[1024];
	bzero(treasure, 1024);
	int lastIndex = 0;
	int family = AF_INET;
	unsigned short localPort = 8080;

	bzero(buf, 8);
	id = htonl(id);
	seed = htons(seed);

	memcpy(&buf[1], &level, 1);
	memcpy(&buf[2], &id, 4);
	memcpy(&buf[6], &seed, 2);

	while (n > 0 && n < 129){
		if(op == 1 || op == 2 || op ==3 ||op == 4 || op == 5 ){
			if (op!=1){
				close(sfd);
			}
			if(ipv6 == 0){
				family = AF_INET;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = INADDR_ANY;
    			address.sin_port = htons( localPort );
			}
			else{
				family = AF_INET6;
				address6.sin6_family = AF_INET6;
				address6.sin6_port = htons(localPort);
				bzero(address6.sin6_addr.s6_addr, 16);
			}
			if(op == 1 || op == 4 ){
				unsigned short portShort = ntohs(opParam);
				bzero(argv[2],strlen(argv[2]));
				sprintf(argv[2], "%d", portShort);
			}

			memset(&hints, 0, sizeof(struct addrinfo));
			if(op == 2){
				localPort = ntohs(opParam);
				if(ipv6 == 0){
					address.sin_port = htons( localPort );
				}else{
					address6.sin6_port =  htons( localPort );
				}
			}

			hints.ai_family = family;   
			hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
			hints.ai_flags = 0;
			hints.ai_protocol = 0;

			getaddrinfo(argv[1],argv[2], &hints, &result);
			for (rp = result; rp != NULL; rp = rp->ai_next) {
				if (op != 1){
					sfd = socket(rp->ai_family, rp->ai_socktype,
						rp->ai_protocol);
				}
				if( op != 3){
					if(ipv6 == 0){
						if (op != 4 && op != 1){
							if(bind(sfd ,(struct sockaddr *)&address, sizeof(address))< 0){
								continue;
							}
						}
						ipv4addr_remote = *(struct sockaddr_in *)rp->ai_addr;
						if (connect(sfd,(struct sockaddr *)&ipv4addr_remote, sizeof(struct sockaddr_in)) != -1){
							if(op == 4){
								struct sockaddr_in addr;
								socklen_t  addrlen = sizeof addr;
								getsockname(sfd,(struct sockaddr *)&addr, &addrlen);
								localPort = ntohs(addr.sin_port);
							}
							break;  /* Success */
						}
					}else{
						if (op != 4 && op != 1){
							if(bind(sfd ,(struct sockaddr *)&address6, sizeof(address6))< 0){
								continue;
							}
						}
						ipv6addr_remote = *(struct sockaddr_in6 *)rp->ai_addr;
						if (connect(sfd, (struct sockaddr *)&ipv6addr_remote, sizeof(struct sockaddr_in6))  != -1) {
								if(op == 4){
									struct sockaddr_in6 addr6;
									socklen_t  addrlen = sizeof addr6;
									getsockname(sfd,(struct sockaddr *)&addr6, &addrlen);
									localPort = ntohs(addr6.sin6_port);
								}
								break;
						}
					}	
				}
				else{
					if(ipv6 == 0){
						if(bind(sfd ,(struct sockaddr *)&address, sizeof(address)) >=0){
							break;
						}
					}
					else{
						if(bind(sfd ,(struct sockaddr *)&address6, sizeof(address6)) >=0){
							break;
						}
					}
				}
			}

			freeaddrinfo(result);  
		}
		bzero(recturnArray , 134);
		if (op == 3){
			unsigned int Sum = 0;
			for(int i=0; i<ntohs(opParam); i++){
				if (ipv6 == 0){
					struct sockaddr_in source;
					socklen_t len = sizeof source;
					recvfrom(sfd,&recturnArray,134,0,(struct sockaddr *)&source,&len);
					Sum += ntohs(source.sin_port);
					fflush(stdout);
				}else{
					struct sockaddr_in6 source6;
					socklen_t len6 = sizeof(source6);
					recvfrom(sfd,&recturnArray,134,0,(struct sockaddr *)&source6,&len6);
					Sum += ntohs(source6.sin6_port);
				}
				bzero(recturnArray , 134);
			}
			Sum += 1;
			nonce = htonl(Sum);
			memcpy(&nonceArray ,&nonce,4);
			close(sfd);
			getaddrinfo(argv[1],argv[2], &hints, &result);
			for (rp = result; rp != NULL; rp = rp->ai_next) {
				fflush(stdout);
				sfd = socket(rp->ai_family, rp->ai_socktype,
						rp->ai_protocol);
				if(ipv6 == 0){
					if(bind(sfd ,(struct sockaddr *)&address, sizeof(address))< 0){
						continue;
					}
				}else {
					if(bind(sfd ,(struct sockaddr *)&address6, sizeof(address6))< 0){
						continue;
					}
				}
				if(ipv6 == 0){
					ipv4addr_remote = *(struct sockaddr_in *)rp->ai_addr;
					if (connect(sfd,(struct sockaddr *)&ipv4addr_remote, sizeof(struct sockaddr_in)) != -1){
						break;  /* Success */
					}
				}else{
					ipv6addr_remote = *(struct sockaddr_in6 *)rp->ai_addr;
					if (connect(sfd, (struct sockaddr *)&ipv6addr_remote, sizeof(struct sockaddr_in6))  != -1) {
							break;
					}
				}
				close(sfd);
			}
			freeaddrinfo(result);  
			send(sfd,nonceArray,4,0);
			bzero(recturnArray , 135);
			recv(sfd,&recturnArray,135,0);
		}
		else { 
			if(first == 0){
				send(sfd,buf,8,0);
				first +=1;
			}
			else {
				send(sfd,nonceArray,4,0);
			}
			recv(sfd,&recturnArray,134,0);

		}
		memcpy(&n,&recturnArray[0],1);
		if (n > 0){
			memcpy(&treasure[lastIndex],&recturnArray[1],n);
			lastIndex +=n;
			memcpy(&op,&recturnArray[n+1],1);
			memcpy(&opParam,&recturnArray[n+2],2);

			memcpy(&nonce, &recturnArray[n+4], 4);
			noncehold = ntohl(nonce);
			noncehold += 1;
			nonce = htonl(noncehold);
			memcpy(&nonceArray ,&nonce,4);
			if(op == 4){
				ipv6 = ipv6 == 1 ? 0 :1;
			}
		}
	}
	
	printf("%s\n",treasure);

}
void print_bytes(unsigned char *bytes, int byteslen) {
	int i, j, byteslen_adjusted;

	if (byteslen % 8) {
		byteslen_adjusted = ((byteslen / 8) + 1) * 8;
	} else {
		byteslen_adjusted = byteslen;
	}
	for (i = 0; i < byteslen_adjusted + 1; i++) {
		if (!(i % 8)) {
			if (i > 0) {
				for (j = i - 8; j < i; j++) {
					if (j >= byteslen_adjusted) {
						printf("  ");
					} else if (j >= byteslen) {
						printf("  ");
					} else if (bytes[j] >= '!' && bytes[j] <= '~') {
						printf(" %c", bytes[j]);
					} else {
						printf(" .");
					}
				}
			}
			if (i < byteslen_adjusted) {
				printf("\n%02X: ", i);
			}
		} else if (!(i % 4)) {
			printf(" ");
		}
		if (i >= byteslen_adjusted) {
			continue;
		} else if (i >= byteslen) {
			printf("   ");
		} else {
			printf("%02X ", bytes[i]);
		}
	}
	printf("\n");
}
