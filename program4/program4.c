#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

/*
INSERT BLOCK COMMENT DESCRIBING PROGRAM
*/


int main(int argc, char **argv)
{
	char directory_path[300];
	char start_directory[500];
	char file_name[256];
	DIR *dir;
	struct dirent *dir_entry;
	struct stat stat_buf_send;
	struct stat stat_buf_receive;
	mode_t file_type;
	off_t file_size;
	blkcnt_t num_blocks;
	int regular_file_count = 0;
	int directory_count = 0;
	int total_filesize = 0;
	int block_count = 0;
	int member_count = 0;
	int the_pipe[2];

	if (argc == 1)
	{
		fprintf(stderr, "Error: Invalid arguments, no directory input\n");
		exit(-1);
	}

	strcpy(directory_path, argv[1]);
	if ((getcwd(start_directory, 500)) == -1)
	{
		perror("getcwd");
		exit(-1);
	}
	if ((chdir(directory_path)) == -1)
	{
		perror("chdir");
		exit(-1);
	}
	dir = opendir(directory_path);
	chdir(directory_path);

	readdir(dir); // Skip current directory
	readdir(dir); // Skip parent directory
	if ((pipe(the_pipe)) == -1)
	{
		perror("pipe");
		exit(-1);
	}
	while ((dir_entry = readdir(dir)) != NULL)
	{
		member_count+=1;
		lstat(dir_entry->d_name, &stat_buf_send);
		if ((write(the_pipe[1], &stat_buf_send, sizeof(stat_buf_send))) == -1)
		{
			perror("write");
			exit(-1);
		}
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
		

		strcpy(directory_path, argv[1]);
	}
	
	fprintf(stdout, "DIRECTORY STATS\n");
	fprintf(stdout, "------------------------------\n");
	fprintf(stdout, "Regular files:%40d\n", regular_file_count);
	fprintf(stdout, "Directories:%42d\n", directory_count);
	fprintf(stdout, "Total filesize (bytes):%31d\n", total_filesize);
	fprintf(stdout, "Total storage allocation (blocks):%22d\n", block_count);
	fprintf(stdout, "Members(debug):%42d\n", member_count);
	
	
	if ((chdir(start_directory)) == -1)
	{
		perror("chdir");
		exit(-1);
	}
}