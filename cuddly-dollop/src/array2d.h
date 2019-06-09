#ifndef ARRAY2D_H__
#define ARRAY2D_H__

template <class T>
class Array2d
{
    public:
        Array2d(int x, int y);
        ~Array2d();
        Array2d(const Array2d& array);

		void Copy(const Array2d& a);
        void Set(int x, int y, T valeur);
		void Add(int x, int y, T valeur);
        T Get(int x, int y) const;
		T Get(int v) const;

		int Count();
		int CountX();
		int CountY();

        void Reset(T valeur);

    private:
        int To2dIndex(int x, int y) const;

    private:
        int m_x, m_y;
        T* m_values;
};

template <class T>
Array2d<T>::Array2d(int x, int y) : m_x(x), m_y(y)
{
	m_values = new T[m_x * m_y];
}

template <class T>
Array2d<T>::~Array2d()
{
	delete[] m_values;
}

template <class T>
Array2d<T>::Array2d(const Array2d& array) : m_x(array.m_x), m_y(array.m_y)
{
	m_values = new T[m_x * m_y];
	for (int i = 0; i < m_x * m_y; ++i)
	{
		T x = array.Get(i, 0);
		Set(i, 0, x);
	}
}

template <class T>
void Array2d<T>::Copy(const Array2d& a)
{
	delete[] m_values;
	m_x = a.m_x;
	m_y = a.m_y;
	m_values = new T[m_x * m_y];
	for (int i = 0; i < m_x * m_y; ++i)
	{
		float x = a.Get(i, 0);
		Set(i, 0, x);
	}
}

template <class T>
void Array2d<T>::Set(int x, int y, T value)
{
	m_values[To2dIndex(x, y)] = value;
}

template <class T>
//The designated slot becomes the sum of the old value + the new value
void Array2d<T>::Add(int x, int y, T value)
{
	//m_values[To2dIndex(x, y)] += value;
	float f = Get(x, y);
	f += value;
	Set(x, y, f);
}

template <class T>
T Array2d<T>::Get(int x, int y) const
{
	return m_values[To2dIndex(x, y)];
}

template <class T>
T Array2d<T>::Get(int v) const
{
	return m_values[v];
}

template <class T>
void Array2d<T>::Reset(T valeur)
{
	for (int i = 0; i < m_x * m_y; ++i)
		m_values[i] = valeur;
}

template <class T>
int Array2d<T>::Count()
{
	return m_x * m_y;
}

template <class T>
int Array2d<T>::CountX()
{
	return m_x;
}

template <class T>
int Array2d<T>::CountY()
{
	return m_y;
}

template <class T>
int Array2d<T>::To2dIndex(int x, int y) const
{
	return x + (y * m_x);
}

#endif // ARRAY2D_H__
