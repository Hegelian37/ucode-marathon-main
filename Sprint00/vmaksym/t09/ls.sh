ls -lh $1 | awk '{print $9, $5, $6}' | sort -s
