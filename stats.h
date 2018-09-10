/*
 * stats.h
 * contains macros and function declarations for stats.c
 *
 * Changelog:
 *
 * 09-10-2018
 * removed old set of normalcdf() constant #defines and normalcdf1() declaration
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
#define A_1 0.0498673470
#define A_2 0.0211410061
#define A_3 0.0032776263
#define A_4 0.0000380036
#define A_5 0.0000488906
#define A_6 0.0000053830
// declare normalcdf and normalpdf
double normalcdf(double x, double mu, double s);
double normalpdf(double x, double mu, double s);
// declarations for random number generators

#endif /* STATS_H */
