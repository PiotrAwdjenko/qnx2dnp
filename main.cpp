
#include <iostream>
#include "ini.h"
#include <stdexcept>
//#include <wiringPi.h>

#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/MeasUpdate.h>
#include <asiopal/UTCTimeSource.h>
#include <asiodnp3/ConsoleLogger.h>
#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/ApplicationIIN.h>
//#include <opendnp3/SlaveConfig.h>

#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
//#include <ncurses.h>
#include <stdio.h>
#include <time.h>

#include "Config.h"
#include "GPIOCommandHandler.h"
#include "QNXclient.h"

//#define MaxIndex 1024
//#define QNXBuffSize 4096

//extern pthread_t threadA[];
extern int ConnectedQNXP;
extern int ConnectedQNXR;
extern int portNoP;
extern char *QnxAddressP;
extern int portNoR;
extern char *QnxAddressR;
extern int timeoutP;
extern int timeoutR;

//extern void *task0_timer(void *);
//pthread_t thread0[3];
time_t czas;

int INDEX = 0;
int portDNP = 20000;
int qnxserie = 0;
int dnpidx = 0;
int dnplog = 0;
int qnxdbg = 0;

// prototypes for handling ini file reading
int cfg_handler(void* user, const char* section, const char* name, const char* value);
bool safe_handler(Config& config, const std::string& section, const std::string& name, const std::string& value);

using namespace opendnp3;
using namespace asiodnp3;
using std::string;

//int xxx;
int licznik = 0;

int idx[1024], ss[1024], seria[1024], bit[1024], typ[1024], stan[1024], stanold[1024];
int idx2[1024], ss2[1024], seria2[1024], bit2[1024], typ2[1024], stan2[1024], stanold2[1024];
//bool stan[1024], stanold[1024];
char *opis[1024];
//int idx[MaxIndex], ss[MaxIndex], seria[MaxIndex], bit[MaxIndex], typ[MaxIndex], stan[MaxIndex];
//char *opis[MaxIndex];

//extern int listenFd;
//extern int portNo;

//int odqnx=0;
//char buforqnx[QNXBuffSize];
//char can[QNXBuffSize];
//int n,c;

//int qnxstarter=0, qnxtyp=0, qnxodbiorca=0, qnxnadawca=0, qnxrozmiar=0, qnxseria=0, qnxtresc=0, qnxcrc=0, qnxstop=0;
//  0             1         2,3            4,5           6,7           8           9,10        11,12     13

//=======================================================================================
//int ramka(int data) {
//    int x;

//    qnxnadawca = can[5]; qnxnadawca = (qnxnadawca <<8) | can[4];
//    qnxseria   = can[8];
//    qnxtresc   = can[10]; qnxtresc  = (qnxtresc   <<8) | can[9];

//    printf("[Jestem w ramka]\n");

//    printf("QNX -> %3d - %02X:%04X\n", qnxnadawca, qnxseria, qnxtresc );
//}

//=======================================================================================
//int paczka(int data) {
//    int x;//,n,c;
    
//    n=0; c=0;
//    for(x=0; x<data;x++) {
//	printf("%02X ", buforqnx[x]);
//	if(buforqnx[x]==0xf2) printf("\n");
//    } printf("\n");
//    printf("[Jestem w paczka:n%d,c%d]\n",n,c);
//    while(n < data) {
//	can[c] = buforqnx[c];
//	if( buforqnx[n] == 0xf0) { can[c]=buforqnx[n] | buforqnx[n+1]; n++; }
//
//	if( buforqnx[n] == 0xf2) {
//	    can[c]=buforqnx[n];
//	    ramka(c+1);
//    printf("[Jestem w paczka:pqnx%d,ramka%d]\n",n,c);

//	    printf("Paczka: ");
//	    for(x=0; x<=c; x++) {
//		printf("%02X ", can[x] );
//	    } printf("\n");
//
//	    c=-1;
//	}
//	
//	c++; n++;
//    }
//}

//=========================================================================================
void ConfigureDatabase(DatabaseConfigView view)
{
    int x;
	// example of configuring analog index 0 for Class2 with floating point variations by default
//	view.analogs[0].variation = StaticAnalogVariation::Group30Var5;
//	view.analogs[0].metadata.clazz = PointClass::Class2;
//	view.analogs[0].metadata.variation = EventAnalogVariation::Group32Var7;
//      Index 0 PE1 STAN
    for(x=0; x< licznik; x++) {
	view.binaries[x].variation = StaticBinaryVariation::Group1Var1;
	view.binaries[x].metadata.clazz = PointClass::Class1;
	view.binaries[x].metadata.variation = EventBinaryVariation::Group2Var2;
//	view.binaries[x].metadata.variation = EventBinaryVariation::Group2Var3; // Time
    }
//      Index 1 PE1 OK
//	view.binaries[1].variation = StaticBinaryVariation::Group1Var1;
//	view.binaries[1].metadata.clazz = PointClass::Class1;
//	view.binaries[1].metadata.variation = EventBinaryVariation::Group2Var2;

}

//opendnp3::IINField XYZ;


int main(int argc, char *argv[])
{
int x;
	if(argc != 2)
	{
		std::cerr << "usage: qnx2dnp <ini file path>" << std::endl;
		return -1;
	}

	auto filename = argv[1];

	Config config;
	if(ini_parse(filename, cfg_handler, &config) != 0)
	{
		std::cerr << "error parsing ini file: " << filename << std::endl;
		return -1;
	}

	// Initialize wiringPi -- using Broadcom pin numbers
	// this call automatically terminates program with an error msg if the setup cannot be completed
//	wiringPiSetupGpio();

	// setup inputs and outputs
//	for(auto pin : config.inputs) {
//		pinMode(pin, INPUT);
//		std::cout << "Index " << static_cast<int>(pin) << " set as: " << std::endl;
//	}

//	for(auto pin : config.outputs) {
//		pinMode(pin, OUTPUT);
//		std::cout << "pin " << static_cast<int>(pin) << " set as OUTPUT" << std::endl;
//	}

	GPIOCommandHandler commandHandler(config.outputs);
	auto LOG_LEVELS = levels::NORMAL;

//#ifdef DNPLOG
	if ( dnplog == 2 ) { LOG_LEVELS = levels::NORMAL | levels::ALL_APP_COMMS; }
//#endif
	if ( dnplog == 1 ) { LOG_LEVELS = levels::NORMAL | levels::ALL_COMMS; }
//#ifndef DNPLOG
	if ( dnplog == 0 ) { LOG_LEVELS = levels::NORMAL; }
//#endif

	DNP3Manager manager(1, ConsoleLogger::Create());

	auto channel = manager.AddTCPServer("server", LOG_LEVELS, ChannelRetry::Default(), "0.0.0.0", portDNP);

	config.stack.dbTemplate = DatabaseTemplate::BinaryOnly(config.inputs.size());
	config.stack.outstation.eventBufferConfig = EventBufferConfig(licznik);
	config.stack.outstation.params.allowUnsolicited = true;
//	config.stack.outstation.allowTimeSync = true;
//	config.stack.outstation.params.timeSyncPeriod = 60000;


	auto outstation = channel->AddOutstation("outstation", commandHandler, DefaultOutstationApplication::Instance(), config.stack);

	ConfigureDatabase(outstation->GetConfigView());

	outstation->Enable();

	std::cout << "DNP3 Sample (ms): " << config.sample_period_ms << std::endl;
        const auto SAMPLE_PERIOD = std::chrono::milliseconds(config.sample_period_ms);

	std::cout << "DNP3 Local Outstation address: " << config.stack.link.LocalAddr << std::endl;
	std::cout << "DNP3 Remote Master address: " << config.stack.link.RemoteAddr << std::endl;
	std::cout << "DNP3 Local Port: " << portDNP << std::endl;
//        const auto SAMPLE_PERIOD = std::chrono::milliseconds(config.sample_period_ms);

//extern int listenFd;
//extern int portNo;
//int odqnx=0;
//char buforqnx[4096];

//ConectQNX();

//	pthread_create(&thread0[0], NULL, task0_timer, NULL );
int pg=0;
printf("\33[=3h\33[2J"); // Consola 80x25 color
//initscr();
//raw();
//cbreak();
//printf("\33[0;0;36m");
	while(true) {
		uint16_t index = 0;

//		    printf("\33[s\33[0;1;31m\33[1;1H%d\33[m\33[u", pg);
		    pg++;
		    if(pg==10) pg=0;

		DNPTime time(asiopal::UTCTimeSource::Instance().Now().msSinceEpoch);

		MeasUpdate update(outstation);

//opendnp3::IINField::WriteHandler::pWriteIIN(opendnp3::IINBit::NEED_TIME);
//ApplicationIIN().needTime < true;
//opendnp3::IINField XYZ;
//XYZ.SetBit(opendnp3::IINBit::NEED_TIME);
//opendnp3::ApplicationIIN XXY;
//XXY.needTime = true;
//printf("TS:%d\n", (int)XXX.needTime );
//opendnp3::IINField XXX;
//XXX.SetBitToValue(opendnp3::IINBit::NEED_TIME, true); // NEED_TIME
//printf("TS1:%d\n", (int)XXX.IsSet(opendnp3::IINBit::NEED_TIME) );
//opendnp3::ApplicationIIN XXY;
//printf("TS2:%d\n", (int)XXY.needTime );
		if(ConnectedQNXP==0) {
//		    printf("\33[s\33[0;1;31m\33[5;79HP\33[m\33[u");
		    usleep(1000 * 1000);
		    ConectQNXP();
//		} else {
//		    printf("\33[s\33[0;1;32m\33[5;79HP\33[m\33[u");
		}

		if(ConnectedQNXR==0) {
//		    printf("\33[s\33[0;1;31m\33[5;80HR\33[m\33[u");
		    usleep(1000 * 1000);
		    ConectQNXR();
//		} else {
//		    printf("\33[s\33[0;1;32m\33[5;80HR\33[m\33[u");
		}

		if((ConnectedQNXP==0)&(ConnectedQNXR==0)) {
		    update.Update(Binary(0, 0x01, time), 0); 
		    printf("\33[0;1;31mQNX NOT CONNECT\33[m\n");
		    usleep(1000 * 1000);
		} else {
		    update.Update(Binary(1, 0x01, time), 0); 
		}
//		    printf("QNX not connect\n");
//		    usleep(1000 * 1000); }

//	printf("[po MeasUpdate]\n");
//		bzero(buforqnx, QNXBuffSize);
//		odqnx = read(listenFd, buforqnx, QNXBuffSize);

//		printf("[od_QNX]:%d\n", odqnx);
//		if(odqnx<=0) { 
//		    update.Update(Binary(0x00, 0x01, time), 0);
//		    close(listenFd);
//		    ConectQNX(); 
//		} else {
//		    update.Update(Binary(0x01, 0x01, time), 0);
//		}
//		paczka(odqnx);
//	printf("[po paczce]\n");
//		uint16_t index = 0;
//		DNPTime time(asiopal::UTCTimeSource::Instance().Now().msSinceEpoch);

//		MeasUpdate update(outstation);

//		for(auto pin : config.inputs) {
//			bool value = 0x01;
//			++index;
//		}

//	pthread_join(threadA[0], NULL);
//		printf("Pause ->\n");
//		bool value = !value;

//		update.Update(Binary(0x01, 0x01, time), 0); 

//		std::this_thread::sleep_for(SAMPLE_PERIOD);

//		update.Update(Binary(0x00, 0x01, time), 0); 

//****************************************************************************************
		INDEX = licznik-1;
//		printf("DNP3 Index: %d\n", licznik-1 );
//****************************************************************************************

	if(ConnectedQNXP==1) {
		for(x=1; x<=INDEX; x++) {
		    if(stan[x] != stanold[x]) {
//			printf("DNP3 ZMIANA Idx:%d [%d->%d]\n", x, stanold[x], stan[x]);
//			time(&czas);
//			char* Czas = ctime(&time);
//#ifdef DNPIDX
			if ( dnpidx ) printf("\33[0;1;36mIdx:%d [%d->%d]\33[m\n", x, stanold[x], stan[x]);
//#endif
//			refresh();
			update.Update(Binary(stan[x], 0x01, time), x);
			stanold[x] = stan[x];
		    } //printf("\n");
		}
	} else {
		for(x=1; x<=INDEX; x++) {
		    if(stan2[x] != stanold2[x]) {
//			printf("DNP3 ZMIANA Idx:%d [%d->%d]\n", x, stanold[x], stan[x]);
//#ifdef DNPIDX
			if ( dnpidx ) printf("\33[0;0;36mIdx:%d [%d->%d]\33[m\n", x, stanold2[x], stan2[x]);
//#endif
//			refresh();
			update.Update(Binary(stan2[x], 0x01, time), x);
			stanold2[x] = stan2[x];
		    } //printf("\n");
		}
	}

		// determines the sampling rate
		std::this_thread::sleep_for(SAMPLE_PERIOD);
	}
//endwin();
}

//int idx[MaxIndex], ss[MaxIndex], seria[MaxIndex], bit[MaxIndex], typ[MaxIndex], stan[MaxIndex];
//char *opis[MaxIndex];
//int licznik = 0;
bool safe_handler(Config& config, const std::string& section, const std::string& name, const std::string& value)
{
	try
	{		
		if(section == "binary")
		{
			const char * n = name.c_str();
			const char * v = value.c_str();
//			std::cout << "Name: " << name << " Value:" << value << "\n";
//			printf("INC Name: %s, Value: %s\n", n, v);

			sscanf(n,"%d", &idx[licznik] );
			sscanf(n,"%d", &idx2[licznik] );
			sscanf(v,"%d,%d,%d,%d;%s", &ss[licznik], &seria[licznik], &bit[licznik], &typ[licznik], opis );
			sscanf(v,"%d,%d,%d,%d;%s", &ss2[licznik], &seria2[licznik], &bit2[licznik], &typ2[licznik], opis );
			printf("DNP3 - Index:%3d; Stanowisko:%3d; Seria:%3d; Bit:%3d; Typ:%3d - %s \n", idx[licznik], ss[licznik], seria[licznik], bit[licznik], typ[licznik], opis );
			licznik++;

			return config.AddInput(std::stoi(name));
		}
//		else if(section == "output")
//		{
//			return config.AddOutput(std::stoi(name));
//		}
		else if(section == "program")
		{
			if(name == "sample-period-ms")
			{
				config.sample_period_ms = std::stoi(value);
				return true;
			}
			else if(name == "qnx-serie")
			{
				qnxserie = std::stoi(value);
				return true;
			}
			else if(name == "dnp-idx")
			{
				dnpidx = std::stoi(value);
				return true;
			}
			else if(name == "dnp-log")
			{
				dnplog = std::stoi(value);
				return true;
			}
			else if(name == "qnx-dbg")
			{
				qnxdbg = std::stoi(value);
				return true;
			}
		}
		else if(section == "dnp3")
		{
			if(name == "remote-addr")
			{
				config.stack.link.RemoteAddr = std::stoi(value);
				return true;
			}
			else if(name == "local-addr")
			{
				config.stack.link.LocalAddr = std::stoi(value);
				return true;
			}
			else if(name == "local-port")
			{
				portDNP = std::stoi(value);
				return true;
			}
		}
		else if(section == "P-qnx")
		{
			if(name == "P-address")
			{
				string qnxadr;
				qnxadr = std::string(value);
				QnxAddressP = strdup(qnxadr.c_str());
				printf("QNX Podstawa Address: %s\n", QnxAddressP );
				return true;
			}
			else if(name == "P-port")
			{
				portNoP = std::stoi(value);
				printf("QNX Podstawa Port: %d\n", portNoP );
				return true;
			}
			else if(name == "P-timeout")
			{
				timeoutP = std::stoi(value);
				printf("QNX Podstawa timeout: %d\n\n", timeoutP );
				return true;
			}
		}
		else if(section == "R-qnx")
		{
			if(name == "R-address")
			{
				string qnxadr;
				qnxadr = std::string(value);
				QnxAddressR = strdup(qnxadr.c_str());
				printf("QNX Rezerwa Address: %s\n", QnxAddressR );
				return true;
			}
			else if(name == "R-port")
			{
				portNoR = std::stoi(value);
				printf("QNX Rezerwa Port: %d\n", portNoR );
				return true;
			}
			else if(name == "R-timeout")
			{
				timeoutR = std::stoi(value);
				printf("QNX Rezerwa timeout: %d\n\n", timeoutR );
				return true;
			}
		}

		std::cerr << "Unknown parameter, section: " << section << " name: " << name << " value: " << value << std::endl;
		return false;
	}
	catch(std::invalid_argument)
	{
		std::cerr << "Bad integer conversion, section: " << section << " name: " << name << " value: " << value << std::endl;
		return false;
	}
}

int cfg_handler(void* user, const char* section, const char* name, const char* value)
{
	return safe_handler(*(Config*)user, section, name, value) ? 1 : 0;
}
