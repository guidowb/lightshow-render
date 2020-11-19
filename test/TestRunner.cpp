#include <cppunit/ui/text/TestRunner.h>

#include "ParserTests.cpp"

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  runner.addTest( ParserTests::suite() );
  runner.run();
  return 0;
}
