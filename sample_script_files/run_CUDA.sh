#!/bin/bash

# please change pool address, wallet address and worker ID to yours
# adjust -b & -t value as described in the README and FAQ
worker=""
pool="http://mine.arionumpool.com"
wallet="2qFmhU3pHSJddgHYx9Bg5QEdmXHpiN84GpRYzAzpwj8fWvCsZWNoQS6sZiy5mH8VASomkUzrcKG7iGRj94fnFW7k"
threads="1"
batches="48"

# set this to false if you do not want miner to auto relaunch after crash
relaunch_miner_on_crash="true"

while :
do
	# -u means use all device, you can also use -d to specify list of devices (ex: -d 0,2,5)
	./arionum-cuda-miner -u -b "$batches" -t "$threads" -p "$pool" -a "$wallet" -i "$worker" --skip-cpu-blocks

	if [ "$relaunch_miner_on_crash" = "true" ]; then
		echo "miner crashed, relaunching in 5 seconds ..."
		sleep 5
	else
		break
	fi
done
