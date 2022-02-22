#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase MUL");

    test_section("mul.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/mul.bin",
            "../tests/artlav/bins/results/res_mul.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
