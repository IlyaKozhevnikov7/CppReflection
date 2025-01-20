#pragma once

/*
*	Macro that is used to register primitive types
*/
#define NO_EXPORT
#define REFLECTION_PRIMITIVE_TYPE_FORWARD_DECLARATION(ExportMacro, T, Name) \
	namespace Reflection \
	{ \
		template<> inline constexpr bool IsReflectable<T> = true; \
		static_assert(std::is_arithmetic_v<T> || std::is_void_v<T>); \
		template<> struct ExportMacro TypeOf<T> final \
		{ \
			constexpr static uint32_t NameLength = sizeof(Name) - 1; \
			static const char* GetName(); \
			static TypePtr Get(); \
		}; \
	} \

#define REFLECTION_PRIMITIVE_TYPE_IMPLEMENTATION(T, Name) \
	namespace Reflection \
	{ \
		const char* TypeOf<T>::GetName() { return __GEN_TO_STRING(Name); } \
		TypePtr TypeOf<T>::Get() \
		{ \
			static Type type(nullptr, GetName(), {}, {}, {}, {}, {}, nullptr, (T*)nullptr); \
			return &type; \
		} \
	} \

/*
*	!!! Don't explicitly use these macros !!!
*/
#define __GEN_TO_STRING_INTERNAL(S) #S
#define __GEN_TO_STRING(String) __GEN_TO_STRING_INTERNAL(String)
#define __GEN_COMBINE_STRING(S1, S2) __GEN_TO_STRING(S1 ## S2)
#define __GEN_ARG(...) __VA_ARGS__

#define __GEN_REFLECTION_NO_NAMESPACE_WRAPPER(typeOf, T, ...) typeOf T
#define __GEN_REFLECTION_NAMESPACE_WRAPPER(typeOf, T, Namespace) namespace Namespace { typeOf T; }
#define __GEN_REFLECTION_NAMESPACE_ENUM_WRAPPER(ForwardDecl, Namespace) namespace Namespace { ForwardDecl; }

#define __GEN_COMBINE_NAMESPACE(a, b) a ## :: ## b

#define __GEN_REFLECTION_TYPE_OF_FORWARD_DECLARATION(ExportMacro, Name, FullName) \
	namespace Reflection \
	{ \
		template<> inline constexpr bool IsReflectable<FullName> = true; \
		template<> \
		struct ExportMacro TypeOf<FullName> final \
		{ \
			constexpr static uint32_t NameLength = sizeof(__GEN_TO_STRING(Name)) - 1; \
			static const char* GetName(); \
			static TypePtr Get(); \
		}; \
	} \

#define __GEN_REFLECTION_TYPE_FORWARD_DECLARATION(ExportMacro, typeOf, T, Wrapper, Namespace) \
	Wrapper(typeOf, T, Namespace); \
	__GEN_REFLECTION_TYPE_OF_FORWARD_DECLARATION(ExportMacro, T, __GEN_COMBINE_NAMESPACE(Namespace, T)) \


#define __GEN_REFLECTION_GET_NAME_IMPLEMENTATION(Name) \
	const char* TypeOf<__CURRENT_TYPE__>::GetName() \
	{ \
		return __GEN_TO_STRING(Name); \
	} \

#define __GEN_REFLECTION_ENUM_TYPE_FORWARD_DECLARATION(ExportMacro, T, Wrapper, ForwardDecl, Namespace) \
	Wrapper(ForwardDecl, Namespace); \
	__GEN_REFLECTION_TYPE_OF_FORWARD_DECLARATION(ExportMacro, T, __GEN_COMBINE_NAMESPACE(Namespace, T)) \
	namespace Reflection \
	{ \
		template<> \
		struct ExportMacro EnumValues<__GEN_COMBINE_NAMESPACE(Namespace, T)> final \
		{ \
			static const std::span<const EnumValue> Get(); \
		}; \
	} \

#define __GEN_COMBINE_NAMES_INTERNAL(A, B) A ## B
#define __GEN_COMBINE_NAMES(A, B) __GEN_COMBINE_NAMES_INTERNAL(A, B)

#define __GEN_ACTUAL_TYPE_FUNCTION_NAME_INTERNAL(N) __GEN_COMBINE_NAMES(__GET_ACTUAL_TYPE_, N)
#define __GEN_ACTUAL_TYPE_FUNCTION_NAME(T) __GEN_ACTUAL_TYPE_FUNCTION_NAME_INTERNAL(T)
#define __GEN_REFLECTION_GET_ACTUAL_TYPE_FUNCTION \
	static const Type* __GEN_ACTUAL_TYPE_FUNCTION_NAME(__CURRENT_TYPE_NAME_STRING__)(void* context) \
	{ \
		return reinterpret_cast<__CURRENT_TYPE__*>(context)->GetType(); \
	} \

#define __GEN_CREATE_INSTANCE_FUNCTION_NAME_INTERNAL(N) __GEN_COMBINE_NAMES(__CREATE_INSTANCE_, N)
#define __GEN_CREATE_INSTANCE_FUNCTION_NAME(T) __GEN_CREATE_INSTANCE_FUNCTION_NAME_INTERNAL(T)

#define __GEN_REFLECTION_CREATE_INSTANCE_FUNCTION \
	static void* __GEN_CREATE_INSTANCE_FUNCTION_NAME(__CURRENT_TYPE_NAME_STRING__)() \
	{ \
		return nullptr; \
	} \

/*
*  ========== Type generation ==========
*/

/*
*  ---------- Attributes ----------
*/
#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_BEGIN { {
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE(T, ...) Attribute::Create<T>(__VA_ARGS__),
#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTE_END },
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES  { { },
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_TYPE_NO_ATTRIBUTES { },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_BEGIN_T {
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_ATTRIBUTES_END_T },
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_ATTRIBUTES_T { },

/*
*  ---------- Class/Struct ----------
*/
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_BEGIN(Namespace) \
	TypePtr TypeOf<__CURRENT_TYPE__>::Get() \
	{ \
		static Type type; \
		static bool initialized = false; \
		if (initialized == false) { \
			initialized = true; \
			new(&type) Type( \
				Namespace, \
				GetName(), \

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_END \
				&Reflection::Generation::GetActualType<__CURRENT_TYPE__>, \
				(__CURRENT_TYPE__*)nullptr); \
		} \
		return &type; \
	} \
	
/*
*  ---------- Parents ----------
*/
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_PARENT_INFO { },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_BEGIN {
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO(Id) { TypeOf<__CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::TParent##Id>::Get(), (const size_t)((uint8_t*)(typename __CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::TParent##Id*)(__CURRENT_TYPE__*)(sizeof(__CURRENT_TYPE__)) - (uint8_t*)(__CURRENT_TYPE__*)sizeof(__CURRENT_TYPE__)) },
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_PARENT_INFO_END },

/*
*  ---------- Fields ----------
*/
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_FIELD_INFO { },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_BEGIN {
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELD_INFO(Name) __GEN_TO_STRING(Name), Reflection::Generation::GetOffsetOfField(&__CURRENT_TYPE__::Name), Reflection::Generation::IsStaticField(&__CURRENT_TYPE__::Name), (decltype(__CURRENT_TYPE__::Name)*)(nullptr) },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_FIELDS_END },

/*
*  ---------- Methods ----------
*/
#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_METHOD_INFO { },

#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO_BEGIN {
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFO(Name, Id, Flags) __GEN_TO_STRING(Name), &Generation::ProxyInvoker<__CURRENT_TYPE__>::Invoke_m##Id, {}, (const __CURRENT_TYPE__*)nullptr, decltype(&__CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::m##Id)(nullptr), Reflection::MethodFlags(Flags) },

#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_METHOD_INFOS_END },

/*
*  ---------- Constructors ----------
*/
#define	__GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_NO_CONSTRUCTOR_INFO { },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTORS_BEGIN {{
#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTOR_INFO(Id) &Generation::ProxyCtor<__CURRENT_TYPE__, decltype(__CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::c##Id)>::Func },

#define __GEN_REFLECTION_GET_TYPE_IMPLEMENTATION_CONSTRUCTORS_END }, &Generation::ProxyDestructor<__CURRENT_TYPE__>::Func },

/*
*  ========== Template generation ==========
*/

#define __GEN_REFLECTION_NAMESPACE_CONVERTER(...) __GEN_TO_STRING(__VA_ARGS__)
#define __GEN_REFLECTION_NULL_NAMESPACE_CONVERTER(...) nullptr

#define __GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_BEGIN(T, Converter, Namespace) \
    namespace Reflection { \
	template<typename ...TArgs> inline constexpr bool IsReflectable<__CURRENT_TYPE__> = true; \
    template<typename ...TArgs> struct TypeOf<__CURRENT_TYPE__> final \
    { \
        constexpr static uint32_t NameLength = sizeof(__GEN_TO_STRING(T)) + 1 + Reflection::Generation::FullNameLength<TArgs...>::value + (sizeof...(TArgs) < 2 ? 0 : (sizeof...(TArgs) - 1) * 2); \
    private: \
        constexpr static const char* NamespaceName = Converter(Namespace); \
        struct Name final : public Reflection::Generation::GenericTypeName<NameLength, TArgs...> \
        { \
            constexpr Name(const char* templateName, int size) : Reflection::Generation::GenericTypeName<NameLength, TArgs...>(templateName, size) { } \
        }; \
    public: \
        static const char* GetName() \
        { \
            static Name name(Generation::__GEN_COMBINE_NAMES(GetTemplateMemberName_, __GEN_REFLECTION_TEMPLATE_MEMBER_0)##_0(), sizeof(__GEN_TO_STRING(T)) - 1); \
            return name; \
        } \
        static TypePtr Get() \
        { \
            static Type type; \
            static bool initialized = false; \
            if (initialized == false) \
            { \
                initialized = true; \
                new(&type) Type(Generation::__GEN_COMBINE_NAMES(GetTemplateMemberName_, __GEN_REFLECTION_TEMPLATE_MEMBER_0)##_0(), \
                    NamespaceName, \
                    GetName(), \

#define __GEN_REFLECTION_TEMPLATE_TYPE_FORWARD_DECLARATION_END \
                    &Reflection::Generation::GetActualType<__CURRENT_TYPE__>, \
                    (__CURRENT_TYPE__*)nullptr); \
            } \
            return &type; \
        } \
    }; \
}; \

#define __GEN_REFLECTION_TEMPLATE_MEMBER_NAME_FUNCTION(ExportMacro, MemberId) ExportMacro extern const char* __GEN_COMBINE_NAMES(GetTemplateMemberName_, __GEN_REFLECTION_TEMPLATE_MEMBER_0)##_##MemberId();

#define __GEN_REFLECTION_TEMPLATE_GET_TYPE_IMPLEMENTATION_FIELD_INFO(MemberId) Generation::__GEN_COMBINE_NAMES(GetTemplateMemberName_, __GEN_REFLECTION_TEMPLATE_MEMBER_0)##_##MemberId(), Reflection::Generation::GetOffsetOfField(&__CURRENT_TYPE__::__GEN_REFLECTION_TEMPLATE_MEMBER_##MemberId), Reflection::Generation::IsStaticField(&__CURRENT_TYPE__::__GEN_REFLECTION_TEMPLATE_MEMBER_##MemberId), (decltype(__CURRENT_TYPE__::__GEN_REFLECTION_TEMPLATE_MEMBER_##MemberId)*)(nullptr) },
#define __GEN_REFLECTION_TEMPLATE_GET_TYPE_IMPLEMENTATION_METHOD_INFO(MemberId, Id, Flags) Generation::__GEN_COMBINE_NAMES(GetTemplateMemberName_, __GEN_REFLECTION_TEMPLATE_MEMBER_0)##_##MemberId(), &Generation::ProxyInvoker<__CURRENT_TYPE__>::Invoke_m##Id, {}, (const __CURRENT_TYPE__*)nullptr, decltype(&__CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::m##Id)(nullptr), Reflection::MethodFlags(Flags) },

/*
*  ========== Enum generation ==========
*/

#define __GEN_REFLECTION_ENUM_VALUE(Name) { __GEN_TO_STRING(Name), static_cast<int64_t>(__CURRENT_TYPE__::Name), }

#define __GEN_REFLECTION_ENUM_VALUES_IMPLEMENTATION(Count, ...) \
	const std::span<const EnumValue> EnumValues<__CURRENT_TYPE__>::Get() \
	{ \
		static std::array<const EnumValue, Count> values = { { __VA_ARGS__ } }; \
		return values; \
	} \

#define	__GEN_REFLECTION_GET_ENUM_TYPE_IMPLEMENTATION(Namespace) \
	TypePtr TypeOf<__CURRENT_TYPE__>::Get() \
	{ \
		static Type type( \
			Namespace, \
			GetName(), \
			(__CURRENT_TYPE__*)nullptr); \
		return &type; \
	} \

/*
*	========== Proxy Invoker generation ==========
*/

#define __GEN_PROXY_INVOKER_BEGIN \
namespace Generation  \
{ \
	template<> \
	struct ProxyInvoker<__CURRENT_TYPE__> \
	{ \

#define __GEN_TEMPLATE_PROXY_INVOKER_BEGIN \
	namespace Reflection::Generation  \
	{ \
		template<typename ...TArgs> \
		struct ProxyInvoker<__CURRENT_TYPE__> \
		{ \

#define __GEN_PROXY_INVOKER_END }; }

#define __GEN_PASS_ARGS Generation::PassArgs<Generation::TypeByIndex_t<Indices, _TArgs...>>(info->args, Generation::GetOffset<Indices, 0, _TArgs...>())...
#define __GEN_PROXY_METHOD(Name, Id) \
	template<typename TSignature, typename... _TArgs, size_t... Indices> \
	static void InvokeInternal_m##Id(const InvokeInfo* info, std::tuple<_TArgs...>*, std::index_sequence<Indices...>) \
	{ \
		using Info = FunctionCoreInfo<TSignature>; \
		using ReturnType = Info::ReturnType; \
		if constexpr (std::is_member_function_pointer_v<TSignature>) \
		{ \
			if constexpr (std::is_void_v<ReturnType>) \
			{ \
				reinterpret_cast<__CURRENT_TYPE__*>(info->object)->Name(__GEN_PASS_ARGS); \
			} \
			else \
			{ \
				if constexpr (std::is_move_constructible_v<ReturnType>) \
				{ \
					new(info->returnData) ReturnType(std::move(reinterpret_cast<__CURRENT_TYPE__*>(info->object)->Name(__GEN_PASS_ARGS))); \
				} \
				else \
				{ \
					new(info->returnData) ReturnType(reinterpret_cast<__CURRENT_TYPE__*>(info->object)->Name(__GEN_PASS_ARGS)); \
				} \
			} \
		} \
		else \
		{ \
			if constexpr (std::is_void_v<ReturnType>) \
			{ \
				__CURRENT_TYPE__::Name(__GEN_PASS_ARGS); \
			} \
			else \
			{ \
				if constexpr (std::is_move_constructible_v<ReturnType>) \
				{ \
					new(info->returnData) ReturnType(std::move(__CURRENT_TYPE__::Name(__GEN_PASS_ARGS))); \
				} \
				else \
				{ \
					new(info->returnData) ReturnType(__CURRENT_TYPE__::Name(__GEN_PASS_ARGS)); \
				} \
			} \
		} \
	} \
	static void Invoke_m##Id(const InvokeInfo* info) \
	{ \
		using Signature = decltype(&__CURRENT_TYPE__::__GEN_REFLECTION_TYPE_META::m##Id); \
		using Args = FunctionCoreInfo<Signature>::Args; \
		InvokeInternal_m##Id<Signature>(info, (Args*)nullptr, std::make_index_sequence<std::tuple_size_v<Args>>{}); \
	} \

/*
*	========== Inline generation ==========
*/

#define __GEN_VIRTUAL_SPECIFIER virtual
#define __GEN_EMPTY_SPECIFIER

#define __GEN_REFLECTION_TYPE_INLINE_CORE(VirtualSpecifier) \
	private: \
		friend Reflection::TypeOf<__THIS_TYPE__>; \
		friend Reflection::Generation::ProxyInvoker<__THIS_TYPE__>; \
	public: \
		VirtualSpecifier Reflection::TypePtr GetType() const \
		{ \
			return Reflection::TypeOf<__THIS_TYPE__>::Get(); \
		} \

#define __GEN_REFLECTION_TYPE_INLINE_META_BEGIN \
	private: \
		struct __GEN_REFLECTION_TYPE_META final \
		{ \


#define __GEN_REFLECTION_TYPE_INLINE_META_END };
