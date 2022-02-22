#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase JUMP2");

    test_section("jump2.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/jump2.bin",
            "../tests/artlav/bins/results/res_jump2.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
