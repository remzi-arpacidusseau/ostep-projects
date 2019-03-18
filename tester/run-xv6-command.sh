#! /usr/bin/env expect

# turn off timeout (perhaps make this flexible later)
set timeout -1

# build and launch xv6 on qemu
spawn make qemu-nox

# wait for initial prompt
expect "init: starting sh\r"
expect "$ "

# send command as per command line
send "$argv\r"

# wait for it to be done
expect "$ "

# send command to halt qemu (ctrl-a x)
# how to do so found here:
# https://stackoverflow.com/questions/27050473/how-to-send-ctrl-a-then-d-in-expect
send "\x01"; send "x"

# make sure to wait for it all to stop
# (without this, script was terminating before qemu quit -> bad)
interact








