/*
	linkusage.c

	Copyright (c) 2014 Rahul Bedarkar <rahulbedarkar89@gmail.com>

	This file is part of Linkusage.

    Linkusage is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Linkusage is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

 */


#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_STAT_INFO_BYTES	50
#define MAX_IF_NAME_BYTES	16

typedef enum {
	speed,
}network_info_t;

static int print_network_stat(const char *if_name, network_info_t type);
static int get_network_stat (const char *if_name, unsigned int *bytes_rx,
	unsigned int *bytes_tx);

static void print_usage(const char *program)
{
	fprintf(stderr, "Usage: %s interface\n"
		"\tshows link speed.\n"
		, program);
}

static int print_network_stat(const char *if_name, network_info_t type)
{
	unsigned int bytes_rx, bytes_tx;
	unsigned int p_bytes_rx, p_bytes_tx;
	char stat_buf[MAX_STAT_INFO_BYTES];

	if (type == speed)
	{
		unsigned int rx, tx;
		int ret;

		ret = get_network_stat(if_name, &p_bytes_rx, &p_bytes_tx);

		if (ret)
			return ret;

		sleep(1);

		ret = get_network_stat(if_name, &bytes_rx, &bytes_tx);

		if (ret)
			return ret;

		rx = bytes_rx - p_bytes_rx;
		tx = bytes_tx - p_bytes_tx;

		if( (rx >= 0 && rx < 1000) || (tx >= 0 && tx < 1000))
		{
			sprintf(stat_buf, "%s Rx %d b/s: Tx %d b/s",
				if_name, rx, tx);
		}
		else
		{
			sprintf(stat_buf, "%s Rx %.2f Kb/s: Tx %.2f Kb/s",
				if_name, (float) rx/1024, (float) tx/1024);
		}

		fprintf(stdout, "\033[H\033[J%s", stat_buf);
		fflush(stdout);
	}

	return 0;
}

static int get_network_stat (const char *if_name, unsigned int *bytes_rx,
	unsigned int *bytes_tx)
{
	FILE *fp;
	char interface[MAX_IF_NAME_BYTES];
	char *buffer = NULL;
	size_t len = 0;
	int found = 0;
	unsigned int p_bytes_rx, p_bytes_tx;

	fp = fopen("/proc/net/dev", "r");

	if (fp == NULL)
	{
		fprintf(stdout, "failed to read network interface\n");
		return -1;
	}

	while (getline(&buffer, &len, fp) != -1)
	{
		int ret = sscanf(buffer, "%s %u"
			"%*d %*d %*d %*d"
			"%*d %*d %*d %u"
			"%*d %*d %*d %*d"
			"%*d %*d %*d %*d",
			interface, &p_bytes_rx, &p_bytes_tx);
		
		if (ret >= 3 && !strncmp(interface, if_name, strlen(if_name)))
		{
			*bytes_tx = p_bytes_tx;
			*bytes_rx = p_bytes_rx;

			found = 1;
			break;
		}
	}

	free(buffer);
	fclose(fp);

	if (found)
		return 0;
	else
		return -1;
}

int main(int argc, char **argv)
{
	int num_bytes;
	char if_name[MAX_IF_NAME_BYTES];
	network_info_t type = speed;

	if (argc != 2)
	{
		fprintf(stderr,"network interface name missing\n");
		print_usage(argv[0]);
		return 0;
	}

	num_bytes = strlen(argv[1]);

	strncpy(if_name, argv[1], num_bytes+1);

	if_name[num_bytes] = '\0';

	while (1)
	{
		int ret = print_network_stat(if_name, type);

		if (ret)
		{
			fprintf(stderr, "%s: no such network interface\n", if_name);
			exit(ret);
		}
	}

	return 0;
}
