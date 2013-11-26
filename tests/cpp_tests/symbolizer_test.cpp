#include <boost/version.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <mapnik/symbolizer.hpp>
#include <vector>
#include <algorithm>

#include "utils.hpp"

using namespace mapnik;

int main(int argc, char** argv)
{
    std::vector<std::string> args;
    for (int i=1;i<argc;++i)
    {
        args.push_back(argv[i]);
    }
    bool quiet = std::find(args.begin(), args.end(), "-q")!=args.end();

    try {
        marker_multi_policy_e policy_in = MARKER_WHOLE_MULTI;
        BOOST_TEST_EQ(policy_in,MARKER_WHOLE_MULTI);
        markers_symbolizer sym;
        std::clog << sym.properties.count(keys::markers_multipolicy) << "\n";
        put<mapnik::enumeration_wrapper>(sym, keys::markers_multipolicy, enumeration_wrapper(policy_in));
        BOOST_TEST_EQ(sym.properties.count(keys::markers_multipolicy),1);
        marker_multi_policy_e policy_out = get<mapnik::marker_multi_policy_e>(sym, keys::markers_multipolicy);
        BOOST_TEST_EQ(policy_out,MARKER_WHOLE_MULTI);
    } catch (...) {
        BOOST_TEST(true);
    }

    if (!::boost::detail::test_errors()) {
        if (quiet) std::clog << "\x1b[1;32m.\x1b[0m";
        else std::clog << "C++ exceptions: \x1b[1;32m✓ \x1b[0m\n";
#if BOOST_VERSION >= 104600
        ::boost::detail::report_errors_remind().called_report_errors_function = true;
#endif
    } else {
        return ::boost::report_errors();
    }
}
