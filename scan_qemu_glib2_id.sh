#!/bin/bash

[ `id -u` -ne 0 -a `id -un` != "qemu" ] &&
	{ echo Need root privilege or be user qemu; exit 1; }

for i in `ps -ef |awk '$1=="qemu"&&$8~/qemu-kvm$/{print $2,$10}'`; do
	[ -z $pid ] && pid=$i && continue
	[ $# -gt 0 ] && 
		{ echo " $* " |grep -Fq " $pid " || { pid=; continue; } }
	echo $pid $i

	read addr glib2_so< <(awk '$NF~/libglib-2.0.so/{split($1,a,"-");print a[1],$NF;exit}' /proc/$pid/maps)

	htag=`md5sum $glib2_so |cut -d" " -f1`
	[ $htag != "b34369d79a25ee3a59cccd1c1944dba8" ] &&
		{ echo -e "qemu-kvm $pid is using a different glib2: $htag\n"; pid=; continue; }

	./glib2_id $pid 0x$addr
	echo

	pid=
done
