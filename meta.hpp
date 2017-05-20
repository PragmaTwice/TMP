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

	// type
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
	struct is_not_ptr
	{
		static constexpr bool value = is_same_type<T, typename remove_ptr<T>::type>::value;
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


	template <typename T >
	struct is_not_ref
	{
		static constexpr bool value = is_same_type<T, typename remove_ref<T>::type>::value;
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

	template <typename T >
	struct is_not_move
	{
		static constexpr bool value = is_same_type<T, typename remove_move<T>::type>::value;
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


	template <typename T >
	struct is_not_const
	{
		static constexpr bool value = is_same_type<T, typename remove_const<T>::type>::value;
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
		template <unsigned n >
		struct at
		{
			using type = typename type_array_private::at_impl<n, T...>::type;
		};

		template<typename U, unsigned start_pos = 0 >
		struct find
		{
			static constexpr unsigned int value = type_array_private::ignore_impl<0, U, start_pos, T...>::value;
		};

		static constexpr unsigned int size = sizeof...(T);
		static constexpr unsigned int nopos = type_array_private::nopos;
	};

}