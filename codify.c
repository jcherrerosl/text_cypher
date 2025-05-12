#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int		fd_in;
	int		fd_out;
	char	ch;
	char	*codified;
	char	*temp;
	int		bytes_read;

	char ascii[4];
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
		return (1);
	}
	fd_in = open(argv[1], O_RDONLY);
	if (fd_in == -1)
	{
		perror("Error opening input file");
		return (1);
	}
	fd_out = open("to_decyph.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		perror("Error opening output file");
		close(fd_in);
		return (1);
	}
	codified = strdup("");
	if (!codified)
	{
		perror("Error allocating memory");
		close(fd_in);
		close(fd_out);
		return (1);
	}
	while ((bytes_read = read(fd_in, &ch, 1)) > 0)
	{
		snprintf(ascii, sizeof(ascii), "%03d", (unsigned char)ch);
		temp = malloc(strlen(codified) + 4);
		if (!temp)
		{
			perror("Memory allocation failed");
			free(codified);
			close(fd_in);
			close(fd_out);
			return (1);
		}
		strcpy(temp, codified);
		strcat(temp, ascii);
		free(codified);
		codified = temp;
	}
	close(fd_in);
	if (bytes_read == -1)
	{
		perror("Error reading input file");
		free(codified);
		close(fd_out);
		return (1);
	}
	if (write(fd_out, codified, strlen(codified)) == -1)
		perror("Error writing to output file");
	close(fd_out);
	free(codified);
	return (0);
}
