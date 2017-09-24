#!/bin/bash
# Andrew Towers
# Program 2
# This program monitors the current directory for any .pdf files
# that have been added. Every 60 seconds any .pdf files that have
# been added since the last output will be printed to the screen


ls -FA | grep -i "\.pdf$" | sort >/tmp/.lastcheck # Initialize contents of file temporary file .lastcheck with sorted list of current .pdf files in directory
while true # Infinite Loop
do
    sleep 60 # Wait 60 seconds
    ls -FA | grep -i "\.pdf$" | sort >/tmp/.sincesleep # Check directory for .pdf files and save sorted output into temporary file .sincesleep
    comm -23 /tmp/.sincesleep /tmp/.lastcheck  # Compare contents of current directory and directory 60 seconds ago, print only files that have been added
    cp /tmp/.sincesleep /tmp/.lastcheck # Set .lastcheck to contents of .sincesleep so comparison will always be done with last read
done
