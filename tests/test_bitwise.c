#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase BITWISE");

    test_section("bitwise.bin");
    test_testcase(
            &tester,
            "../tests/bins/bitwise.bin",
            "../tests/bins/results/res_bitwise.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
