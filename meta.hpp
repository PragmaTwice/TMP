// PRAGMA TWICE 2017.5.19

#pragma once

namespace meta
{
	// base 

	template <bool, typename Then, typename Else >
	struct IF
	{
	};
	template <typename Then, typename Else >
	struct IF <true, Then, Else>
	{
		using type = Then;
	};
	template <typename Then, typename Else >
	struct IF <false, Then, Else>
	{
		using type = Else;
	};

	template <template <typename > class Condition, typename Statement >
	class WHILE
	{
		template <typename Statement > struct STOP
		{
			using type = Statement;
		};

	public:
		using type =
			typename IF<
			Condition<Statement>::value,
			WHILE<Condition, typename Statement::next_type>,
			STOP<Statement>
			>::type::type;
	};

	// type operation

	template <typename T, typename U>
	struct is_same_type
	{
		static constexpr bool value = false;
	};
	template <typename T >
	struct is_same_type <T, T>
	{
		static constexpr bool value = true;
	};


	template <typename T >
	struct remove_ptr
	{
		using type = T;
	};
	template <typename T >
	struct remove_ptr<T *>
	{
		using type = T;
	};
	template <typename T >
	struct remove_ptr<T * const>
	{
		using type = T;
	};
	template <typename T >
	struct remove_ptr<T * volatile>
	{
		using type = T;
	};
	template <typename T >
	struct remove_ptr<T * const volatile>
	{
		using type = T;
	};

	template <typename T >
	struct is_not_ptr
	{
		static constexpr bool value = is_same_type<T, typename remove_ptr<T>::type>::value;
	};
	template <typename T >
	struct is_ptr
	{
		static constexpr bool value = !is_not_ptr<T>::value;
	};

	class remove_all_ptr_private
	{
		template <bool, typename T >
		struct impl
		{
		};
		template <typename T >
		struct impl<false, T>
		{
			using type =
				typename impl<is_not_ptr<T>::value, typename remove_ptr<T>::type>::type;
		};
		template <typename T >
		struct impl<true, T>
		{
			using type = T;
		};

		template <typename T >
		friend struct remove_all_ptr;
	};
	template <typename T >
	struct remove_all_ptr
	{
		using type = typename remove_all_ptr_private::impl<is_not_ptr<T>::value, T>::type;
	};


	template<typename T >
	struct remove_ref
	{
		using type = T;
	};
	template<typename T >
	struct remove_ref <T&>
	{
		using type = T;
	};
	template<typename Ret, typename... Args >
	struct remove_ref <Ret(Args...) & >
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct is_not_ref
	{
		static constexpr bool value = is_same_type<T, typename remove_ref<T>::type>::value;
	};
	template <typename T >
	struct is_ref
	{
		static constexpr bool value = !is_not_ref<T>::value;
	};

	template<typename T >
	struct remove_move
	{
		using type = T;
	};
	template<typename T >
	struct remove_move <T&&>
	{
		using type = T;
	};
	template<typename Ret, typename... Args >
	struct remove_move <Ret(Args...) &&>
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct is_not_move
	{
		static constexpr bool value = is_same_type<T, typename remove_move<T>::type>::value;
	};
	template <typename T >
	struct is_move
	{
		static constexpr bool value = !is_not_move<T>::value;
	};

	template <typename T >
	struct remove_rm
	{
		using type = typename remove_move<typename remove_ref<T>::type>::type;
	};

	template <typename T >
	struct remove_const
	{
		using type = T;
	};
	template <typename T >
	struct remove_const <const T>
	{
		using type = T;
	};
	template <typename Ret, typename... Args >
	struct remove_const <Ret(Args...) const>
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct is_not_const
	{
		static constexpr bool value = is_same_type<T, typename remove_const<T>::type>::value;
	};
	template <typename T >
	struct is_const
	{
		static constexpr bool value = !is_not_const<T>::value;
	};


	template <typename T >
	struct remove_volatile
	{
		using type = T;
	};
	template <typename T >
	struct remove_volatile <volatile T>
	{
		using type = T;
	};
	template <typename Ret, typename... Args >
	struct remove_volatile <Ret(Args...) volatile>
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct is_not_volatile
	{
		static constexpr bool value = is_same_type<T, typename remove_volatile<T>::type>::value;
	};
	template <typename T >
	struct is_volatile
	{
		static constexpr bool value = !is_not_volatile<T>::value;
	};

	template <typename T >
	struct remove_cv
	{
		using type = typename remove_const<typename remove_volatile<T>::type>::type;
	};
	template <typename Ret, typename... Args >
	struct remove_cv <Ret(Args...) const volatile>
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct remove_cvrm
	{
		using type = typename remove_cv<typename remove_rm<T>::type>::type;
	};
	template <typename Ret, typename... Args >
	struct remove_cvrm <Ret(Args...) const volatile &>
	{
		using type = typename Ret(Args...);
	};
	template <typename Ret, typename... Args >
	struct remove_cvrm <Ret(Args...) const volatile &&>
	{
		using type = typename Ret(Args...);
	};

	template <typename T >
	struct add_ref
	{
		using type = T &;
	};

	template <typename T >
	struct add_move
	{
		using type = typename remove_ref<T>::type &&;
	};

	template <typename T >
	struct add_const
	{
		using type = const T;
	};


	template<class T, class U>
	class can_converse_to
	{

		struct can_converse {};
		struct cannot_converse {};

		static can_converse test(U);
		static cannot_converse test(...);

		static T make_T();

	public:

		static constexpr bool value = is_same_type<decltype(test(make_T())), can_converse>::value;

	};


#define DEFINE_has_typedef(_type)                                                                             \
	namespace meta                                                                                            \
	{                                                                                                         \
		template<typename T>                                                                                  \
		struct has_typedef_ ## _type                                                                          \
		{                                                                                                     \
			struct does_have_typedef {};                                                                      \
			struct not_has_typedef {};                                                                        \
			                                                                                                  \
			template<typename C> static does_have_typedef test(typename C:: _type *);                         \
			template<typename> static not_has_typedef test(...);                                              \
			                                                                                                  \
		public:                                                                                               \
			                                                                                                  \
			static constexpr bool value = is_same_type<decltype(test<T>(nullptr)), does_have_typedef>::value; \
			                                                                                                  \
		};                                                                                                    \
	}


#define has_typedef(_type) has_typedef_ ## _type


	//type container
	class type_array_private
	{
		static constexpr unsigned int nopos = -1;

		template <unsigned n, typename U, typename... V >
		struct at_impl
		{
			using type = typename at_impl<n - 1,V...>::type;
		};
		template <typename U, typename... V >
		struct at_impl <0,U,V...>
		{
			using type = U;
		};

		template <typename T, unsigned k, typename U, typename... V >
		struct find_impl
		{
			static constexpr unsigned int value = is_same_type<T, U>::value ? k : find_impl<T, k + 1, V...>::value;
		};
		template <typename T, unsigned k, typename U>
		struct find_impl<T, k, U>
		{
			static constexpr unsigned int value = is_same_type<T, U>::value ? k : nopos;
		};

		template < unsigned k, typename T, unsigned n, typename U, typename... V >
		struct ignore_impl
		{
			static constexpr unsigned int value = ignore_impl<k + 1, T, n, V...>::value;
		};
		template <typename T, unsigned n, typename U, typename... V>
		struct ignore_impl<n, T, n, U, V...>
		{
			static constexpr unsigned int value = find_impl<T,n,U,V...>::value;
		};

		template <typename... T >
		friend struct type_array;
	};
	
	template <typename... T >
	struct type_array
	{
		static constexpr unsigned int size = sizeof...(T);
		static constexpr unsigned int nopos = type_array_private::nopos;

		template <unsigned n >
		struct at
		{
			using type = typename type_array_private::at_impl<n, T...>::type;
		};

		template <typename U, unsigned start_pos = 0 >
		struct find
		{
			static constexpr unsigned int value = type_array_private::ignore_impl<0, U, start_pos, T...>::value;
		};

		template <typename U >
		struct has
		{
			static constexpr bool value = find<U>::value != nopos;
		};

	};


	// constant

	template <typename T, T v = T{} >
	struct constant
	{
		using value_type = T;
		static constexpr T value = v;
	};

	using true_type = constant<bool, true>;
	using false_type = constant<bool, false>;

	// type if

	template <typename T >
	struct is_void : constant <bool, is_same_type<void, typename remove_cv<T>::type>::value> {};

	template <typename T >
	struct is_integral : constant <bool, type_array<
		bool,
		char, signed char, unsigned char,
		char16_t, char32_t, wchar_t,
		signed short, unsigned short,
		signed int, unsigned int,
		signed long, unsigned long,
		signed long long, unsigned long long
	>::has<typename remove_cv<T>::type>::value> {};

	template <typename T >
	struct is_floating_point :constant<bool, type_array<
		float, double, long double
	>::has<typename remove_cv<T>::type>::value> {};

	template <typename T >
	struct is_array : false_type {};
	template <typename T >
	struct is_array <T []> : true_type {};
	template <typename T, unsigned N >
	struct is_array <T [N]> : true_type {};

	// whether a type is a class , struct , or union
	template <typename T >
	class is_class
	{
		struct does_be_class {};
		struct is_not_class {};

		template <class T> does_be_class test(int T::*);
		template <class T> is_not_class test(...);

	public:

		static constexpr bool value = is_same_type<decltype(test<T>(nullptr)), does_be_class>::value;

	};

	class is_function_private
	{
		template <typename T >
		struct impl : false_type {};

		template <typename Ret, typename... Args >
		struct impl <Ret(Args...)> :true_type {};

		template <typename Ret, typename... Args >
		struct impl <Ret(Args...,...)> :true_type {};

		template <typename T >
		friend struct is_function;
	};
	template <typename T >
	struct is_function : is_function_private::impl<typename remove_cvrm<T>::type> {};


}