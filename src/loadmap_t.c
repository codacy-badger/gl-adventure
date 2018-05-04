/*
 *  Copyright 2008-2010, 2018 Jerome PASQUIER
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

#include "loadmap_t.h"
#include "loadmap.h"

START_TEST (test_read_map_directory)
{
  
}
END_TEST

Suite * loadmap_suite(void)
{
    Suite* s = suite_create("Loadmap");

    TCase* tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_read_map_directory);
    suite_add_tcase(s, tc_core);

    return s;
}
