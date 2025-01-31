#pragma once

#include "ExampleLib/Common.h"

namespace ExampleDll
{
	REFLECTABLE()
	template<class T>
	class MyPtr
	{
		#include "Generation/ExampleDll.MyPtr.meta.inl"

	private:

		FIELD()
		T* m_Ptr;

	public:

		MyPtr(T* ptr = nullptr) :
			m_Ptr(ptr)
		{
		}

		METHOD()
		T* GetPtr() const
		{
			return m_Ptr;
		}
	};

	REFLECTABLE()
	template<typename T1, typename T2>
	class MyPair
	{
		#include "Generation/ExampleDll.MyPair.meta.inl"

	private:

		FIELD()
		T1 m_First;

		FIELD()
		T2 m_Second;

	public:

		CONSTRUCTOR()
		MyPair() = default;

		CONSTRUCTOR()
		MyPair(T1 first, T2 second) :
			m_First(first),
			m_Second(second)
		{
		}

		METHOD()
		T1 GetFirst() const { return m_First; }

		METHOD()
		T2 GetSecond() const { return m_Second; }
	};

	REFLECTABLE()
	template<typename... TArgs>
	class MyEclipse
	{
		#include "Generation/ExampleDll.MyEclipse.meta.inl"

	public:

		virtual ~MyEclipse()
		{
			printf(__FUNCTION__ "\n");
		}
	};
}
