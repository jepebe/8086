#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase ADD");

    test_section("add.bin");
    test_testcase(
            &tester,
            "../tests/bins/add.bin",
            "../tests/bins/results/res_add.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
