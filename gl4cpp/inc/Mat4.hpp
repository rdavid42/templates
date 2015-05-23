
#ifndef MAT4_HPP
# define MAT4_HPP

template<typename TYPE>
class Mat4
{
public:
	float		m[16];

	Mat4(void)
	{
		for (int i = 0; i < 16; i++)
			m[i] = 0.0f;
	}

	~Mat4();
private:
};

#endif
