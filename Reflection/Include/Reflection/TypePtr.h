#pragma once

#include "CppReflectionCore.h"

namespace Reflection
{
	class Type;

	class REFLECTION_API TypePtr
	{
		friend class Assembly;

	private:

		const Type* m_Type;

	public:

		TypePtr(const TypePtr&) noexcept = default;
		TypePtr(TypePtr&&) noexcept = default;

		TypePtr& operator=(const TypePtr&) noexcept = default;
		TypePtr& operator=(TypePtr&&) noexcept = default;

		TypePtr(const Type* type = nullptr) noexcept :
			m_Type(type)
		{
		}

		const Type* operator->() const noexcept
		{
			return m_Type;
		}

		operator bool() const noexcept
		{
			return m_Type != nullptr;
		}

		operator const Type*() const noexcept
		{
			return m_Type;
		}

		bool operator==(const TypePtr& other) const noexcept;

		bool operator!=(const TypePtr& other) const noexcept
		{
			return operator==(other) == false;
		}

		bool operator==(std::nullptr_t) const noexcept
		{
			return m_Type == nullptr;
		}

		bool operator!=(std::nullptr_t) const noexcept
		{
			return m_Type != nullptr;
		}
	};
}


