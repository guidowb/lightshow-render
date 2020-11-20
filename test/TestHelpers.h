#ifndef _TESTHELPERS_H_
#define _TESTHELPERS_H_

#define CPPUNIT_ASSERT_STRINGS_EQUAL(expected, actual) CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(actual))

#endif // _TESTHELPERS_H_