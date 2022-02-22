#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase JUMP1");

    test_section("jump1.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/jump1.bin",
            "../tests/artlav/bins/results/res_jump1.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
