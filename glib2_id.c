#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ptrace.h>

#define CTX_LIST_OFFSET			0x304090
#define CTX_ID_OFFSET			0x60
#define CTX_SOURCE_LIST_OFFSET	0x68
#define SOURCE_ID_OFFSET		0x30
#define SOURCE_NEXT_OFFSET		0x48
#define SOURCE_NAME_OFFSET		0x50

int main( int argc, char* argv[] )
{
	long id, target_id;
	int ret, i, j, has_id;
	pid_t pid;
	void *addr, *next, *nsrc, *name;
	char src_name[128];

	if (argc != 3 && argc != 4) {
		fprintf(stderr, "Usage: glib2_id target_pid glib2_addr [set_id_value]\n");
		return -1;
	}

	pid = atoi(argv[1]);
	addr = (void*)strtoul(argv[2], NULL, 16);
	if (argc == 3)
		target_id = 0;
	else if (!(target_id = strtoul(argv[3], NULL, 10))) {
		fprintf(stderr, "Invalid next id value: %s\n", argv[3]);
		return -2;
	}

	if (!pid || !addr) {
		fprintf(stderr, "Invalid target_pid or glib2_addr\n");
		return -3;
	}

	if (-1 == ptrace(PTRACE_ATTACH, pid, NULL, 0)) {
		perror("Failed to attach target process");
		return -4;
	}

	waitpid(pid, NULL, 0);

	next = (void*)ptrace(PTRACE_PEEKDATA, pid, addr + CTX_LIST_OFFSET, 0);
	if (next == (void*)-1) {
		perror("Failed to retrieve context list pointer");
		ret = -5;
		goto done;
	}

	ret = 0;

	if (!next) {
		printf("None context\n");
		goto done;
	}

	do {
		addr = next;
		next = (void*)ptrace(PTRACE_PEEKDATA, pid, addr + 8, 0);
		if (next == (void*)-1) {
			perror("Failed to retrieve next context pointer");
			ret = -6;
			break;
		}

		addr = (void*)ptrace(PTRACE_PEEKDATA, pid, addr, 0);
		id = ptrace(PTRACE_PEEKDATA, pid, addr + CTX_ID_OFFSET, 0);

		printf("%p@%u\n", addr, id >> 32);
		if (target_id) {
			target_id = (id & 0xffffffff) | (target_id << 32);
			if (-1 == ptrace(PTRACE_POKEDATA, pid, addr + CTX_ID_OFFSET, target_id)) {
				perror("Failed to reset context next id");
				ret = -7;
				break;
			}
			printf(" Reset next id to: %u\n", target_id >> 32);
		}

		nsrc = (void*)ptrace(PTRACE_PEEKDATA, pid, addr + CTX_SOURCE_LIST_OFFSET, 0);
		if (!(has_id = (int)nsrc))
			printf(" <NULL>\n");
		else do {
			id = ptrace(PTRACE_PEEKDATA, pid, nsrc + SOURCE_ID_OFFSET, 0);
			name = (void*)ptrace(PTRACE_PEEKDATA, pid, nsrc + SOURCE_NAME_OFFSET, 0);
			if (!name)
				src_name[0] = '\0';
			else {
				for (i = 0; i < sizeof(src_name); i += sizeof(long)) {
					*(long*)(&src_name[i]) = ptrace(PTRACE_PEEKDATA, pid, name + i, 0);
					for (j = 0; j < sizeof(long) && src_name[i + j]; ++j);
					if (j < sizeof(long))
						break;
				}
				src_name[sizeof(src_name) - 1] = '\0';
			}
			printf(" %10u %s\n", id & 0xffffffff, src_name);
		} while ((nsrc = (void*)ptrace(PTRACE_PEEKDATA, pid, nsrc + SOURCE_NEXT_OFFSET, 0)));
	} while (next);

done:

	ptrace(PTRACE_DETACH, pid, NULL, 0);

	return ret;
}
