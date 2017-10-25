#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

/*
INSERT BLOCK COMMENT DESCRIBING PROGRAM
*/


int main(int argc, char **argv)
{
	char start_directory[500];
	char file_name[256];
	char write_buf[256];
	DIR *dir;
	struct dirent *dir_entry;
	struct stat stat_buf_send;
	struct stat stat_buf_receive;
	int fd_dir_stats;
	int regular_file_count = 0;
	int directory_count = 0;
	int total_filesize = 0;
	int block_count = 0;
	int member_count = 0;
	int the_pipe[2];
	int status;

	if (argc == 1)
	{
		fprintf(stderr, "Error: Invalid arguments, no directory input\n");
		exit(-1);
	}

	if ((getcwd(start_directory, 500)) == NULL) //store the starting directory to return to it at program close
	{
		perror("getcwd");
		exit(-1);
	}
	if ((chdir(argv[1])) == -1) // change directories to target directory
	{
		perror("chdir");
		exit(-1);
	}
	if ((dir = opendir(argv[1])) == NULL) //open target directory
	{
		perror("opendir");
		exit(-1);
	}

	readdir(dir); // Skip current directory
	readdir(dir); // Skip parent directory

	if ((pipe(the_pipe)) == -1)
	{
		perror("pipe");
		exit(-1);
	}

	if((fd_dir_stats = open("directory_stats", O_RDWR | O_CREAT | O_TRUNC)) == -1)
	{
		perror("open");
		exit(-1);
	}

	switch (fork())
	{
		case 0:
			close(the_pipe[1]);
			stat_buf_receive.st_ino = -1;
			while(stat_buf_receive.st_ino != 0)
			{
				if ((read(the_pipe[0], &stat_buf_receive, sizeof(stat_buf_receive))) == -1)
			{
				perror("write");
				exit(-1);
			}
			if (S_ISREG(stat_buf_receive.st_mode))
			{
				regular_file_count += 1;
			}
			else if (S_ISDIR(stat_buf_receive.st_mode))
			{
				directory_count += 1;
			}

			total_filesize += (int)stat_buf_receive.st_size;

			block_count += (int)stat_buf_receive.st_blocks;
			}

			sprintf(write_buf, "DIRECTORY STATS\n");
			write(fd_dir_stats, write_buf, strlen(write_buf));
			sprintf(write_buf, "------------------------------\n");
			write(fd_dir_stats, write_buf, strlen(write_buf));
			sprintf(write_buf, "Regular files:%40d\n", regular_file_count);
			write(fd_dir_stats, write_buf, strlen(write_buf));
			sprintf(write_buf, "Directories:%42d\n", directory_count);
			write(fd_dir_stats, write_buf, strlen(write_buf));
			sprintf(write_buf, "Total filesize (bytes):%31d\n", total_filesize);
			write(fd_dir_stats, write_buf, strlen(write_buf));
			sprintf(write_buf, "Total storage allocation (blocks):%22d\n", block_count);
			write(fd_dir_stats, write_buf, strlen(write_buf));

			exit(0);
				
		default:
			close(the_pipe[0]);
			while ((dir_entry = readdir(dir)) != NULL)
			{
				member_count+=1;
				lstat(dir_entry->d_name, &stat_buf_send);
				if ((write(the_pipe[1], &stat_buf_send, sizeof(stat_buf_send))) == -1)
				{
					perror("write");
					exit(-1);
				}
			}

			stat_buf_send.st_ino = 0;
			if ((write(the_pipe[1], &stat_buf_send, sizeof(stat_buf_send))) == -1)
			{
				perror("write");
				exit(-1);
			}

			wait(&status);

			if ((chdir(start_directory)) == -1)
			{
				perror("chdir");
				exit(-1);
			}
			close(fd_dir_stats);
			exit(0);
		
	}
}	
