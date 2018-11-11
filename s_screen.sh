#!/bin/sh
echo "******** SCREEN DNP3 **********"
echo "podglad   -> screen -r DNP3"
echo "wyjscie   -> Ctrl+a Ctrl+d"
echo "logowanie -> Ctrl+a H ( screenlog.0 )"
screen -dmS DNP3 ./qnx2dnp warszawa.ini
