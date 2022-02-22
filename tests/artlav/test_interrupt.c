#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase INTERRUPT");

    test_section("interrupt.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/interrupt.bin",
            "../tests/artlav/bins/results/res_interrupt.bin",
            false
    );

    test_summary(&tester);

    exit(tester.fail);
}
