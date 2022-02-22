#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase SEGPR");

    test_section("segpr.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/segpr.bin",
            "../tests/artlav/bins/results/res_segpr.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
