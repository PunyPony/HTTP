#!/usr/bin/env bash
bourre()
{
  while true; do
    #echo -n 'some string to telnet' | nc $1 $2 -w0;
    curl -i --connect-timeout 1 http://$1:$2 
    sleep 0.1
  done
}

for i in $(seq 1 7); do bourre $1 $2 & done


# sleep 20;
# pkill -f multi;
# pkill -f curl;
