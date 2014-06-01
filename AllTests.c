#include <stdio.h>

#include "CuTest.h"

CuSuite* CuGetSuite();
CuSuite* CuStringGetSuite();

void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

    //CuSuiteAddSuite(suite, CuGetSuite());
    //CuSuiteAddSuite(suite, CuStringGetSuite());
    CuSuiteAddSuite(suite, AtomGetSuite());
    CuSuiteAddSuite(suite, BuiltinGetSuite());
    CuSuiteAddSuite(suite, EnvironmentGetSuite());
    CuSuiteAddSuite(suite, ParserGetSuite());
    CuSuiteAddSuite(suite, GcGetSuite());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
}

int main(void)
{
	RunAllTests();
}
