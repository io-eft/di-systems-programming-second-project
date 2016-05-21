#!/bin/bash
queued=($(./jobCommander poll queued))	#get the respose for poll queued
if [ "${queued[0]}" != "No" ]	#if there are jobs in it
then
    for ((i=0; i <= ${#queued[@]} - 1; i=i+2))	
    do	#send a stop order for even numbers in the reply
	./jobCommander stop ${queued[i]}
    done
fi
running=($(./jobCommander poll running))	#same as above for running
if [ "${running[0]}" != "No" ]
then
    for ((i=0; i <= ${#running[@]} - 1; i=i+2))
    do
	./jobCommander stop ${running[i]}
    done
fi
exit 0
