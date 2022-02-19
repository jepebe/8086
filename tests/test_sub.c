#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase SUB");

    test_section("sub.bin");
    test_testcase(
            &tester,
            "../tests/bins/sub.bin",
            "../tests/bins/results/res_sub.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
