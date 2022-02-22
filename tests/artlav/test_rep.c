#include <stdlib.h>
#include "../testing.h"
#include "../testing80x86.h"


int main() {
    Tester tester = create_tester("Testcase REP");

    test_section("rep.bin");
    test_testcase(
            &tester,
            "../tests/artlav/bins/rep.bin",
            "../tests/artlav/bins/results/res_rep.bin",
            true
    );

    test_summary(&tester);

    exit(tester.fail);
}
