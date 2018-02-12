#!/usr/bin/env bash
bourre()
{
  while [ 1 ]; do
    echo -n 'some string to telnet' | nc $1 $2 -w0;
  done
}

for i in $(seq 1 7); do bourre $1 $2 & done
