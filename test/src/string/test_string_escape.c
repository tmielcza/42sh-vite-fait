#include "header.h"
#include "criterion/criterion.h"

TestSuite(string_escape);

Test(string_escape, escape) {

	t_string	str;

	string_init_dup(&str, "Hello World");
	string_escape_chars(&str, 'l');
	cr_assert_str_eq("He\\l\\lo Wor\\ld", str.str);
}

Test(string_escape, unescape) {

	t_string	str;

	string_init_dup(&str, "He\\l\\lo Wor\\ld");
	string_unescape_chars(&str, 'l');
	cr_assert_str_eq("Hello World", str.str);
}

Test(string_escape, unescape_no_match) {

	t_string	str;

	string_init_dup(&str, "He\\\\l\\lo Wor\\ld");
	string_unescape_chars(&str, 'l');
	cr_assert_str_eq("He\\\\llo World", str.str);
}

Test(string_escape, escape_Newline) {

	t_string	str;

	string_init_dup(&str, "Hello World\nAnd good morning!\n");
	string_escape_chars(&str, '\n');
	cr_assert_str_eq("Hello World\\\nAnd good morning!\\\n", str.str);
}
