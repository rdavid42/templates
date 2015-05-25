#ifndef MAT4_HPP
# define MAT4_HPP

#include "Vec3.hpp"

template<typename TYPE>
class Mat4
{
public:
	TYPE		val[16];

	Mat4()
	{
		int		i;

		i = 0;
		while (i < 16)
		{
			this->val[i] = 0;
			i++;
		}
		return ;
	}

	~Mat4(void)
	{
		return ;
	}

	void
	set(TYPE const *val)
	{
		int		i;
		
		i = 0;
		while (i < 16)
		{
			this->val[i] = val[i];
			i++;
		}
		return ;
	}

	void
	reset(void)
	{
		int		i;

		i = 0;
		while (i < 16)
		{
			this->val[i] = 0;
			++i;
		}
		return ;
	}

	void
	multiplyMatrix(Mat4<TYPE> mat)
	{
		Mat4<TYPE>	res;
		int			i;
		int			j;
		int			k;


		i = 0;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				res.val[j * 4 + i] = 0.0f;
				k = 0;
				while (k < 4)
				{
					res.val[j * 4 + i] += this->val[k * 4 + i] * mat.val[j * 4 + k];
					k++;
				}
				j++;
			}
			i++;
		}
		*this = res;
	}

	void
	setIdentityMatrix()
	{
		int			i;

		i = -1;
		while (i++, i < 16)
			this->val[i] = 0.0f;
		i = -1;
		while (i++, i < 4)
			this->val[i + i * 4] = 1.0f;
	}

	void
	setTranslationMatrix(Vec3<TYPE> val)
	{
		/*
		1		0		0		0
		0		1		0		0
		0		0		1		0
		x		y		z		1
		*/

		this->setIdentityMatrix();
		this->val[12] = val.x;
		this->val[13] = val.y;
		this->val[14] = val.z;
	}

	void
	translateMatrix(Vec3<TYPE> val)
	{
		Mat4<TYPE>		translate;

		translate.setTranslationMatrix(val);
		*this = *this * translate;
		return ;
	}

	void
	translateMatrix(Mat4<TYPE> translate)
	{
		*this = *this * translate;
		return ;
	}

	void
	setRotationMatrix(Vec3<TYPE> &axis, float const &angle)
	{
		TYPE		s = sin(angle);
		TYPE		c = cos(angle);
		TYPE		oc = 1.0 - c;
		
		axis.normalize();

		this->val[0] = oc * axis.x * axis.x + c;
		this->val[1] = oc * axis.x * axis.y - axis.z * s;
		this->val[2] = oc * axis.z * axis.x + axis.y * s;
		this->val[3] = 0.0;
		this->val[4] = oc * axis.x * axis.y + axis.z * s;
		this->val[5] = oc * axis.y * axis.y + c;
		this->val[6] = oc * axis.y * axis.z - axis.x * s;
		this->val[7] = 0.0;
		this->val[8] = oc * axis.z * axis.x - axis.y * s;
		this->val[9] = oc * axis.y * axis.z + axis.x * s;
		this->val[10] = oc * axis.z * axis.z + c;
		this->val[11] = 0.0;
		this->val[12] = 0.0;
		this->val[13] = 0.0;
		this->val[14] = 0.0;
		this->val[15] = 1.0;
		return ;
	}

	void
	rotateMatrix(Vec3<TYPE> const &axis, float const &angle)
	{
		Mat4<TYPE>		rotation;

		rotation.setRotationMatrix(axis, angle);

		*this = *this * rotation;
		return ;
	}

	void
	rotateMatrix(Mat4<TYPE> rotation)
	{
		*this = *this * rotation;
		return ;
	}

	void
	setScaleMatrix(Vec3<TYPE> scale)
	{
		this->reset();

		this->val[0] = scale.x;
		this->val[5] = scale.y;
		this->val[10] = scale.z;
		this->val[15] = 1;
		return ;
	}

	void
	scaleMatrix(Vec3<TYPE> val)
	{
		Mat4<TYPE>	scale;

		scale.setScaleMatrix(val);
		*this = *this * scale;
		return ;
	}

	void
	scaleMatrix(Mat4<TYPE> scale)
	{
		*this = *this * scale;
		return ;
	}

	Mat4<TYPE> &
	operator=(Mat4<TYPE> const &rhs)
	{
		int		i;

		i = 0;
		if (this != & rhs)
		{
			while (i < 16)
			{
				this->val[i] = rhs.val[i];
				++i;
			}
		}
		return (*this);
	}

	Mat4<TYPE>
	operator*(Mat4<TYPE> const & rhs) const
	{
		Mat4<TYPE>	res;
		int			i;
		int			j;
		int			k;


		i = 0;
		while (i < 4)
		{
			j = 0;
			while (j < 4)
			{
				res.val[j * 4 + i] = 0.0f;
				k = 0;
				while (k < 4)
				{
					res.val[j * 4 + i] += this->val[k * 4 + i] * rhs.val[j * 4 + k];
					k++;
				}
				j++;
			}
			i++;
		}
		return (res);
	}

	Mat4<TYPE>
	operator-(Mat4<TYPE> const &rhs) const
	{
		Mat4<TYPE> res;

		int		i;

		i = 0;
		while (i < 15)
		{
			res[i] = this->val[i] - rhs[i];
			++i;
		}
		return (res);
	}

	Mat4<TYPE> &
	operator-=(Mat4<TYPE> const &rhs)
	{
		int		i;

		i = 0;
		while (i < 15)
		{
			this->val[i] = this->val[i] - rhs[i];
			++i;
		}
		return (*this);
	}

	Mat4<TYPE>
	operator+(Mat4<TYPE> const &rhs) const
	{
		Mat4<TYPE> res;

		int		i;

		i = 0;
		while (i < 15)
		{
			res[i] = this->val[i] + rhs[i];
			++i;
		}
		return (res);
	}

	Mat4<TYPE> &
	operator+=(Mat4<TYPE> const &rhs)
	{
		int		i;

		i = 0;
		while (i < 15)
		{
			this->val[i] = this->val[i] + rhs[i];
			++i;
		}
		return (*this);
	}
};

template<typename TYPE>
std::ostream &
operator<<(std::ostream &o, Mat4<TYPE> const &mat)
{
	int		i;

	i = 0;
	o << "Matrice : \n";
	o << "|";
	while (i < 16)
	{
		o << " "<< mat.val[i];
		if (i == 15)
			o << "|\n";
		else if (i % 4 == 3)
			o << "|\n|";
		++i;
	}
	o << std::endl;
	return (o);
}
// template<typename TYPE>
// std::ostream		&operator<<(std::ostream &o, Mat4<TYPE> const &mat);


#endif
