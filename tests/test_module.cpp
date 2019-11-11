#define BOOST_TEST_MODULE app_tests

#include <boost/test/unit_test.hpp>
#include "../version.h"


BOOST_AUTO_TEST_SUITE(app_tests)

BOOST_AUTO_TEST_CASE(test_version)
{
    BOOST_CHECK(PROJECT_VERSION_PATCH > 0);
}


BOOST_AUTO_TEST_SUITE_END()
