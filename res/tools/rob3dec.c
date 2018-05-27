#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUT_BUF_SIZE 2048

int decode(char *in_buf, char *out_buf, int buf_size)
{
	unsigned char in_char, counter;
	int out_buf_size = 0;
	in_char = in_buf[0];
	while (in_char != 0x0FF)
	{
		in_buf++;
		if (in_char & 0x80)
		{
			for (counter = (in_char & 0x7F); counter > 0; counter--)
			{
				out_buf[out_buf_size] = in_buf[0];
				out_buf_size++;
				in_buf++;
			}
		}
		else
		{
			for (counter = in_char; counter > 0; counter--)
			{
				out_buf[out_buf_size] = in_buf[0];
				out_buf_size++;
			}
			in_buf++;
		}
		in_char = in_buf[0];
	}
	return out_buf_size;
}

int fsize(FILE *f)
{
	int temp;
	fseek(f, 0, SEEK_END);
	temp = ftell(f);
	fseek(f, 0, SEEK_SET);
	return temp;
}

int main(int argc, const char *argv[])
{
	FILE *i_file, *o_file;
	int i_filesize, o_filesize;
	void *inptr;
	void *outptr;

	if (argc == 3)
	{
		if (!(i_file = fopen(argv[1], "rb")))
		{
			printf(" Error loading file: %s\n", argv[1]);
			return 1;
		}
		i_filesize = fsize(i_file);
		inptr = malloc(i_filesize);
		fread(inptr, 1, i_filesize, i_file);

		outptr = malloc(OUT_BUF_SIZE);

		o_filesize = decode(inptr, outptr, OUT_BUF_SIZE);
		o_file = fopen(argv[2], "wb");
		fwrite(outptr, 1, o_filesize, o_file);

		fclose(o_file);
		free(outptr);

		fclose(i_file);
		free(inptr);

	}
	else printf("Parameter lost!");
	return 0;
}
