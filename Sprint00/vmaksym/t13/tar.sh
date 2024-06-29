if [ $1 == "-c" ]; then tar -c -f "$2.tar" $3
	exit 0
fi
if [ $1 == "-e" ]; then	tar -x -f $2
	exit 0
fi
