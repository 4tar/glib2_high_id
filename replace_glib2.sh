#!/bin/bash

[ $# -eq 1 -a "$1" == "-h" ] && {
	echo -e "\nUsage: replace_glib2.sh [log]\n\n" \
		"  The script replaces RHEL/CentOS 6.5 glib2 (/lib64/libglib-2.0.so.0)\n"\
		"  with our version which fixes the id overflow issue.\n\n"\
		"  With the optional [log] argument, the new version of glib2 would\n"\
		"  log every id increment in a log file under /dev/shm/.\n\n"\
		"  A VM restart is required to take effect.\n\n"\
		"  The original glib2 would be backuped with a .bak extension.\n\n"\
		"  Root privilege is needed.\n"
	exit
}

[ `id -u` -ne 0 ] &&
	{ echo Need root privilege to replace target glib2; exit 1; }

htag=`md5sum /lib64/libglib-2.0.so.0.2600.1 |cut -d" " -f1`
[ $htag != "b34369d79a25ee3a59cccd1c1944dba8" ] &&
	{ echo -e "Current system holds a different glib2: $htag\n"; exit 2; }

[ $# -gt 0 -a "$1" == "log" ] && new_glib2=libglib-2.0.so.0.2600.1.log || new_glib2=libglib-2.0.so.0.2600.1
mv -f /lib64/libglib-2.0.so.0.2600.1{,.bak} && cp $new_glib2 /lib64/libglib-2.0.so.0.2600.1
