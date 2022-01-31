#!/usr/bin/bash

echo "DP Interface : pos snd/rcv:25001,25003 atm snd/rcv:25002,25004"
echo "Proto Recv-Q Send-Q Local Address           Foreign Address         State"
netstat -an|grep tcp|grep -E "25001|25002|25003|25004"|sort

echo 
echo "HSM Interface : 1500"
echo "Proto Recv-Q Send-Q Local Address           Foreign Address         State"
netstat -an|grep tcp|grep 1500

echo 
echo "FHM Interface : 22571"
echo "Proto Recv-Q Send-Q Local Address           Foreign Address         State"
netstat -an|grep tcp|grep 22571

echo 
echo "HOST Interface : 22570"
echo "Proto Recv-Q Send-Q Local Address           Foreign Address         State"
netstat -an|grep tcp|grep 22570

echo 
echo "ps : hstifs hsmif atmdp posdp fhmifs hsm_server"
ps -ef|grep -E "hstifs|hsmif|atmdp|posdp|fhmifs|hsm_server"|grep -v grep
