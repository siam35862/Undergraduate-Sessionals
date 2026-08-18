#include <stdio.h>
char *escaped = "Backslash test: \\";char *sql = "SELECT * FROM users; WHERE status = 1;";char quote_char = '"';int x = 10;
#define MACRO_TEST(a, b) ((a) + (b))
int main(){printf("Result: %d\n", MACRO_TEST(1, 2));return 0;}