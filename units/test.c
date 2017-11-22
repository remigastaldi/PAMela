/**
 * @Author: Remi Gastaldi
 * @Date:   2017-10-05T12:45:12+02:00
 * @Project: Spider
 * @Last modified by:   gastal_r
 * @Last modified time: 2017-11-22T19:20:07+01:00
 */


#include <criterion/criterion.h>


Test(base_mult, mult_2_by_5)
{
  cr_assert_str_eq("10", "10");
}

Test(base_mult, mult_20_by_100000)
{
  cr_assert_str_eq("100000", "2000000");
}
