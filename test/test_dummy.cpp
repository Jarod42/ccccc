
#include "UnitTest++.h"

#include "utils.h"

TEST(CheckEqualWithUnsignedLong)
{
	unsigned long something = 2;
	CHECK_EQUAL( something, something );
}

