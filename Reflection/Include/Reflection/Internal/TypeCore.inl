template<typename T, TypeFlag Flag>
struct FlagConditional;

template<typename T>
struct FlagConditional<T, TypeFlag::Primitive> : std::bool_constant<std::is_arithmetic_v<T> || std::is_void_v<T>> {};

template<typename T>
struct FlagConditional<T, TypeFlag::Abstract> : std::bool_constant<std::is_abstract_v<T>> {};

template<typename T>
struct FlagConditional<T, TypeFlag::TemplateType>
{
	template<typename>
	constexpr static bool IsTemplate = false;

	template<template <typename...> typename T, typename ...TArgs>
	constexpr static bool IsTemplate<T<TArgs...>> = true;

	constexpr static bool value = IsTemplate<T>;
};

template<typename T>
struct FlagConditional<T, TypeFlag::Enum> : std::bool_constant<std::is_enum_v<T>> {};

template<typename T>
struct ExcludeFlags
{
	template<TypeFlag Flag>
	constexpr static BitMask<TypeFlag> FLAG = FlagConditional<T, Flag>::value ? Flag : TypeFlag(0);
	constexpr static BitMask<TypeFlag> Flags = FLAG<TypeFlag::Primitive> | FLAG<TypeFlag::Abstract> | FLAG<TypeFlag::TemplateType> | FLAG<TypeFlag::Enum>;
};

struct ClassInfo
{
	const std::vector<const Type*>	parentTypes;
	const std::vector<FieldInfo>	fieldInfos;
	const std::vector<MethodInfo>	methodInfos;

public:

	template<typename T>
	ClassInfo(std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		parentTypes(parentTypes),
		fieldInfos(fieldInfos),
		methodInfos(methodInfos)
	{
	}
};

struct TemplateClassInfo : public ClassInfo
{
	const char* templateName;
	std::vector<ParameterType>	parameterTypes;

public:

	template<template <typename...> typename T, typename ...TArgs>
	TemplateClassInfo(const char* templateName, std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T<TArgs...>*) :
		ClassInfo(parentTypes, fieldInfos, methodInfos, (T<TArgs...>*)nullptr),
		templateName(templateName),
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
		++i;

		if constexpr (sizeof...(TOther) > 0)
			ExcludeTemplateParameterTypes<TOther...>(i);
	}
};

struct EnumInfo
{
	const Type* base;
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
	TypeInternalInfo(std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		classInfo(parentTypes, fieldInfos, methodInfos, (T*)nullptr)
	{
	}

	template<typename T>
	TypeInternalInfo(const char* templateName, std::initializer_list<const Type*> parentTypes, std::initializer_list<FieldInfo> fieldInfos, std::initializer_list<MethodInfo> methodInfos, T*) :
		templateClassInfo(templateName, parentTypes, fieldInfos, methodInfos, (T*)nullptr)
	{
	}

	template<typename T>
	TypeInternalInfo(T*) :
		enumInfo((T*)nullptr)
	{
	}
};