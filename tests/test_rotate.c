#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase ROTATE");

    test_section("rotate.bin");
    test_testcase(
            &tester,
            "../tests/bins/rotate.bin",
            "../tests/bins/res_rotate.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
