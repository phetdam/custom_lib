/*
 * stats.h
 * contains macros and function declarations for stats.c
 *
 * Changelog:
 *
 * 08-30-2018
 * added changelog and function declarations
 *
 * 08-28-2018
 * initial completion
 */

#ifndef STATS_H
#define STATS_H
// define standard normal mu and s
#define STD_MU 0
#define STD_S 1
// define constants for normalcdf function
/*
#define A_1 0.0705230784
#define A_2 0.0422820123
#define A_3 0.0092705272
#define A_4 0.0001520143
#define A_5 0.0002765672
#define A_6 0.0000430638
*/
#define A_1 0.0498673470
#define A_2 0.0211410061
#define A_3 0.0032776263
#define A_4 0.0000380036
#define A_5 0.0000488906
#define A_6 0.0000053830
// declare normalcdf and normalpdf
double normalcdf(double x, double mu, double s);
double normalcdf1(double, double, double);
double normalpdf(double x, double mu, double s);
// declarations for random number generators

#endif /* STATS_H */
