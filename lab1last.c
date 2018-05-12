#include "unistd.h"
#include "stdio.h"
#include "dirent.h"
#include "fcntl.h"
#include "sys/types.h"
#include "string.h"
#include "sys/stat.h"
#include "stdlib.h"
#include "errno.h"

#define BS 1024  //размер буфера, через котороый считывают
#define BS3 256  //маленький буфер для имени

void Pack(char *dir)
{
	DIR *d = NULL;   //поток 
	struct dirent *sd; //сркуктура дирректории
	struct stat ss; //структура имени 
	int f, in, nread; // f и in это возвращаемые значения функции open
	char buff[BS];
	size_t l; //размер имени
	off_t l1; //размер файла

	d = opendir(dir); // открытие дирректории
	if (d == NULL) 
	{
		write(1, "Error opening Dir\n", 18);
		exit(11); //
	}
	
	f = open("MAIzipper", O_WRONLY | O_CREAT, 0775);
	chdir(dir);
	if (f < 0) 
	{
		write(1, "Error opening file MAIzipper\n", 23);
		exit(12);
	}
	while ((sd = readdir(d)) != NULL) 
	{
		if (!(lstat(sd->d_name, &ss) == 0)) 
	{
			write(1, "Error\n", 6);
			exit(13);
	}
		if (S_ISDIR(ss.st_mode)) 
		{
			if (strcmp(".", sd->d_name) == 0 ||
				strcmp("..", sd->d_name) == 0)
				continue;
			l = strlen(sd->d_name);
			if (write(f, "/", 1) == -1) {
				write(1, "Error writing 1\n", 16);
				exit(14);
			}
			if (write(f, &l, sizeof(l)) == -1) {
				write(1, "Error writing 2\n", 16);
				exit(14);
			}
			if (write(f, &(sd->d_name), l) == -1) {
				write(1, "Error writing 3\n", 16);
				exit(14);
			}
		} else
			if ((strcmp("MAIzipper", sd->d_name)) != 0) {
				l = strlen(sd->d_name);
				if (write(f, "*", 1) == -1) 
				{
					write(1, "Error writing\n", 14);
					exit(14);
				}
				if (write(f, &l, sizeof(l)) == -1) 
				{
					write(1, "Error writing\n", 14);
					exit(14);
				}
				if (write(f, sd->d_name, l) == -1) 
				{
					write(1, "Error writing\n", 14);
					exit(14);
				}
				if (write(f, &ss.st_size,
					sizeof(ss.st_size)) == -1) 
				{
					write(1, "Error writing\n", 14);
					exit(14);
				}
				in = open(sd->d_name, O_RDONLY);
				if (in < 0) 
				{
					write(1, "Error open in\n", 14);
					exit(14);
				}
				l1 = ss.st_size;
				while (l1 > BS) 
				{
					nread = read(in, buff, BS);
					if (nread == -1) 
					{
						write(1,
						"Error reading1\n", 15);
						exit(14);
					}
					l1 = l1-BS;
					if (write(f, buff, BS) == -1) 
					{
						write(1,"Error writing\n", 14);
						exit(14);
					}
				}
				if (read(in, buff, l1) == -1) 
				{
					write(1,
					"Error reading\n", 14);
					exit(14);
				}
				if (write(f, buff, l1) == -1) 
				{
					write(1,
					"Error writing\n", 14);
					exit(14);
				}
			}
	}
	write(1, "Complete\n", 9);
	if (closedir(d) == 0)
		exit(15);
	if (close(f) == 0)
		exit(15);
}


void main(void)
{
	char buf[BS3];
	char i, j;
	int n;

	write(1, "Press the key:\n 1 - Packing\n", 28);
	write(1, " 2 - Unpacking\n Any key - Exit\n", 31);
	if (read(0, &i, 1) < 1) {
		write(1, "Error reading key\n", 18);
		exit(1);
	}
	if (read(0, &j, 1) < 1) {
		write(1, "Error reading\n", 14);
		exit(1);
	}
	switch (i) {
	case '1':
	{
		write(1, "Write dirent\n", 13);
		n = read(0, buf, sizeof(buf));
		if (n < 2) {
			write(1, "Error reading dirent\n", 21);
			exit(1);
		}
		buf[n-1] = '\0';
		Pack(buf);
		clearbuf(buf, n);
		break;
	}
	case '2':
	{
		write(1, "Write dirent\n", 13);
		n = read(0, &buf, sizeof(buf));
		if (n < 2) {
			write(1, "Error reading dirent\n", 21);
			exit(1);
		}
		buf[n-1] = '\0';
		Unpack(buf);
		clearbuf(buf, n);
		break;
	}
	default:
	{
		i = '0';
		break;
	}
	}
}
