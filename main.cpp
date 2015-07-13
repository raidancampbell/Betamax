#include <iostream>
#include <time.h>
#include <cmath>
#include <math.h>


#define LONGITUDE 81.68
#define LATITUDE 41.48
#define PI 3.14159265
#define TO_RAD * (PI/180.0)
#define TO_DEG * (180/PI)
using namespace std;

double get_julian_time();
double get_julian_date_cycle(double julian_time);
double get_approximate_solar_noon(double julian_date_cycle);
double get_solar_mean_anomaly(double approximate_solar_noon);
double get_equation_of_center(double solar_mean_anomaly);
double get_ecliptic_longitude(double solar_mean_anomaly, double equation_of_center);
double get_solar_transit(double approximate_solar_noon, double solar_mean_anomaly, double ecliptic_longitude);
double get_declination_of_sun(double ecliptic_longitude);
double get_hour_angle(double declination_of_sun);
double get_sunrise(double solar_transit, double hour_angle);
double get_sunset(double solar_transit, double hour_angle);

int main() {
    double julian_time = get_julian_time();
    double julian_date_cycle = get_julian_date_cycle(julian_time);
    double approximate_solar_noon = get_approximate_solar_noon(julian_date_cycle);
    double solar_mean_anomaly = get_solar_mean_anomaly(approximate_solar_noon);
    double equation_of_center = get_equation_of_center(solar_mean_anomaly);
    double ecliptic_longitude = get_ecliptic_longitude(solar_mean_anomaly, equation_of_center);
    double solar_transit = get_solar_transit(approximate_solar_noon, solar_mean_anomaly, ecliptic_longitude);
    double declination_of_sun = get_declination_of_sun(ecliptic_longitude);
    double hour_angle = get_hour_angle(declination_of_sun);
    double sunrise = get_sunrise(solar_transit, hour_angle);
    double sunset = get_sunset(solar_transit, hour_angle);

    printf("SUNRISE: %.3f \r\n", sunrise);
    printf("SUNSET: %.3f \r\n", sunset);
    return EXIT_SUCCESS;
}

double get_julian_time() {
    int unixSecs = (int) time(NULL);
    return ( unixSecs / 86400.0 ) + 2440587.0;/*TODO: add 0.5 to this number?*/
}

double get_julian_date_cycle(double julian_time){
    double approximation = julian_time - 2451545.009 - LONGITUDE / 360.0;
    return floor(approximation + 0.5);
}

double get_approximate_solar_noon(double julian_cycle){
    return 2451545.0009 + (LONGITUDE / 360.0) + julian_cycle;

}

double get_solar_mean_anomaly(double approximate_solar_noon){
    /*whee modular double precision arithmetic, for all your brain-melting needs!*/
    return fmod(357.5291 + 0.98560028*(approximate_solar_noon-2451545.009), 360.0);
}

double get_equation_of_center(double solar_mean_anomaly){
    return 1.9148*sin(solar_mean_anomaly TO_RAD)
           + 0.0200*sin(2.0*solar_mean_anomaly TO_RAD)
           + 0.0003*sin(3.0*solar_mean_anomaly TO_RAD);
}

double get_ecliptic_longitude(double solar_mean_anomaly, double equation_of_center){
    return fmod(solar_mean_anomaly+102.9372+equation_of_center+180.0,360.0);
}

double get_solar_transit(double approximate_solar_noon, double solar_mean_anomaly, double ecliptic_longitude){
    return approximate_solar_noon + 0.0053*sin(solar_mean_anomaly TO_RAD) - 0.0069*sin(2.0*ecliptic_longitude TO_RAD);
}

double get_declination_of_sun(double ecliptic_longitude){
    return asin(sin(ecliptic_longitude TO_RAD)*sin(23.45 TO_RAD)) TO_DEG;/*TO_RAD again?*/
}

double get_hour_angle(double declination_of_sun){
    return acos((sin(-0.83 TO_RAD) - sin(LATITUDE TO_RAD)*sin(declination_of_sun TO_RAD)) /(cos(LATITUDE TO_RAD)*cos(declination_of_sun TO_RAD))) TO_DEG;
}

double get_sunrise(double solar_transit, double hour_angle){
    return solar_transit - hour_angle / 360.0;
}

double get_sunset(double solar_transit, double hour_angle){
    return solar_transit + hour_angle / 360.0;
}