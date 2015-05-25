#ifndef MATSTACK_HPP
# define MATSTACK_HPP

# include <stack>
# include "Mat4.hpp"

template<typename T>
class MatStack
{
public:
	std::stack<Mat4<T>>			stack;

	MatStack(void)
	{
		Mat4<T>		mat;

		mat.setIdentity();
		stack.push(mat);
	}

	~MatStack(void)
	{
	}

	void			push(void)
	{
		Mat4<T>		mat;

		mat = stack.top();
		stack.push(mat);
	}

	void			pop(void)
	{
		stack.pop();
	}

	void			rotate(float const &angle, float const &x,
							float const &y, float const &z)
	{
		stack.top().rotate(angle, x, y, z);
	}

	void			translate(void)
	{
		stack.top().translate();
	}

	void			scale(void)
	{

	}

	MatStack<

private:
	MatStack(MatStack const &src);
};

#endif

