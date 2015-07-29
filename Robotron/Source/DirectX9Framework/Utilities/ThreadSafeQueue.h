// 
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2015 Media Design School
//
// File Name	: TreadSafeQueue.h
// Desrciption	: CThreadSafeQueue implementation file.
// Author		: Hayden Asplet.
// Mail			: hayden.asplet@mediadesignschool.com
//

#pragma once

#ifndef __THREADSAFEQUEUE_H__
#define __THREADSAFEQUEUE_H__

// Library Includes
#include <queue>
#include <memory>
#include <cassert>
#include <mutex>
#include <condition_variable>

// Local Includes

template<typename T>
class CThreadSafeQueue {
	// Member Functions
public:
	CThreadSafeQueue();
	CThreadSafeQueue(const CThreadSafeQueue& _krOther);
	CThreadSafeQueue& operator=(CThreadSafeQueue& _krOther);

	~CThreadSafeQueue();

	// Pushes the new value onto the back of the queue, will notify a thread waiting to pop and retrieve a value.
	void Push(T _newValue);

	bool TryToPop(T& _frontValue);
	bool WaitToPop(T& _frontValue);

	// Clears the entire queue. 
	void Clear();

	int Size() const;
	bool Empty() const;
protected:
private:
	void Copy(const CThreadSafeQueue& _krOther);

	// Member Variables
public:
protected:
private:
	std::queue<T> m_queue;	// The queue itself, this class is just a thread safe wrapper for std::queue's.
	mutable std::mutex m_mutex;				// The mutex to lock to ensure thread safety, must be mutable so that 
											// it can be changed inside const member functions.
	std::condition_variable m_condition;
	bool m_bIsKillingWaiters;	// True if threads that are waiting should stop waiting.
};

template<typename T>
CThreadSafeQueue<T>::CThreadSafeQueue()
	:m_bIsKillingWaiters(false)
{

}

template<typename T>
CThreadSafeQueue<T>::~CThreadSafeQueue()
{

}

template<typename T>
CThreadSafeQueue<T>::CThreadSafeQueue(const CThreadSafeQueue& _krOther)
{
	Copy(_krOther);
}

template<typename T>
CThreadSafeQueue& CThreadSafeQueue<T>::operator=(CThreadSafeQueue& _krOther)
{
	Copy(_krOther);
	return *this;
}

template<typename T>
void CThreadSafeQueue<T>::Push(T _newValue)
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	m_queue.push(_newValue);

	// Notify a thread waiting to pop.
	m_condition.notify_one();
}

template<typename T>
void CThreadSafeQueue<T>::KillWaiting()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	m_bIsKillingWaiters = true;

	m_condition.notify_all();
}

template<typename T>
bool CThreadSafeQueue<T>::TryToPop(T& _frontValue)
{
	std::unique_lock<std::mutex> unqlockGuard(m_mutex);

	if (m_queue.empty()) {
		return false;
	}

	_frontValue = m_queue.front();
	m_queue.pop();

	return true;
}

template<typename T>
bool CThreadSafeQueue<T>::WaitToPop(T& _frontValue)
{
	std::unique_lock<std::mutex> unqlockGuard(m_mutex);

	// Wait for the queue to not be empty.
	m_condition.wait(unqlockGuard, [this]() { return !m_queue.empty() || m_bIsKillingWaiters; });

	if (m_bIsKillingWaiters) {
		return false;
	}

	assert(!m_queue.empty());

	_frontValue = m_queue.front();
	m_queue.pop();

	return true;
}

template<typename T>
void CThreadSafeQueue<T>::Clear()
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	while (!m_queue.empty()) {
		m_queue.pop();
	}
}

template<typename T>
int CThreadSafeQueue<T>::Size() const
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_queue.size();
}

template<typename T>
bool CThreadSafeQueue<T>::Empty() const
{
	std::lock_guard<std::mutex> lockGuard(m_mutex);
	return m_queue.empty();
}

template<typename T>
void CThreadSafeQueue<T>::Copy(const CThreadSafeQueue& _krOther)
{
	std::lock_guard<std::mutex> lockGuard(_krOther.m_mutex);
	m_queue = _krOther.m_queue;
}

#endif	// __THREADSAFEQUEUE_H__
