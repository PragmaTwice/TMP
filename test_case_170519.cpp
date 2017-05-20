#define TEST_CASE_170519

#if defined TEST_CASE_170519

#include "meta.hpp"
#include <iostream>
#include <string>
#include <typeinfo>

DEFINE_has_typedef(value_type);

int main()
{
	using namespace std;
	// remove_ptr
	cout << "TEST:remove_ptr" << endl;

	cout << "\ttypeid(remove_ptr<char* const>::type).name()                      \t" << typeid(meta::remove_ptr<char* const>::type).name() << endl;
	cout << "\ttypeid(remove_ptr<int*>::type).name()                             \t" << typeid(meta::remove_ptr<int*>::type).name() << endl;
	cout << "\ttypeid(remove_ptr<char*>::type).name()                            \t" << typeid(meta::remove_ptr<char>::type).name() << endl << endl;
	cout << "\ttypeid(remove_ptr_all<char*****>::type).name()                    \t" << typeid(meta::remove_all_ptr<char*****>::type).name() << endl;
	cout << "\ttypeid(remove_ptr_all<char***const**const*const***>::type).name() \t" << typeid(meta::remove_all_ptr<char***const**const*const***>::type).name() << endl << endl;

	// can_converse_to
	cout << "TEST:can_converse_to" << endl;

	cout << "\tcan_converse_to<int, double>::value        \t" << meta::can_converse_to<int, double>::value << endl;
	cout << "\tcan_converse_to<const int&, int&>::value   \t" << meta::can_converse_to<const int&, int&>::value << endl;
	cout << "\tcan_converse_to<int&,int&>::value          \t" << meta::can_converse_to<int&,int&>::value << endl;
	cout << "\tcan_converse_to<float,int>::value          \t" << meta::can_converse_to<float,int>::value << endl;
	cout << "\tcan_converse_to<const char*,string>::value \t" << meta::can_converse_to<const char*,string>::value << endl << endl;

	// has_typedef value_type
	cout << "TEST:has_typedef value_type" << endl;

	cout << "\tstruct x { using value_type = void; };" << endl;
	struct x { using value_type = void; };
	cout << "\tstruct y { using fuck_you   = void; };" << endl << endl;
	struct y { using fuck_you = void; };

	cout << "\thas_typedef(value_type) <x>::value      \t" << meta:: has_typedef(value_type) <x>::value << endl;
	cout << "\thas_typedef(value_type) <y>::value      \t" << meta:: has_typedef(value_type) <y>::value << endl;
	cout << "\thas_typedef(value_type) <string>::value \t" << meta:: has_typedef(value_type) <string>::value << endl;
	cout << "\thas_typedef(value_type) <int>::value    \t" << meta:: has_typedef(value_type) <int>::value << endl << endl;

	// type_array
	cout << "TEST:type_array" << endl;

	struct a {};
	struct b {};

	cout << "\tarr = meta::type_array<int, bool, float, char, double, a, unsigned int, char, bool>" << endl << endl;
	using arr = meta::type_array<int, bool, float, char, double, a, unsigned int, char, bool>;

	cout << "\ttypeid(arr::at<0>::type).name():\t" << typeid(arr::at<0>::type).name() << endl;
	cout << "\ttypeid(arr::at<5>::type).name():\t" << typeid(arr::at<5>::type).name() << endl;
	cout << "\ttypeid(arr::at<8>::type).name():\t" << typeid(arr::at<8>::type).name() << endl;
	cout << "\tarr::find<float>::value:        \t" << arr::find<float>::value << endl;
	cout << "\tarr::find<char>::value:         \t" << arr::find<char>::value << endl;
	cout << "\tarr::find<char,4>::value:       \t" << arr::find<char,4>::value << endl;
	cout << "\tarr::find<a>::value:            \t" << arr::find<a>::value << endl;
	cout << "\tarr::find<b>::value:            \t" << (signed) arr::find<b>::value << endl;

	cout << endl << "TEST:end." << endl;
	cin.get();

	return 0;
}

#endif