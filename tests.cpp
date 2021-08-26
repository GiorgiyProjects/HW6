#define BOOST_TEST_MODULE tests
#include "CommandBlock.h"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( TestSuite )

    struct cout_redirect {
        cout_redirect( std::streambuf * new_buffer )
                : old( std::cout.rdbuf( new_buffer ) )
        { }

        ~cout_redirect( ) {
            std::cout.rdbuf( old );
        }

    private:
        std::streambuf * old;
    };

    BOOST_AUTO_TEST_CASE(testBulk1) {

        boost::test_tools::output_test_stream output;
        {
            cout_redirect guard(output.rdbuf());
            int argc = 1;
            int argv0 = 3;

            std::ifstream in("input/1.txt");
            size_t N = (argc > 1) ? argv0 : 3;
            CommandMemoryManager M(N);
            InputCommandParser I;
            CommandBlockConsoleOutputter* O1 = new CommandBlockConsoleOutputter();
            I.Attach(O1);
            CommandBlockFileOutputter* O2 = new CommandBlockFileOutputter();
            I.Attach(O2);
            I.InterpretInputs(in, M);
        }
        BOOST_CHECK(output.is_equal ("bulk: cmd1,cmd2,cmd3\n"
                                        "bulk: cmd4,cmd5\n"
                                        ));
    }

    BOOST_AUTO_TEST_CASE(testBulk2) {

        boost::test_tools::output_test_stream output;
        {
            cout_redirect guard(output.rdbuf());
            int argc = 1;
            int argv0 = 3;

            std::ifstream in("input/2.txt");
            size_t N = (argc > 1) ? argv0 : 3;
            CommandMemoryManager M(N);
            InputCommandParser I;
            CommandBlockConsoleOutputter* O1 = new CommandBlockConsoleOutputter();
            I.Attach(O1);
            CommandBlockFileOutputter* O2 = new CommandBlockFileOutputter();
            I.Attach(O2);
            I.InterpretInputs(in, M);
        }
        BOOST_CHECK(output.is_equal ("bulk: cmd1,cmd2\n"
                                        "bulk: cmd3,cmd4\n"
                                        "bulk: cmd5,cmd6,cmd7,cmd8,cmd9\n"
        ));
    }
BOOST_AUTO_TEST_SUITE_END()