# glib2_high_id

Introduction ���
=================

There is a bug in glib2 could causes KVM to crash in certain circumstances(Please refer to our WeChat official account "sys-ops" for more details).  This repo provides a patch for this issue targeting glib2-2.26.1-3.el6 released within RHEL/CentOS 6.5, and a tool to help IT Pro identify and mitigate this issue before they manully shutdown and restart their VMs to reload the new glib2 library.

����RHEL/CentOS���а���glib2����һ��bug����ĳЩ�ض�����½�����KVM��������йػ�����ο�����΢�Ź��ںš���ά�ھ֡�������������˽����ϸ�ڣ�����repoΪRHEL/CentOS  6.5��glib2-2.26.1-3�ṩ����Ӧ��patch��ͬʱ�ṩ�˹��߿ɰ���ϵͳ������Ա���µ�glib2��������Чǰ��λ�������bug������Ӱ�졣

Description ����
================

* glib2-2.26.1-3.el6-source-id-overflow.patch
  ** The patch which fixes the glib2 source id overflow issue.
  ** glib2�¼�Դid���bug��patch�ļ���

* replace_glib2.sh
  ** A wrap shell script to udpate target glib2 library, it checks the system glib2 version to avoid possible error.
  ** �����滻ϵͳ����glib2���ļ��Ľű������л���ϵͳglib2�汾�Ա���Ǳ�ڵķ��ա�

* glib2_id.c
  ** The tool which can 1) scan the target process memory for glib2 IO event loop next id value and all source id value; 2) set the glib2 IO event loop next id to user specified value.
  ** �ù����ṩ���¹��ܣ�1) ɨ��Ŀ�������glib2ά����ȫ���¼�ѭ���е�Դidֵ��2) ����glib2ά����ʱ��ѭ����һԴidֵ��

* scan_qemu_glib2_id.sh
  ** A wrap shell script which leverages the above tool to scan and report glib2 IO event next id value and all source id value in all or specified KVM host process.
  ** ����ɨ����������ȫ����ָ��KVM��Ӧqemu-kvm���̵�glib2�¼�ѭ����һԴidֵ��ȫ���¼�Դidֵ�Ľű���

* adjust_qemu_glib2_id.sh
  ** A wrap shell script which leverages the above tool to adjust glib2 IO event next id value in all or specified KVM host process.
  ** ���ڵ�����������ȫ����ָ��KVM��Ӧ���̵�glib2�¼�ѭ����һԴidֵ�Ľű���