#include <stdlib.h>
#include <stdio.h>
// gcc driver.c -o driver
int main()
{
	// TEST1
	printf("\n\n****************************************************************************************************************************\n\nTEST: DIVISIBILITY\n");
	int status = system("gcc ./DIVISOR_PROBABILITY/divisibilityTest.c -o  ./DIVISOR_PROBABILITY/divisibilityTest -lgmp");
	status = system("./DIVISOR_PROBABILITY/divisibilityTest");
	// TEST2
	printf("\n\n\n\n****************************************************************************************************************************\n\nTEST: FAST REMAINNDER W/O COMPILER OPTIMIZATION\n");
	status = system("gcc ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod.c -o ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod -lgmp");
	status = system("./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod");

	printf("\nTEST: FAST REMAINNDER WITH COMPILER OPTIMIZATION (-O3)\n");
	status = system("gcc ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod.c -o ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod -lgmp -O3");
	status = system("./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod");
	// TEST3
	printf("\n\n\n\n****************************************************************************************************************************\n\nTEST: XGCD 31-BIT RUNTIME W/O COMPILER OPTIMIZATION\n");
	status = system("gcc ./XGCD/xgcd.c -o ./XGCD/xgcd -lgmp");
	status = system("./XGCD/xgcd");
	printf("\nTEST: XGCD 31-BIT RUNTIME WITH COMPILER OPTIMIZATION (-O3)\n");
	status = system("gcc ./XGCD/xgcd.c -o ./XGCD/xgcd -lgmp -O3");
	status = system("./XGCD/xgcd");
	// TEST4
	printf("\n\n\n\n****************************************************************************************************************************\n\nTEST: BXGCD 31-BIT RUNTIME W/O COMPILER OPTIMIZATION\n");
	status = system("gcc ./BXGCD/bxgcd.c -o ./BXGCD/bxgcd -lgmp");
	status = system("./BXGCD/bxgcd");
	printf("\nTEST: BXGCD 31-BIT RUNTIME WITH COMPILER OPTIMIZATION (-O3)\n");
	status = system("gcc ./BXGCD/bxgcd.c -o ./BXGCD/bxgcd -lgmp -O3");
	status = system("./BXGCD/bxgcd");
	// TEST5
	printf("\n\n\n\n****************************************************************************************************************************\n\nTEST: FAST BXGCD 31-BIT RUNTIME W/O COMPILER OPTIMIZATION\n");
	status = system("gcc ./BXGCD_FASTDIV2/bxgcd_fast_div2.c -o ./BXGCD_FASTDIV2/bxgcd_fast_div2 -lgmp");
	status = system("./BXGCD_FASTDIV2/bxgcd");
	printf("\nTEST: FAST BXGCD 31-BIT RUNTIME WITH COMPILER OPTIMIZATION (-O3)\n");
	status = system("gcc ./BXGCD_FASTDIV2/bxgcd_fast_div2.c -o ./BXGCD_FASTDIV2/bxgcd_fast_div2 -lgmp -O3");
	status = system("./BXGCD_FASTDIV2/bxgcd");
	return 0;
}
