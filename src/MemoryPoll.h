#pragma once

#include <stack>

template<typename Type>
class MemoryPoll
{
public:

	MemoryPoll() {}

	// delete copy constructor
	MemoryPoll(const MemoryPoll&) = delete;

	// move constructor
	MemoryPoll(MemoryPoll&& other)
	{
		this->free(std::move(other.free));
	}

	// destructor
	~MemoryPoll()
	{
		clear();
	}

	void clear()
	{
		while (!m_free.empty())
		{
			Type* obj = static_cast<Type*>(m_free.top());
			::operator delete(obj);
			m_free.pop();
		}
	}

    // allocate a chunk of memory
	void* allocate()
	{
		void* place;

		if (!m_free.empty())
		{
			place = static_cast<void*>(m_free.top());
			m_free.pop();
		}
		else
		{
			std::cout << "New Object" << std::endl;
			place = operator new(sizeof(Type));
		}

		return place;
	}

	// mark object memory as available
	void deallocate(void* o)
	{
		Type* obj = static_cast<Type*>(o);
		m_free.push(obj);
	}

private:

	// pointers to free chunks
	std::stack<Type*> m_free;
};

template<typename Child, template<typename = Child> class PoolClass = MemoryPoll >
class InheritablePool
{
public:

	// provide access from the inheriting object to clear the pool
	static void clearPool()
	{
		pool.clear();
	}

	static void* operator new(size_t)
	{
		return pool.allocate();
	}

	static void operator delete(void* in)
	{
		pool.deallocate(in);
	}

private:

	// protected ctors so only Child can make instances
	InheritablePool() {}
	InheritablePool(const InheritablePool&) {}

	// our object pool
	static PoolClass<Child> pool;

	// Child class can make instances
	friend Child;
};

//define the static pool variable
template<typename Child, template<typename> class PoolClass>
PoolClass<Child> InheritablePool<Child, PoolClass>::pool;

//make inheriting a little less wordy using a define
#define UseObjectPool( x ) public virtual InheritablePool<x>