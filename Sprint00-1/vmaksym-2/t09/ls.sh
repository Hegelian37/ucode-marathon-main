ls $1 -A -lh | tail -n +2 | awk '{print $9 " " $5}'
