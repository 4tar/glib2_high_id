# glib2_high_id

Introduction 简介
=================

There is a bug in glib2 could causes KVM to crash in certain circumstances(Please refer to our WeChat official account "sys-ops" for more details).  This repo provides a patch for this issue targeting glib2-2.26.1-3.el6 released within RHEL/CentOS 6.5, and a tool to help IT Pro identify and mitigate this issue before they manully shutdown and restart their VMs to reload the new glib2 library.

现有RHEL/CentOS发行版中glib2存在一个bug，在某些特定情况下将导致KVM虚拟机自行关机（请参考我们微信公众号“运维镖局”的相关文章以了解具体细节）。本repo为RHEL/CentOS  6.5的glib2-2.26.1-3提供了相应的patch，同时提供了工具可帮助系统管理人员在新的glib2被加载生效前定位、缓解该bug带来的影响。

Description 描述
================

* glib2-2.26.1-3.el6-source-id-overflow.patch
  ** The patch which fixes the glib2 source id overflow issue.
  ** glib2事件源id溢出bug的patch文件。

* replace_glib2.sh
  ** A wrap shell script to udpate target glib2 library, it checks the system glib2 version to avoid possible error.
  ** 用于替换系统现有glib2库文件的脚本，其中会检查系统glib2版本以避免潜在的风险。

* glib2_id.c
  ** The tool which can 1) scan the target process memory for glib2 IO event loop next id value and all source id value; 2) set the glib2 IO event loop next id to user specified value.
  ** 该工具提供如下功能：1) 扫描目标进程中glib2维护的全部事件循环中的源id值；2) 设置glib2维护的时间循环下一源id值。

* scan_qemu_glib2_id.sh
  ** A wrap shell script which leverages the above tool to scan and report glib2 IO event next id value and all source id value in all or specified KVM host process.
  ** 用于扫描宿主机上全部或指定KVM对应qemu-kvm进程的glib2事件循环下一源id值及全部事件源id值的脚本。

* adjust_qemu_glib2_id.sh
  ** A wrap shell script which leverages the above tool to adjust glib2 IO event next id value in all or specified KVM host process.
  ** 用于调整宿主机上全部或指定KVM对应进程的glib2事件循环下一源id值的脚本。