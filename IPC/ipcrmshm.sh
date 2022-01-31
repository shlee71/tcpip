#!/bin/bash -f
#ipcrmshm.sh
export my_account=`whoami`
echo
echo "### ipcrm_shm_start ###"
echo "(BEF)"
ipcs -mp | grep -E "shmid|$my_account|------"
echo --------------------------------------------------
#set usr_semid = `ipcs -m | grep $my_account | awk '{print $2}'`
export usr_semid=`ipcs -m | grep $my_account | awk '{print $2}'`

for i in $usr_semid
do
  ipcrm -m $i
  echo "$i shm purged"
done

export dest_semid=`ipcs -mp | grep $my_account | awk '{print $3}'`

echo $dest_semid

for i in $dest_semid
do
  kill -15 i
  echo "$i shm killed"
done
echo "(AFT)"
ipcs -mp | grep -E "shmid|$my_account|------"
echo --------------------------------------------------
echo "### ipcrm_shm_end   ###"
