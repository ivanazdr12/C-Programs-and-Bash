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

echo "">output3.csv;
echo "">title.txt;
echo "">user.txt;
echo "">upload_time.txt;
echo "">duration.txt;
echo "">views.txt;

function commands(){
echo "Title	User	Time	Duration	Views" >> output3.csv
sed -n 's/\(.*\) by \(.*\) \([0-9].*\) ago \([0-9].*\) \([0-9].*\) views$/\ "\1\", \ "\2\", \ "\3\", \ "\4\", \ "\5\" /p' $filename >> output3.csv
}

function main(){
	commands;
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

	for ((i=0; i<${#user_list[@]}; i++)); do
		echo "${title_list[$i]},${user_list[$i]},${upload_time_list[$i]},${duration_list[$i]}, ${views_list[$i]}" >> output3.csv;
	done;
}
main
