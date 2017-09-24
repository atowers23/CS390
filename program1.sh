#!/bin/bash
# Andrew Towers
# Program #1
# A status report program
# This program greets the user, then prints the current date, current
# number of users on the system, current directory, home directory,
# and the amount of space of the files under the user's home directory

date=`date +"%A: %B %d, %Y"` # %A: abbr. weekday %B: month %Y: year
hour=`date +%H` # %H: hour (00 - 23)
numberofusers=`who | wc -l`

if [ $hour -lt 12 ]
then
    timeofday="Morning"
elif [ $hour -lt 17 ]
then
    timeofday="Afternoon"
else
    timeofday="Evening"
fi

if [ $# -eq 1 ] # If this script is called with one argument
then
    name=$1 # name is set to first argument
else
echo -n "Please enter your first name: "
read name
fi

echo ""
echo "Good $timeofday, $name"
echo ""
echo "Today is $date"
echo "There are $numberofusers users on this system today"
echo "Your current directory is `pwd`"
echo "Your home directory is $HOME, and"
set `du -sh $HOME` # set output of du to positional parameters
storagespace=$1 # set storagespace to first string in du output
echo "Your files are using $1 in storage space"
echo ""
