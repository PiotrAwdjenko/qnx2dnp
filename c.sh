#!/bin/sh
#
# define
# QNXSERIE	Wyswietlanie serii odebranych z QNX
# DNPIDX	Wyswietlanie zmian na indexach
# DNPLOG	Wyswietlanie MAX logow DNP
#
#g++ -o qnx2dnp -std=c++11 -lasiodnp3 -lasiopal -lopendnp3 -lopenpal -lpthread main.cpp ini.c Config.cpp QNXclient.c GPIOCommandHandler.cpp
g++ -o qnx2dnp -DDNPIDX -DDNPLOG -std=c++11 -lasiodnp3 -lasiopal -lopendnp3 -lopenpal -lpthread main.cpp ini.c Config.cpp QNXclient.c GPIOCommandHandler.cpp
