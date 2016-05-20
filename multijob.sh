#!/bin/bash
for j in ${*:1}	#for every file given
do
    if [ -e $j ]	#if it exists
    then if [ -f $j ]	#and it's a file
	then if [ -r $j ]	#and i can read it
		then
		    exec < "$j" #Take input from this file

		    while read line	#while it has lines
	  	    do
	  		./jobCommander issuejob $line	#execute the commander with them
	  	    done
		else	#if you cannot read it
		   echo "I cannot read this file"
		fi
	    else	#if it's not a file
		echo "This is not a file"
	    fi
	else	#if it doesn't exist
	    echo "This file does not exist"
	fi
done	#end iwth the for
exit 0
