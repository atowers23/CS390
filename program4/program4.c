#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/*
Andrew Towers
This program will generate a directory statistics file for a directory supplied as the first argument.
The number of regular files, number of directories, total filesize in bytes, and total file storage
allocation in blocks will be calculated for the supplied directory and printed on the screen.  The output
will also be stored in a file called "directory_stats" in the same directory as this program is run from.
*/


int main(int argc, char **argv)
{
	char start_directory[500];
	char write_buf[256];
	char read_buf[1024];
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
	int num_bytes_read;

	if (argc == 1) // Check if no directory was specified
	{
		fprintf(stderr, "Error: Invalid arguments, no directory input\n");
		exit(-1);
	}

	if ((getcwd(start_directory, 500)) == NULL) //Store the starting directory to return to it at program close
	{
		perror("getcwd");
		exit(-1);
	}

	if ((fd_dir_stats = open("directory_stats", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU)) == -1) //Open directory statistics file for writing
	{
		perror("open");
		exit(-1);
	}

	if ((chdir(argv[1])) == -1) // Change directories to target directory
	{
		perror("chdir");
		exit(-1);
	}
	if ((dir = opendir(".")) == NULL) //Open target directory
	{
		perror("opendir");
		exit(-1);
	}

	readdir(dir); // Skip current directory
	readdir(dir); // Skip parent directory

	if ((pipe(the_pipe)) == -1) //Open pipe for IPC
	{
		perror("pipe");
		exit(-1);
	}

	switch (fork()) // Fork process
	{
	case 0: // Child process
		close(the_pipe[1]); // Close write end of pipe
		if ((read(the_pipe[0], &stat_buf_receive, sizeof(stat_buf_receive))) == -1) //Initial read of status record
		{
			perror("write");
			exit(-1);
		}
		while (stat_buf_receive.st_ino != 0) // Loop until status record with 0 inode encountered
		{

			if (S_ISREG(stat_buf_receive.st_mode)) // If file is regular file
			{
				regular_file_count += 1; // Increment regular file count
			}
			else if (S_ISDIR(stat_buf_receive.st_mode)) // If file is directory
			{
				directory_count += 1; // Increment directory count
			}

			total_filesize += (int)stat_buf_receive.st_size; // Add this file's bytes to total bytes

			block_count += (int)stat_buf_receive.st_blocks; // Add this file's blocks to total blocks

			if ((read(the_pipe[0], &stat_buf_receive, sizeof(stat_buf_receive))) == -1) // Read next status record
			{
				perror("write");
				exit(-1);
			}
		}

		sprintf(write_buf, "DIRECTORY STATS\n"); // Fill write buffer with line to be printed
		write(fd_dir_stats, write_buf, strlen(write_buf)); // Write contents of line buffer to directory statistics file

		sprintf(write_buf, "------------------------------------------------------\n"); // Update write buffer with line to be printed
		write(fd_dir_stats, write_buf, strlen(write_buf));

		sprintf(write_buf, "Regular files:%40d\n", regular_file_count);
		write(fd_dir_stats, write_buf, strlen(write_buf));

		sprintf(write_buf, "Directories:%42d\n", directory_count);
		write(fd_dir_stats, write_buf, strlen(write_buf));

		sprintf(write_buf, "Total filesize (bytes):%31d\n", total_filesize);
		write(fd_dir_stats, write_buf, strlen(write_buf));

		sprintf(write_buf, "Total storage allocation (blocks):%20d\n", block_count);
		write(fd_dir_stats, write_buf, strlen(write_buf));

		close(fd_dir_stats); // Close directory statistics file in child
		exit(0); // Exit process successfully

	default:
		close(the_pipe[0]); // Close read end of pipe
		while ((dir_entry = readdir(dir)) != NULL) // Read every directory entry
		{
			lstat(dir_entry->d_name, &stat_buf_send); // Store status record of directory entry in status_buf_send
			if ((write(the_pipe[1], &stat_buf_send, sizeof(stat_buf_send))) == -1) // Send status record over pipe
			{
				perror("write");
				exit(-1);
			}
		}

		stat_buf_send.st_ino = 0; // Set inode to finished flag value
		if ((write(the_pipe[1], &stat_buf_send, sizeof(stat_buf_send))) == -1) // Send status record over pipe
		{
			perror("write");
			exit(-1);
		}

		if ((chdir(start_directory)) == -1) // Change directory back to starting directory
		{
			perror("chdir");
			exit(-1);
		}

		wait(&status); // Wait for child to die

		lseek(fd_dir_stats, 0, SEEK_SET); // Set file offset pointer to beginning of file

		while ((num_bytes_read = read(fd_dir_stats, read_buf, 1024)) > 0) // Read into read_buf until end of file
		{
			if (num_bytes_read != write(STDOUT_FILENO, read_buf, num_bytes_read)) // Print read_buf to screen
			{
				perror("write");
				exit(-1);
			}
		}
		close(fd_dir_stats); // Close directory statistics file in parent
		exit(0); // Exit successfully

	}
}