// CSS Preprocessor -- Test Suite
// Copyright (c) 2015-2019  Made to Order Software Corp.  All Rights Reserved
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

/** \file
 * \brief Test the nth_child.cpp file.
 *
 * This test runs a battery of tests agains the nth_child.cpp
 * implementation to ensure full coverage.
 */

#include "catch_tests.h"

//#include "csspp/exceptions.h"
//#include "csspp/lexer.h"
#include "csspp/nth_child.h"

#include <sstream>

//#include <string.h>

namespace
{


} // no name namespace


TEST_CASE("Nth child", "[nth-child] [constructors]")
{
    {
        csspp::nth_child a((4LL << 32) + 3);
        REQUIRE(a.get_a() == 3);
        REQUIRE(a.get_b() == 4);
        REQUIRE(a.get_nth() == ((4LL << 32) + 3));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "3n+4");
    }

    {
        csspp::nth_child a(3, 4);
        REQUIRE(a.get_a() == 3);
        REQUIRE(a.get_b() == 4);
        REQUIRE(a.get_nth() == ((4LL << 32) + 3));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "3n+4");
    }

    {
        csspp::nth_child a("3n+4");
        REQUIRE(a.get_a() == 3);
        REQUIRE(a.get_b() == 4);
        REQUIRE(a.get_nth() == ((4LL << 32) + 3));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "3n+4");
    }

    // no error left over
    REQUIRE_ERRORS("");
}

TEST_CASE("Simple nth child", "[nth-child] [basics]")
{
    {
        csspp::nth_child a(-3, 9);
        REQUIRE(a.get_a() == -3);
        REQUIRE(a.get_b() == 9);
        REQUIRE(a.get_nth() == ((9LL << 32) + 0xFFFFFFFD));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "-3n+9");

        // special cases
        a.parse("odd");
        REQUIRE(a.get_a() == 2);
        REQUIRE(a.get_b() == 1);
        REQUIRE(a.get_nth() == ((1LL << 32) + 2));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "odd");

        a.parse("even");
        REQUIRE(a.get_a() == 2);
        REQUIRE(a.get_b() == 0);
        REQUIRE(a.get_nth() == ((0LL << 32) + 2));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "2n");
    }

    {
        csspp::nth_child a(3, 4);
        REQUIRE(a.get_a() == 3);
        REQUIRE(a.get_b() == 4);
        REQUIRE(a.get_nth() == ((4LL << 32) + 3));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "3n+4");

        std::string expected;
        for(int i(-100); i <= 100; ++i)
        {
            for(int j(-100); j <= 100; ++j)
            {
                a.set_a(i);
                a.set_b(j);

                REQUIRE(a.get_a() == i);
                REQUIRE(a.get_b() == j);
                REQUIRE(a.get_nth() == ((static_cast<csspp::integer_t>(j) << 32) + (i & 0xFFFFFFFF)));
                REQUIRE(a.get_error() == "");
                if(i == 2 && j == 1)
                {
                    expected = "odd";
                }
                else if(i == 0)
                {
                    expected = std::to_string(j);
                }
                else if(j == 0)
                {
                    expected = std::to_string(i) + "n";
                }
                else
                {
                    expected = std::to_string(i) + "n" + (j >= 0 ? "+" : "") + std::to_string(j);
                }
                REQUIRE(a.to_string() == expected);

                // try all combos with spaces
                for(int k(0); k < (1 << 6); ++k)
                {
                    a.parse(
                              std::string((k & (1 << 0)) ? " " : "")
                            + (i >= 0 ? (rand() % 5 == 0 ? "+" : "") : "-")
                            + ((k & (1 << 1)) ? " " : "")
                            + std::to_string(abs(i))
                            + ((k & (1 << 2)) ? " " : "")  // this one should be illegal, we may enforce it later
                            + "n"
                            + ((k & (1 << 3)) ? " " : "")
                            + (j >= 0 ? "+" : "-")
                            + ((k & (1 << 4)) ? " " : "")
                            + std::to_string(abs(j))
                            + ((k & (1 << 5)) ? " " : "")
                        );

                    REQUIRE(a.get_a() == i);
                    REQUIRE(a.get_b() == j);
                    REQUIRE(a.get_nth() == ((static_cast<csspp::integer_t>(j) << 32) + (i & 0xFFFFFFFF)));
                    REQUIRE(a.get_error() == "");
                    REQUIRE(a.to_string() == expected);
                }

                if(i == 0)
                {
                    for(int k(0); k < (1 << 3); ++k)
                    {
                        a.parse(
                                  std::string((k & (1 << 0)) ? " " : "")
                                + (j >= 0 ? (rand() % 5 <= 2 ? "+" : "") : "-")
                                + ((k & (1 << 1)) ? " " : "")
                                + std::to_string(abs(j))
                                + ((k & (1 << 2)) ? " " : "")
                            );

                        REQUIRE(a.get_a() == i);
                        REQUIRE(a.get_b() == j);
                        REQUIRE(a.get_nth() == ((static_cast<csspp::integer_t>(j) << 32) + (i & 0xFFFFFFFF)));
                        REQUIRE(a.get_error() == "");
                        REQUIRE(a.to_string() == expected);
                    }
                }
            }
        }
    }

    // no error left over
    REQUIRE_ERRORS("");
}

TEST_CASE("Invalid nth child", "[nth-child] [invalid]")
{
    {
        csspp::nth_child a(5, 3);
        REQUIRE(a.get_a() == 5);
        REQUIRE(a.get_b() == 3);
        REQUIRE(a.get_nth() == ((3LL << 32) + 5));
        REQUIRE(a.get_error() == "");
        REQUIRE(a.to_string() == "5n+3");

        REQUIRE_FALSE(a.parse("random"));
        REQUIRE(a.get_error() == "In an An+B expression, we expect an optional signed followed by a number or 'even' or 'odd'.");

        REQUIRE_FALSE(a.parse("electric"));
        REQUIRE(a.get_error() == "In an An+B expression, we expect an optional signed followed by a number or 'even' or 'odd'.");

        REQUIRE_FALSE(a.parse("even3"));
        REQUIRE(a.get_error() == "'even' cannot be followed by anything else in an An+B expression.");

        REQUIRE_FALSE(a.parse("odor"));
        REQUIRE(a.get_error() == "In an An+B expression, we expect an optional signed followed by a number or 'even' or 'odd'.");

        REQUIRE_FALSE(a.parse("odd+3"));
        REQUIRE(a.get_error() == "'odd' cannot be followed by anything else in an An+B expression.");

        REQUIRE_FALSE(a.parse("++5"));
        REQUIRE(a.get_error() == "In an An+B expression, we expect an optional signed followed by a number or 'even' or 'odd'.");

        REQUIRE_FALSE(a.parse("--5"));
        REQUIRE(a.get_error() == "In an An+B expression, we expect an optional signed followed by a number or 'even' or 'odd'.");

        REQUIRE_FALSE(a.parse("5+3"));
        REQUIRE(a.get_error() == "The first number has to be followed by the 'n' character.");

        REQUIRE_FALSE(a.parse("5n3"));
        REQUIRE(a.get_error() == "A sign (+ or -) is expected between the 'An' and the 'B' parts in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n+odd"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n+even"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n++"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n-+"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n+-"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n--"));
        REQUIRE(a.get_error() == "The value B must be a valid integer in 'An+B'.");

        REQUIRE_FALSE(a.parse("5n-3odd"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");

        REQUIRE_FALSE(a.parse("5n-3even"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");

        REQUIRE_FALSE(a.parse("5n-3+"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");

        REQUIRE_FALSE(a.parse("5n-3-"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");

        REQUIRE_FALSE(a.parse("5n-3 3"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");

        REQUIRE_FALSE(a.parse("5n-3n"));
        REQUIRE(a.get_error() == "An 'An+B' expression cannot be followed by anything else.");
    }

    // no error left over
    REQUIRE_ERRORS("");
}

// Local Variables:
// mode: cpp
// indent-tabs-mode: nil
// c-basic-offset: 4
// tab-width: 4
// End:

// vim: ts=4 sw=4 et
