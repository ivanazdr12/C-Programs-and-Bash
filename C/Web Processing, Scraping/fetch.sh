#!/bin/bash

index_page="https://finance.yahoo.com/most-active/"

timer=0
while [ $timer -lt 60 ]
do
    current_date=`date +"%Y_%m_%d_%H_%M_%S"`
    html_fn="yahoo_${current_date}.html"
    wget -O $html_fn $index_page
    java -jar tagsoup-1.2.1.jar --files --files $html_fn
    xhtml_fn=${html_fn/.html/.xhtml}
    python3 hw8.py $xhtml_fn
    sleep 60
    timer=$[$timer+1]
done 
