#!/bin/bash

while [true]; do 
  vals=$(shuf -i 0-15)

  echo $vals
  echo -e "\nastar:"
  ./fifteen $vals astar

  if (( $? != 0 )); then
    echo unsolvable.
  else
    echo -e "\nbfs: "
    ./fifteen $vals bfs
    echo -e "\ndfs: "
    ./fifteen $vals dfs
  fi

  wait 1

done