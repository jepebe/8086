#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase DIV");

    test_section("div.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/div.bin",
            "../tests/artlav/bins/results/res_div.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
