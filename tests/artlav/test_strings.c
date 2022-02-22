#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase STRINGS");

    test_section("strings.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/strings.bin",
            "../tests/artlav/bins/results/res_strings.bin",
            false
    );

    test_summary(&tester);

    exit(tester.fail);
}
