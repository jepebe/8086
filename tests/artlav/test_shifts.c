#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase SHIFTS");

    test_section("shifts.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/shifts.bin",
            "../tests/artlav/bins/results/res_shifts.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
