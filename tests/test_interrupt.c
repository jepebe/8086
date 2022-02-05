#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase INTERRUPT");

    test_section("interrupt.bin");
    test_testcase(
            &tester,
            "../tests/bins/interrupt.bin",
            "../tests/bins/res_interrupt.bin",
            false
    );

    test_summary(&tester);

    exit(tester.fail);
}
