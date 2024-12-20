#pragma once

namespace Reflection::Generation
{
	template<typename T>
	inline constexpr size_t SizeOf = sizeof(T);

	template<>
	inline constexpr size_t SizeOf<void> = 0;

	template<typename... Args>
	struct FullNameLength;

	template<typename T>
	struct NameLengthOf
	{
		constexpr static uint32_t value = IsReflectable<T> ? TypeOf<T>::NameLength : 1;
	};

	template<>
	struct FullNameLength<>
	{
		constexpr static uint32_t value = 0;
	};

	template<typename First, typename... Rest>
	struct FullNameLength<First, Rest...>
	{
		constexpr static uint32_t value = NameLengthOf<First>::value + FullNameLength<Rest...>::value;
	};

	template<size_t Size, typename ...Args>
	struct TemplateNameGenerator
	{
	private:

		char m_Name[Size + 1];

	public:

		constexpr TemplateNameGenerator(const char* templateName, int size) // TODO: maybe remove size
		{
			memcpy(m_Name, templateName, size);
			m_Name[size] = '<';

			Generate<0, Args...>(m_Name + size + 1);

			m_Name[Size - 1] = '>';
			m_Name[Size] = 0;
		}

		template<int CopySize>
		constexpr void Copy(char* dest, const char* src)
		{
			for (size_t i = 0; i < CopySize; ++i)
				dest[i] = src[i];
		}

		template<size_t I, typename First, typename... Rest>
		constexpr void Generate(char* dest)
		{
			constexpr size_t length = TypeOf<First>::NameLength;
			if constexpr (length > 0)
			{
				const char* type_name = TypeOf<First>::GetName();
				Copy<length>(dest, type_name);
			}
			else
			{
				Copy<1>(dest, "?");
			}

			if constexpr (sizeof...(Rest) > 0)
			{
				dest += (length > 0 ? length : 1);
				Copy<2>(dest, ", ");
				Generate<I + 1, Rest...>(dest + 2);
			}
		}

		template<size_t I>
		constexpr void Generate(char* dest)
		{
		}

		constexpr operator const char* () const
		{
			return m_Name;
		}
	};

	template <typename T, typename U>
	bool IsStaticField(U T::*)
	{
		return false;
	}

	template <typename U>
	bool IsStaticField(U* field)
	{
		return true;
	}

	template <typename T, typename U>
	size_t GetOffsetOfField(U T::* field)
	{
		return reinterpret_cast<size_t>(&(((T*)0)->*field));
	}

	template <typename U>
	size_t GetOffsetOfField(U* field)
	{
		return reinterpret_cast<size_t>(field);
	}

	template<typename T>
	const Type* GetActualType(void* object)
	{
		return ((T*)object)->GetType();
	}
}
