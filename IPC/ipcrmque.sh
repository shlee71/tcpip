#!/bin/bash -f
#ipcrmque.sh

export my_account=`whoami`
echo "### ipcrm_que_start ###"

echo "(BEF)"
ipcs -q | grep -E "msqid|$my_account|------"
echo -----------------------------------------------------------------
#set usr_msqid = `ipcs -q | grep $my_account | awk '{print $2}'`
export usr_msqid=`ipcs -q | grep $my_account | awk '{print $2}'`

echo $usr_msqid
for i in $usr_msqid
do
  ipcrm -q $i
  echo "$i queue purged"
done

echo "(AFT)"
ipcs -q | grep -E "msqid|$my_account|------"
echo -----------------------------------------------------------------

echo "### ipcrm_que_end   ###"
echo