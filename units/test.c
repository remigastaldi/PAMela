/**
* @Author: Remi Gastaldi
* @Date:   2017-10-05T12:45:12+02:00
* @Project: Spider
 * @Last modified by:   prost_m
 * @Last modified time: 2017-11-26T22:55:38+01:00
*/

#include <criterion/criterion.h>
#include "unistd.h"

Test(sample, library_exist) {
  cr_assert_neq(-1, access("/lib/x86_64-linux-gnu/security/mypam.so", F_OK), "Library \"/lib/x86_64-linux-gnu/security/mypam.so\" doesn't exist");
}
