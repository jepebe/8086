#include <stdlib.h>
#include "testing.h"
#include "testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase DATATRNF");

    test_section("datatrnf.bin");
    test_testcase(
            &tester,
            "../tests/bins/datatrnf.bin",
            "../tests/bins/results/res_datatrnf.bin",
            false
    );

    test_summary(&tester);

    exit(tester.fail);
}
