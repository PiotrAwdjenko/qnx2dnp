#include <string.h>
#include <cstring>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <strings.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <string>
#include <time.h>
#include <vector>
//#include <ncurses.h>

#include "QNXclient.h"

using namespace std;
//void *task0_timer(void *);
void *task1(void *);
void *task2(void *);

pthread_t threadA[3];
//bool loop1 = true;

int ConnectedQNXP=0;
int ConnectedQNXR=0;

extern char buforqnx[];
extern char can[];
extern int qnxserie;
extern int qnxdbg;
//extern QNXBuffSize;
using std::string;

int lp=0;
int x;
int listenFd, listenFd2, portNoP, portNoR;
char *QnxAddressP;// = "255.255.255.255";
char *QnxAddressR;// = "255.255.255.255";
//string QnxAdr = "255.255.255.255";

unsigned int timeoutP=90, timeoutR=90;

struct timeval tvP;
struct timeval tvR;

int ConectQNXP(void);
int ConectQNXR(void);
//int ConectQNX(int argc, char* argv[]);

//int main (int argc, char* argv[])
//{
//    int loop;
//    loop = 1;
//    while(loop) {
//	ConectQNX(argc, argv);
//	usleep(1000 * 1000);
//    }
//}
//int ConectQNX(int argc, char* argv[])

//=================================================================================
int ConectQNXP(void)
{
//    int listenFd, portNo;
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
tvP.tv_sec = timeoutP;
tvP.tv_usec = 0;

time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
strftime(s, sizeof(s), "%c", tm);

//char QnxAddress[24] = "255.255.255.255";

//    QnxAddress = strdup("10.10.10.1");
//    QnxAddress = "10.10.10.1";
//    strcpy(QnxAddress,"10.10.10.5");

//    if(argc < 3)
//    {
//        cerr<<"Syntax : ./client <host name> <port>"<<endl;
//        return 0;
//    }
    
//    portNo = atoi(argv[2]);
//    portNo = 2345;
//    if((portNo > 65535) || (portNo < 2000))
//    {
//        cerr<<"Please enter port number between 2000 - 65535"<<endl;
//        return 0;
//    }       
    
    //create client skt
    listenFd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvP,sizeof(struct timeval));
    
    if(listenFd < 0)
    {
        cerr << "PODSTAWA Cannot open socket" << endl;
        return 0;
    }
    
//    server = gethostbyname("10.10.10.1");
    server = gethostbyname(QnxAddressP);
//    server = gethostbyname(argv[1]);
//    server = "10.10.10.1";

    if(server == NULL)
    {
        cerr << "PODSTAWA Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
    svrAdd.sin_port = htons(portNoP);
    
    int checker = connect(listenFd,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
    if (checker < 0)
    {
//        cerr << "PODSTAWA Cannot connect!" << endl;
        return 0;
    } else {
	ConnectedQNXP=1;
printf("\33[0;1;34m%s -> ", s);
        printf("\33[0;1;34mPODSTAWA Connected\33[m\n");
//	loop1 = true;
	pthread_create(&threadA[0], NULL, task1, NULL);
	}
    
    //send stuff to server
//    for(;;)
//    {
//        char s[300];
//	int odqnx;
//
//	bzero(s, 301);
//
//        odqnx = read(listenFd, s, 300);
//	if(odqnx == 0) return(1);
//	lp++;
//	printf("QNX:%d -> %d -> ", lp, odqnx);
//	for(x=0; x<odqnx; x++) {
//	    printf("%02X ", s[x] );
//	}
//	printf("\n");
//
//    }
}

//=================================================================================
int ConectQNXR(void)
{
    bool loop = false;
    struct sockaddr_in svrAdd;
    struct hostent *server;
tvR.tv_sec = timeoutR;
tvR.tv_usec = 0;

time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
strftime(s, sizeof(s), "%c", tm);


    listenFd2 = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(listenFd2, SOL_SOCKET, SO_RCVTIMEO, (char *)&tvR,sizeof(struct timeval));
    
    if(listenFd2 < 0)
    {
        cerr << "REZERWA Cannot open socket" << endl;
        return 0;
    }
    
//    server = gethostbyname("10.10.10.1");
    server = gethostbyname(QnxAddressR);
//    server = gethostbyname(argv[1]);
//    server = "10.10.10.1";

    if(server == NULL)
    {
        cerr << "REZERWA Host does not exist" << endl;
        return 0;
    }
    
    bzero((char *) &svrAdd, sizeof(svrAdd));
    svrAdd.sin_family = AF_INET;
    
    bcopy((char *) server -> h_addr, (char *) &svrAdd.sin_addr.s_addr, server -> h_length);
    
    svrAdd.sin_port = htons(portNoR);
    
    int checker = connect(listenFd2,(struct sockaddr *) &svrAdd, sizeof(svrAdd));
    
    if (checker < 0)
    {
//        cerr << "REZERWA Cannot connect!" << endl;
        return 0;
    } else {
	ConnectedQNXR=1;
printf("\33[0;1;34m%s -> ", s);
        printf("\33[0;1;34mREZERWA  Connected\33[m\n");
	pthread_create(&threadA[1], NULL, task2, NULL);
	}
    
    //send stuff to server
//    for(;;)
//    {
//        char s[300];
//	int odqnx;
//
//	bzero(s, 301);
//
//        odqnx = read(listenFd, s, 300);
//	if(odqnx == 0) return(1);
//	lp++;
//	printf("QNX:%d -> %d -> ", lp, odqnx);
//	for(x=0; x<odqnx; x++) {
//	    printf("%02X ", s[x] );
//	}
//	printf("\n");
//
//    }
}
unsigned char bufqnx[4096];
unsigned char ramka[64];
//int bufqnxpos;
int ramkapos;
unsigned char bufqnx2[4096];
unsigned char ramka2[64];
//int bufqnxpos;
int ramkapos2;
extern int idx[1024], ss[1024], seria[1024], bit[1024], typ[1024], stan[1024];
extern int idx2[1024], ss2[1024], seria2[1024], bit2[1024], typ2[1024], stan2[1024];
int qnx_ss, qnx_seria, qnx_tresc;
int qnx_ss2, qnx_seria2, qnx_tresc2;
extern int INDEX;

//========================================================================================
int task1_tablica(void)
{
int x;
//    printf("[...task1->tablica...]\n");
    for(x=1; x<=INDEX; x++) {
	if((qnx_ss==ss[x]) & (qnx_seria==seria[x])) {
	    switch(bit[x])
	    {
		case 0x01:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0001); }
		               else { stan[x] = ((qnx_tresc & 0x0001) ^ stan[typ[x]]); }//if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x02:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0002) >> 1; }
		               else { stan[x] =(((qnx_tresc & 0x0002) >> 1 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x03:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0004) >> 2; }
		               else { stan[x] =(((qnx_tresc & 0x0004) >> 2 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x04:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0008) >> 3; }
		               else { stan[x] =(((qnx_tresc & 0x0008) >> 3 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x05:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0010) >> 4; }
		               else { stan[x] =(((qnx_tresc & 0x0010) >> 4 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x06:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0020) >> 5; }
		               else { stan[x] =(((qnx_tresc & 0x0020) >> 5 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x07:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0040) >> 6; }
		               else { stan[x] =(((qnx_tresc & 0x0040) >> 6 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x08:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0080) >> 7; }
		               else { stan[x] =(((qnx_tresc & 0x0080) >> 7 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x09:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0100) >> 8; }
		               else { stan[x] =(((qnx_tresc & 0x0100) >> 8 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0a:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0200) >> 9; }
		               else { stan[x] =(((qnx_tresc & 0x0200) >> 9 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0b:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0400) >> 10; }
		               else { stan[x] =(((qnx_tresc & 0x0400) >> 10 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0c:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x0800) >> 11; }
		               else { stan[x] =(((qnx_tresc & 0x0800) >> 11 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0d:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x1000) >> 12; }
		               else { stan[x] =(((qnx_tresc & 0x1000) >> 12 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0e:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x2000) >> 13; }
		               else { stan[x] =(((qnx_tresc & 0x2000) >> 13 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x0f:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x4000) >> 14; }
		               else { stan[x] =(((qnx_tresc & 0x4000) >> 14 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x10:
		    if(typ[x] == 0) { stan[x] = ( qnx_tresc & 0x8000) >> 15; }
		               else { stan[x] =(((qnx_tresc & 0x8000) >> 15 ) ^ stan[typ[x]]); if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		default:
		    break;
	    }
//	    if(typ[x] >
	    if ( qnxdbg ) printf("P SS:%3d Seria:%02X Bit:%2d INDEX:%2d Typ:%3d STAN:%d\n", qnx_ss, qnx_seria, bit[x], x, typ[x], stan[x]);
	}
    }
}

//========================================================================================
int task2_tablica(void)
{
int x;
//    printf("[...task1->tablica...]\n");
    for(x=1; x<=INDEX; x++) {
	if((qnx_ss2==ss2[x]) & (qnx_seria2==seria2[x])) {
	    switch(bit2[x])
	    {
		case 0x01:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0001); }
		                else { stan2[x] = ((qnx_tresc2 & 0x0001) ^ stan2[typ2[x]]); }//if(stan[x]==1) stan[x]=0; else stan[x]=1; }
		    break;
		case 0x02:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0002) >> 1; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0002) >> 1 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x03:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0004) >> 2; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0004) >> 2 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x04:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0008) >> 3; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0008) >> 3 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x05:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0010) >> 4; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0010) >> 4 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x06:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0020) >> 5; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0020) >> 5 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x07:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0040) >> 6; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0040) >> 6 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x08:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0080) >> 7; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0080) >> 7 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x09:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0100) >> 8; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0100) >> 8 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0a:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0200) >> 9; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0200) >> 9 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0b:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0400) >> 10; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0400) >> 10 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0c:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x0800) >> 11; }
		                else { stan2[x] =(((qnx_tresc2 & 0x0800) >> 11 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0d:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x1000) >> 12; }
		                else { stan2[x] =(((qnx_tresc2 & 0x1000) >> 12 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0e:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x2000) >> 13; }
		                else { stan2[x] =(((qnx_tresc2 & 0x2000) >> 13 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x0f:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x4000) >> 14; }
		                else { stan2[x] =(((qnx_tresc2 & 0x4000) >> 14 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		case 0x10:
		    if(typ2[x] == 0) { stan2[x] = ( qnx_tresc2 & 0x8000) >> 15; }
		                else { stan2[x] =(((qnx_tresc2 & 0x8000) >> 15 ) ^ stan2[typ2[x]]); if(stan2[x]==1) stan2[x]=0; else stan2[x]=1; }
		    break;
		default:
		    break;
	    }
//	    if(typ[x] >
	    if ( qnxdbg ) printf("r SS:%3d Seria:%02X Bit:%2d INDEX:%2d Typ:%3d STAN:%d\n", qnx_ss2, qnx_seria2, bit2[x], x, typ2[x], stan2[x]);
	}
    }
}

//========================================================================================
int task1_ramka(void)
{
int x,p;
time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
strftime(s, sizeof(s), "%c", tm);
//    printf("\n[...task1:ramka...]:\n");
    p=0;
    for(x=0; x<ramkapos; x++) {
	if(ramka[x]==0xf0) {
	    ramka[p]=ramka[x] | ramka[x+1];
	    x++;
	} else ramka[p]=ramka[x];
//	printf("%02X ", ramka[p]);
	p++;
    } //printf("\n");

    qnx_seria = ramka[8];
    if((qnx_seria >= 0x41) & (qnx_seria <= 0x7f)) { qnx_seria = qnx_seria-0x40; }
    if(qnx_seria > 0x40) { return 0; }
    qnx_ss    = ramka[5];  qnx_ss    = (qnx_ss << 8) | ramka[4];
    qnx_tresc = ramka[10]; qnx_tresc = (qnx_tresc << 8) | ramka[9];
//printf("%s -> ", s);
//#ifdef QNXSERIE
    if ( qnxserie ) printf("\33[1m%3d-%02X:%04X\33[m\n", qnx_ss, qnx_seria, qnx_tresc );
//#endif
//    refresh();
//    timer_1 = 0;
    task1_tablica();
}

//========================================================================================
int task2_ramka(void)
{
int x,p;
time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
strftime(s, sizeof(s), "%c", tm);
//    printf("\n[...task1:ramka...]:\n");
    p=0;
    for(x=0; x<ramkapos2; x++) {
	if(ramka2[x]==0xf0) {
	    ramka2[p]=ramka2[x] | ramka2[x+1];
	    x++;
	} else ramka2[p]=ramka2[x];
//	printf("%02X ", ramka[p]);
	p++;
    } //printf("\n");

    qnx_seria2 = ramka2[8];
    if((qnx_seria2 >= 0x41) & (qnx_seria2 <= 0x7f)) { qnx_seria2 = qnx_seria2-0x40; }
    if(qnx_seria2 > 0x40) { return 0; }
    qnx_ss2    = ramka2[5];  qnx_ss2    = (qnx_ss2 << 8) | ramka2[4];
    qnx_tresc2 = ramka2[10]; qnx_tresc2 = (qnx_tresc2 << 8) | ramka2[9];
//printf("%s -> ", s);
//#ifdef QNXSERIE
    if ( qnxserie ) printf("\33[0m%3d-%02X:%04X\33[0m\n", qnx_ss2, qnx_seria2, qnx_tresc2 );
//#endif
//    refresh();
//    timer_1 = 0;
    task2_tablica();
}

//bool loop1 = true;
//========================================================================================
//void *task0_timer (void *dummyPt)
//{
//bool loop = true;

//    while(loop) {
//	timer_1++;
//	timer_2++;
//	printf("T1:%2d T2:%2d\n", timer_1, timer_2 );
//	if(timer_1 >= 60) {
//	    timer_1=0;
////	    ConnectedQNX=0;
////	    close(listenFd);
//	    printf("QNX podstawa timeout...\n");
////	    usleep(1000 * 1000 * 15);
//	    loop1 = false;
//	}
//	usleep(1000 * 1000);
//    }
//}

//struct pollfd fd;
//int ret;
//========================================================================================
void *task1 (void *dummyPt)
{
bool loop1 = true;
//char bufqnxread[4096];
int IleOdQnx;
int x;
//struct pollfd fd;
//int ret;

time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
//strftime(s, sizeof(s), "%c", tm);

    while(loop1) {
strftime(s, sizeof(s), "%c", tm);
//	bzero(bufqnxread, 4096);
	bzero(bufqnx, 4096);
	IleOdQnx = 0;
//	printf("Jestem przed read...%d\n", IleOdQnx );
	IleOdQnx = read(listenFd, bufqnx, 4096);
//	printf("Jestem po read...%d\n", IleOdQnx );
//	IleOdQnx = recv(listenFd, bufqnx, 4096, 0);
	if(IleOdQnx <= 0) { loop1 = false; }
//	printf("[...task1...%d]:\n", IleOdQnx );
//	printf(".");
//for(x=0; x<=IleOdQnx; x++) { 
//    bufqnx[x] = bufqnx[x] & 0xff;
//    printf("%02X\n", bufqnx[x]);
//    if(bufqnx[x] > 0xff) { printf("->"); }
//    printf("%02X\n", bufqnx[x]);
//}

	for(x=0; x<IleOdQnx; x++) {
	    if(bufqnx[x]==0xf1) ramkapos=0;
	    ramka[ramkapos] = bufqnx[x];
	    ramkapos++;
//	    printf("%02X ", bufqnx[x]);
	    if(bufqnx[x]==0xf2) {
		task1_ramka();
//		printf("\n");
	    }
	} //printf("\n");
//	usleep(1000 * 1000);
	usleep(100);
    }

    ConnectedQNXP=0;
    close(listenFd);
printf("\33[0;1;33m%s -> ", s );
    printf("\33[0;1;33mPODSTAWA Socket Timeout Zamykam [task1]\33[m\n");
}

//========================================================================================
void *task2 (void *dummyPt)
{
bool loop1 = true;
//char bufqnxread[4096];
int IleOdQnx;
int x;
//struct pollfd fd;
//int ret;

time_t t = time(NULL);
struct tm *tm = localtime(&t);
char s[64];
//strftime(s, sizeof(s), "%c", tm);

    while(loop1) {
strftime(s, sizeof(s), "%c", tm);
//	bzero(bufqnxread, 4096);
	bzero(bufqnx2, 4096);
	IleOdQnx = 0;
//	printf("Jestem przed read...%d\n", IleOdQnx );
	IleOdQnx = read(listenFd2, bufqnx2, 4096);
//	printf("Jestem po read...%d\n", IleOdQnx );
//	IleOdQnx = recv(listenFd, bufqnx, 4096, 0);
	if(IleOdQnx <= 0) { loop1 = false; }
//	printf("[...task1...%d]:\n", IleOdQnx );
//	printf(".");
//for(x=0; x<=IleOdQnx; x++) { 
//    bufqnx[x] = bufqnx[x] & 0xff;
//    printf("%02X\n", bufqnx[x]);
//    if(bufqnx[x] > 0xff) { printf("->"); }
//    printf("%02X\n", bufqnx[x]);
//}

	for(x=0; x<IleOdQnx; x++) {
	    if(bufqnx2[x]==0xf1) ramkapos2=0;
	    ramka2[ramkapos2] = bufqnx2[x];
	    ramkapos2++;
//	    printf("%02X ", bufqnx[x]);
	    if(bufqnx2[x]==0xf2) {
//		printf("Task2_ramka WYL\n");
		task2_ramka();
//		printf("\n");
	    }
	} //printf("\n");
//	usleep(1000 * 1000);
	usleep(100);
    }

    ConnectedQNXR=0;
    close(listenFd2);
printf("\33[0;1;33m%s -> ", s );
    printf("\33[0;1;33mREZERWA  Socket Timeout Zamykam [task2]\33[m\n");
}
