
#include "Bmp.hpp"

Bmp::Bmp(void)
{
	return ;
}

Bmp::~Bmp(void)
{
	delete [] this->data;
	return ;
}

int
Bmp::error(char const *s)
{
	std::cerr << s << std::endl;
	return (0);
}

int
Bmp::getBmpInfo(void)
{
	int					unused_header_size;
	unsigned char		*unused_header;

	if (*(uint16_t *)&bmp_header[0] != 0x4D42)
		return (error(HEADER_E2));
	bmp_size = *(uint32_t *)&bmp_header[2];
	data_offset = *(uint32_t *)&bmp_header[10];
	width = *(uint32_t *)&dib_header[4];
	height = *(uint32_t *)&dib_header[8];
	if ((bpp = *(uint16_t *)&dib_header[14]) != 24)
		return (error(BPP_E1));
	if ((compression = *(uint32_t *)&dib_header[16]) != BI_RGB)
		return (error(COMPRESSION_E1));
	raw_bmp_size = *(uint32_t *)&dib_header[20];
	unused_header_size = data_offset - (BMP_HSIZE + DIB_HSIZE);
	if (!raw_bmp_size)
		raw_bmp_size = bmp_size - (BMP_HSIZE + DIB_HSIZE);
	if (unused_header_size > 0)
	{
		unused_header = (unsigned char *)malloc(unused_header_size);
		read(fd, unused_header, unused_header_size);
		free(unused_header);
	}
	return (1);
}

int
Bmp::writeData(void)
{
	unsigned char		buf[3];
	uint32_t			i;
	uint32_t			j;
	uint32_t			k;

	k = 0;
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (read(fd, buf, 3) == -1)
				return (0);
			data[k++] = buf[2];
			data[k++] = buf[1];
			data[k++] = buf[0];
			j++;
		}
		if (j % 4)
			if (read(fd, buf, 1) == -1)
				return (0);
		i++;
	}
	return (1);
}

int
Bmp::load(char const *filename)
{
	if ((fd = open(filename, O_RDONLY)) == -1)
		return (error(FILE_E1));
	if (read(fd, bmp_header, BMP_HSIZE) == -1)
		return (error(HEADER_E1));
	if (read(fd, dib_header, DIB_HSIZE) == -1)
		return (error(HEADER_E3));
	if (!getBmpInfo())
		return (0);
	data = new unsigned char[raw_bmp_size];
	if (!writeData())
		return (0);
	close(fd);
	return (1);
}

Bmp &
Bmp::operator=(Bmp const &rhs)
{
	if (this != &rhs)
	{
		// copy members here
	}
	return (*this);
}

std::ostream &
operator<<(std::ostream &o, Bmp const &i)
{
	o	<< "Bmp: " << &i;
	return (o);
}
