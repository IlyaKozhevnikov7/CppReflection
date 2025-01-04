template<typename T, TypeFlag Flag>
struct FlagConditional;

template<typename T>
struct FlagConditional<T, TypeFlag::Primitive> : std::bool_constant<std::is_arithmetic_v<T> || std::is_void_v<T>> {};

template<typename T>
struct FlagConditional<T, TypeFlag::Abstract> : std::bool_constant<std::is_abstract_v<T>> {};

template<typename T>
struct FlagConditional<T, TypeFlag::GenericType>
{
	template<typename>
	constexpr static bool IsGenericType = false;

	template<template <typename...> typename T, typename ...TArgs>
	constexpr static bool IsGenericType<T<TArgs...>> = true;

	constexpr static bool value = IsGenericType<T>;
};

template<typename T>
struct FlagConditional<T, TypeFlag::Enum> : std::bool_constant<std::is_enum_v<T>> {};

template<typename T>
struct ExcludeFlags
{
	template<TypeFlag Flag>
	constexpr static BitMask<TypeFlag> FLAG = FlagConditional<T, Flag>::value ? Flag : TypeFlag(0);
	constexpr static BitMask<TypeFlag> Flags = FLAG<TypeFlag::Primitive> | FLAG<TypeFlag::Abstract> | FLAG<TypeFlag::GenericType> | FLAG<TypeFlag::Enum>;
};

struct ClassInfo
{
	const std::vector<ParentInfo> parentInfos;
	const std::vector<FieldInfo> fieldInfos;
	const std::vector<MethodInfo> methodInfos;

public:

	template<typename T>
	ClassInfo(std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		parentInfos(parentInfos),
		fieldInfos(fieldInfos),
		methodInfos(methodInfos)
	{
	}
};

struct TemplateClassInfo : public ClassInfo
{
	const char* name;
	std::vector<ParameterType>	parameterTypes;

public:

	template<template <typename...> typename T, typename ...TArgs>
	TemplateClassInfo(const char* name, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T<TArgs...>*) :
		ClassInfo(parentInfos, fieldInfos, methodInfos, (T<TArgs...>*)nullptr),
		name(name),
		parameterTypes(sizeof...(TArgs))
	{
		if constexpr (sizeof...(TArgs) > 0)
		{
			ExcludeTemplateParameterTypes<TArgs...>();
		}
	}

private:

	template<typename T, typename ...TOther>
	void ExcludeTemplateParameterTypes(uint32_t i = 0)
	{
		new(&parameterTypes[i]) ParameterType(ParameterType::Initializer<T>{});

		if constexpr (sizeof...(TOther) > 0)
			ExcludeTemplateParameterTypes<TOther...>(i + 1);
	}
};

struct EnumInfo
{
	TypePtr base;
	std::span<const EnumValue>	values;

public:

	template<typename T>
	EnumInfo(T*) :
		base(TypeOf<std::underlying_type_t<T>>::Get()),
		values(EnumValues<T>::Get())
	{
	}
};

struct TemplateInfo
{
	const char* name;
	std::vector<ParameterType>	parameterTypes;
};

union TypeInternalInfo
{
	ClassInfo			classInfo;
	TemplateClassInfo	templateClassInfo;
	EnumInfo			enumInfo;

	~TypeInternalInfo() {}
	TypeInternalInfo() {}

	template<typename T>
	TypeInternalInfo(std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		classInfo(parentInfos, fieldInfos, methodInfos, (T*)nullptr)
	{
	}

	template<typename T>
	TypeInternalInfo(const char* name, std::initializer_list<ParentInfo> parentInfos, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		templateClassInfo(name, parentInfos, fieldInfos, methodInfos, (T*)nullptr)
	{
	}

	template<typename T>
	TypeInternalInfo(T*) :
		enumInfo((T*)nullptr)
	{
	}
};