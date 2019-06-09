#ifndef ARRAY3D_H__
#define ARRAY3D_H__

template <class T>
class Array3d
{
    public:
        Array3d(int x, int y, int z);
        ~Array3d();
        Array3d(const Array3d& array);

        void Set(int x, int y, int z, T valeur);
        T Get(int x, int y, int z) const;
		T Get(int x) const;
		int GetXCount();
		int GetYCount();
		int GetZCount();

		int Count();
		int Count(int x);

        void Reset(T valeur);

    private:
        int To2dIndex(int x, int y, int z) const;

    private:
        int m_x, m_y, m_z;
        T* m_values;
};

template <class T>
Array3d<T>::Array3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z)
{
	m_values = new T[m_x * m_y * m_z];
}

template <class T>
Array3d<T>::~Array3d()
{
	delete[] m_values;
}

template <class T>
int Array3d<T>::Count()
{
	return m_x * m_y * m_z;
}

template <class T>
int Array3d<T>::Count(int x)
{
	if (x == 0)
		return m_x;
	else if (x == 1)
		return m_y;
	else
		return m_z;
}

template <class T>
Array3d<T>::Array3d(const Array3d& array) : m_x(array.m_x), m_y(array.m_y), m_z(array.m_z)
{
	m_values = new T[m_x * m_y * m_z];
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_values[i] = array.m_values[i];
}

template <class T>
void Array3d<T>::Set(int x, int y, int z, T value)
{
	if (To2dIndex(x, y, z) >= Count())
		return;
	m_values[To2dIndex(x, y, z)] = value;
}

template <class T>
T Array3d<T>::Get(int x, int y, int z) const
{
	return m_values[To2dIndex(x, y, z)];
}

template <class T>
T Array3d<T>::Get(int x) const
{
	return m_values[x];
}

template <class T>
int Array3d<T>::GetXCount()
{
	return m_x;
}

template <class T>
int Array3d<T>::GetYCount()
{
	return m_y;
}

template <class T>
int Array3d<T>::GetZCount()
{
	return m_z;
}

template <class T>
void Array3d<T>::Reset(T valeur)
{
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_values[i] = valeur;
}

template <class T>
int Array3d<T>::To2dIndex(int x, int y, int z) const
{
	return x + (z * m_x) + (y * m_z * m_x);
}

#endif // ARRAY3D_H__

