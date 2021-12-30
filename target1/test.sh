#!/bin/bash
COLOR='\E[1;33m'

NOCOLOR='\E[0m'
echo -e "${COLOR}\n=====   Phase1   =====${NOCOLOR}"
./ctarget -q < ctarget.l1.key
echo -e "${COLOR}\n=====   Phase2   =====${NOCOLOR}"
./ctarget -q < ctarget.l2.key
echo -e "${COLOR}\n=====   Phase3   =====${NOCOLOR}"
./ctarget -q < ctarget.l3.key
echo -e "${COLOR}\n=====   Phase4   =====${NOCOLOR}"
./rtarget -q < rtarget.l1.key
echo -e "${COLOR}\n=====   Phase5   =====${NOCOLOR}"
./rtarget -q < rtarget.l2.key
