#ifndef QUEUE_H__
#define QUEUE_H__

template <class T>
class Queue
{
public:
	Queue(T data);
	T Data;
	Queue<T>* Next = NULL;
};

template <class T>
Queue<T>::Queue(T data) : Data(data) {}

#endif