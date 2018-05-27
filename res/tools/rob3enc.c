#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUT_BUF_SIZE 2048

int encode(char *in_buf, char *out_buf, int buf_size)
{
	int out_buf_size = 0, in_buf_size = 0, _count = 1;
	unsigned char in_char, _pos = 0;

	while (in_buf_size<buf_size)
	{
		in_char = (unsigned char)in_buf[0];
		while ((in_char != (unsigned char)in_buf[_count]) && ((in_buf_size + _count) <= buf_size) && (_count<120))
		{
			in_char = (unsigned char)in_buf[_count];
			_count++;
		}
		if (_count>1)
		{
			_count--;
			_count &= 0x7F;
			(unsigned char)out_buf[out_buf_size] = _count | 0x80;
			out_buf_size++;
			for (_pos = 0; _pos<_count; _pos++)
			{
				(unsigned char)out_buf[out_buf_size] = (unsigned char)in_buf[_pos];
				out_buf_size++;
			}
		}
		else
		{
			while ((in_char == (unsigned char)in_buf[_count]) && ((in_buf_size + _count) <= buf_size) && (_count<0x7F))
			{
				in_char = (unsigned char)in_buf[_count];
				_count++;
			}
			(unsigned char)out_buf[out_buf_size] = _count;
			out_buf_size++;
			(unsigned char)out_buf[out_buf_size] = in_char;
			out_buf_size++;
		}
		in_buf += _count;
		in_buf_size += _count;
		_count = 1;
	}
	(unsigned char)out_buf[out_buf_size] = 0xFF;
	out_buf_size++;
	(unsigned char)out_buf[out_buf_size] = 0xFF;
	out_buf_size++;
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

		o_filesize = encode(inptr, outptr, i_filesize);
		o_file = fopen(argv[2], "wb");
		fwrite(outptr, 1, o_filesize, o_file);

		fclose(o_file);
		free(outptr);

		fclose(i_file);
		free(inptr);

	}
	else printf("USAGE: <file.exe> <input.file> <output.file>");
	return 0;
}
