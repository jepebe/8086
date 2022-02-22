#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase CONTROL");

    test_section("control.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/control.bin",
            "../tests/artlav/bins/results/res_control.bin",
            false
    );

    test_summary(&tester);

    exit(tester.fail);
}
