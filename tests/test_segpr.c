#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase SEGPR");

    test_section("segpr.bin");
    test_testcase(
            &tester,
            "../tests/bins/segpr.bin",
            "../tests/bins/results/res_segpr.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
