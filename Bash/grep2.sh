#!/bin/bash

filename='clips.txt'

title_list=();
user_list=();
upload_time_list=();
duration_list=();
views_list=();

title_count=0;
user_count=0;
time_count=0;
duration_count=0;
views_count=0;

echo "">output2.csv;
echo "">title.txt;
echo "">user.txt;
echo "">upload_time.txt;
echo "">duration.txt;
echo "">views.txt;

function title(){
	grep -P -o '.*(?= by)' $filename >> title.txt
}

function user(){
	grep -P -o '(?<= by ).*(?= [0-9].* ago)' $filename >> user.txt
}

function upload_time(){
	grep -P -o '[0-9]* (seconds?|minutes?|hours?|days?|months?|years?)(?= ago)' $filename >> upload_time.txt
}

function duration(){
	grep -P -o '(?<= ago ).*(?= [0-9].*)' $filename | tr "," " " >> duration.txt
}

function views(){
	grep -P -o '([,0-9]+)(?= views)' $filename	 | tr "," " " | sed 's/ //g' >> views.txt;
}

function main(){
	title;
	user;
	upload_time;
	duration;
	views;
	echo "OUTPUT SAVED!";

	while read line; do
		title_list[$title_count]=$line;
		((title_count++));
	done < title.txt;
		
	while read line; do
		user_list[$user_count]=$line;
		((user_count++));
	done < user.txt;

	while read line; do
		upload_time_list[$time_count]=$line;
		((time_count++));
	done < upload_time.txt;

	while read line; do
		duration_list[$duration_count]=$line;
		((duration_count++));
	done < duration.txt;

	while read line; do
		views_list[$views_count]=$line;
		((views_count++));
	done < views.txt;

	echo "Title	User	Time	Duration	Views" >> output2.csv;
	for ((i=0; i<${#user_list[@]}; i++)); do
		echo "${title_list[$i]},${user_list[$i]},${upload_time_list[$i]},${duration_list[$i]}, ${views_list[$i]}" >> output2.csv;
	done;
}
main

