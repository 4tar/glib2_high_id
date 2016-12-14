#!/bin/bash

[ $# -ne 2 ] &&
	{ echo Usage: adjust_qemu_glib2_id.sh qemu-kvm-pid next-id-value; exit 1; }

[ `id -u` -ne 0 -a `id -un` != "qemu" ] &&
	{ echo Need root privilege or be user qemu; exit 2; }

vm=`ps -ef |awk '$1=="qemu"&&$2=='$1'&&$8~/qemu-kvm$/{print $10}'`
echo $1 $vm

read addr glib2_so< <(awk '$NF~/libglib-2.0.so/{split($1,a,"-");print a[1],$NF;exit}' /proc/$1/maps)

htag=`md5sum $glib2_so |cut -d" " -f1`
[ $htag != "b34369d79a25ee3a59cccd1c1944dba8" ] &&
	{ echo -e "qemu-kvm $1 is using a different glib2: $htag\n"; continue; }

./glib2_id $1 0x$addr $2
