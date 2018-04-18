/*
 *  Copyright 2008-2010, 2018 Jérôme PASQUIER
 * 
 *  This file is part of glAdventure - A little game using XGL.
 *
 *  glAdventure is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  glAdventure is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with glAdventure; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 *  02110-1301  USA
 *
 */

#include <check.h>

#include "colors.h"

/** Test the to_x macro
 *
 */
START_TEST (test_to_x)
{
  int v = TO_X(255);
  ck_assert_int_eq(v, 65535);
}
END_TEST

Suite * colors_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Colors");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_to_x);
    suite_add_tcase(s, tc_core);

    return s;
}
