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

	// lvalue ref

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

	// rvalue ref

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
	struct is_lr_ref
	{
		static constexpr bool value = is_same_type<T,typename remove_rm<T>::type>::value;
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

	template <typename T >
	struct add_volatile
	{
		using type = volatile T;
	};

	template <typename T >
	struct add_cv
	{
		using type = typename add_volatile<typename add_const<T>::type>::type;
	};

	class remove_ptr_private
	{
		template <typename T >
		struct impl
		{
			using type = T;
		};
		template <typename T >
		struct impl<T *>
		{
			using type = T;
		};
		template <typename T >
		friend struct remove_ptr;
	};
	template <typename T >
	struct remove_ptr
	{
		using type = typename remove_ptr_private::impl<typename remove_cv<T>::type>::type;
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

	// constant

	template <typename T, T v = T{} >
	struct constant
	{
		using value_type = T;
		static constexpr T value = v;
	};


	using true_type = constant<bool, true>;
	using false_type = constant<bool, false>;


	template<class T, class U>
	class can_converse_to
	{

		static true_type test(U);
		static false_type test(...);

		static T make_T();

	public:

		static constexpr bool value = decltype(test(make_T()))::value;

	};


#define DEFINE_has_typedef(_type)                                                                             \
	namespace meta                                                                                            \
	{                                                                                                         \
		template<typename T>                                                                                  \
		struct has_typedef_ ## _type                                                                          \
		{                                                                                                     \
			                                                                                                  \
			template<typename C> static true_type test(typename C:: _type *);                                 \
			template<typename> static false_type test(...);                                                   \
			                                                                                                  \
		public:                                                                                               \
			                                                                                                  \
			static constexpr bool value = decltype(test<T>(nullptr))::value;                                  \
			                                                                                                  \
		};                                                                                                    \
	}


#define has_typedef(_type) has_typedef_ ## _type


	// template void

	template <typename... T >
	using template_void = void;

	// template class

	template <template <typename... > class T, typename... U >
	using template_class = T<U...>;

	template <typename T >
	struct is_template_class_spec
	{
		static constexpr bool value = false;
	};

	template <template <typename... > class T, typename... U >
	struct is_template_class_spec <T<U...>>
	{
		static constexpr bool value = true;
	};

	template <typename T >
	struct get_template_from_spec
	{
		using type = void;
	};

	template <template <typename... > class T, typename... U >
	struct get_template_from_spec <T<U...>>
	{
		template <typename... V>
		using type = T<V...>;
	};

	// copy template para

	template <template <typename... > class T, typename U >
	struct copy_template_para 
	{
		using type = void;
	};

	template <template <typename... > class T, template <typename... > class U, typename... V >
	struct copy_template_para <T,U <V...>> 
	{
		using type = T <V...>;
	};

	// merge template para

	class merge_template_para_private
	{
		template <template <typename... > class T, typename U1, typename U2 >
		struct impl
		{
			using type = void;
		};
		template <template <typename... > class T, template <typename... > class U1, typename... V1, template <typename... > class U2, typename... V2 >
		struct impl <T, U1 <V1...>, U2 <V2...>>
		{
			using type = T <V1..., V2...>;
		};

		template <template <typename... > class T, typename U1, typename... Un >
		friend struct merge_template_para;
	};
	template <template <typename... > class T, typename U1, typename... Un >
	struct merge_template_para
	{
		using type = typename merge_template_para_private::impl<T,U1,typename merge_template_para<T,Un...>::type>::type;
	};
	template <template <typename... > class T, typename U1, typename U2 >
	struct merge_template_para <T,U1,U2>
	{
		using type = typename merge_template_para_private::impl<T, U1, U2>::type;
	};


	// type container

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

		template <unsigned k, typename T, unsigned n, typename U, typename... V >
		struct ignore_impl
		{
			static constexpr unsigned int value = ignore_impl<k + 1, T, n, V...>::value;
		};
		template <typename T, unsigned n, typename U, typename... V>
		struct ignore_impl<n, T, n, U, V...>
		{
			static constexpr unsigned int value = find_impl<T,n,U,V...>::value;
		};

		template <typename T, typename U, typename... V >
		struct count_impl
		{
			static constexpr unsigned int value = (is_same_type<T, U>::value ? 1 : 0) + count_impl<T,V...>::value;
		};
		template <typename T, typename U>
		struct count_impl <T,U>
		{
			static constexpr unsigned int value = (is_same_type<T, U>::value ? 1 : 0);
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
		
		template <unsigned... n >
		struct at_s
		{
			using type = typename type_array<typename at<n>::type...>;
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

		template <typename U >
		struct count
		{
			static constexpr unsigned int value = type_array_private::count_impl<U,T...>::value;
		};

	};

	// copy template para to type array

	template <typename U >
	struct make_type_array_bypara
	{
		using type = typename copy_template_para<type_array, U>::type;
	};

	// push back/forward template para

	template <typename T, typename... U >
	struct push_template_para_back
	{
		using type = typename merge_template_para<typename get_template_from_spec<T>::type, T, type_array<U...>>::type;
	};

	template <typename T, typename... U>
	struct push_template_para_forward
	{
		using type = typename merge_template_para<typename get_template_from_spec<T>::type, type_array<U...>, T>::type;
	};

	// pop back/forward template para

	template <template <typename... > typename V >
	class pop_template_para_forward_private
	{
		template <typename T, typename ...U >
		struct impl
		{
			using type = typename V<U...>;
		};

		template <typename T>
		friend class pop_template_para_forward;
	};
	template <typename T >
	class pop_template_para_forward
	{
		using impl_type = typename copy_template_para<
			typename pop_template_para_forward_private<
				typename get_template_from_spec<T>::type
			>::template impl,
			T
		>::type;

	public:

		using type = typename impl_type::type;

	};

	template <template <typename... > typename V >
	class pop_template_para_back_private
	{
		template <typename T, typename ...U >
		struct impl
		{
			using type = typename push_template_para_forward<typename impl<U...>::type,T>::type;
		};

		template <typename T >
		struct impl<T>
		{
			using type = V<>;
		};

		template <typename T>
		friend class pop_template_para_back;
	};
	template <typename T >
	class pop_template_para_back
	{
		using impl_type = typename copy_template_para<
			typename pop_template_para_back_private<
				typename get_template_from_spec<T>::type
			>::template impl,
			T
		>::type;

	public:

		using type = typename impl_type::type;

	};

	// type if

	template <typename T >
	struct is_void : constant <bool, is_same_type <void, typename remove_cv<T>::type>::value> {};

	using nullptr_type = decltype(nullptr);

	template <typename T >
	struct is_nullptr_type : is_same_type <nullptr_type, typename remove_cv<T>::type> {};

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
	struct is_floating_point :constant <bool, type_array <
		float, double, long double
	>::has<typename remove_cv<T>::type>::value> {};

	// array

	template <typename T >
	struct is_array : false_type {};
	template <typename T >
	struct is_array <T []> : true_type {};
	template <typename T, unsigned N >
	struct is_array <T [N]> : true_type {};

	template <typename T >
	struct array_dimensions : constant<unsigned, 0> {};
	template <typename T >
	struct array_dimensions <T[]> : constant<unsigned, array_dimensions<T>::value + 1> {};
	template <typename T, unsigned N >
	struct array_dimensions <T[N]> : constant<unsigned, array_dimensions<T>::value + 1> {};

	template<class T, unsigned N = 0>
	struct array_dimension_length : constant<unsigned, 0> {};
	template<class T>
	struct array_dimension_length<T[], 0> : constant<unsigned, (unsigned)(-1)> {};
	template<class T, unsigned N>
	struct array_dimension_length<T[], N> : array_dimension_length<T, N - 1> {};
	template<class T, unsigned L>
	struct array_dimension_length<T[L], 0> : constant<unsigned, L> {};
	template<class T, unsigned L, unsigned N>
	struct array_dimension_length<T[L], N> : array_dimension_length<T, N - 1> {};


	// whether a type is a class , struct , or union

	template <typename T >
	class is_class
	{

		template <class T> static true_type test(int T::*);
		template <class T> static false_type test(...);

	public:

		static constexpr bool value = decltype(test<T>(nullptr))::value;

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

	template <typename T >
	struct is_arithmetic : constant <
		bool,
		is_integral<T>::value ||
		is_floating_point<T>::value
	> {};

	template< class T >
	struct is_fundamental : constant <
		bool,
		is_arithmetic<T>::value ||
		is_void<T>::value ||
		is_nullptr_type<T>::value
	> {};


	// is base of

	class is_base_of_private
	{
		template <typename Base> static true_type test(Base*);
		template <typename Base> static false_type test(void*);

		template <typename Base, typename Derived>
		using impl = decltype(test<Base>((typename remove_cv<Derived>::type*)(nullptr)));

		template <typename Base, typename Derived>
		friend struct is_base_of;
	};

	template <typename Base, typename Derived>
	struct is_base_of : IF<
		is_class<Base>::value && is_class<Derived>::value,
		typename is_base_of_private::impl <Base, Derived>,
		false_type
	>::type {};

	// (un/)signed

	class is_signed_private
	{
		template<typename T, bool = is_arithmetic<T>::value>
		struct impl : constant<bool, T(-1) < T(0) > {};

		template<typename T>
		struct impl<T, false> : false_type {};

		template<typename T>
		friend struct is_signed;
	};

	template<typename T>
	struct is_signed : is_signed_private::impl<T>::type {};

	class is_unsigned_private
	{
		template<typename T, bool = is_arithmetic<T>::value>
		struct impl : constant<bool, T(0) < T(-1)> {};

		template<typename T>
		struct impl<T, false> : false_type {};

		template<typename T>
		friend struct is_unsigned;
	};

	template<typename T>
	struct is_unsigned : is_unsigned_private::impl<T>::type {};



	// enable if

	template <bool, typename T = void >
	struct enable_if {};

	template <typename T >
	struct enable_if <true, T> 
	{ 
		using type = T;
	};


}