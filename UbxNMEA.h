#ifndef __AUBXLIB_NMEA_H_
#define __AUBXLIB_NMEA_H_

#include "Arduino.h"

/* ************************************************************************* */
struct coord {
	byte deg = 0;
	float fract = 0.0;
};

/* ************************************************************************* */
struct UbxNMEA_GPRMC {
	double utime = 0.0;
	byte status = 0;
	coord lat;
	byte ns_ind = 0;
	coord lon;
	byte ew_ind = 0;
	float spd = 0.0;
	float cog = 0.0;
	byte date[3] = { 0 };
	byte mode = 0;
	byte checksum[2] = { 0 };
};

/* ************************************************************************* */
struct UbxNMEA_GPGGA {
};

/* ************************************************************************* */
struct UbxNMEA_GPGLL {
};

/* ************************************************************************* */
struct UbxNMEA_GPGSA {
};

/* ************************************************************************* */
struct UbxNMEA_GPGSV {
};

/* ************************************************************************* */
struct UbxNMEA_GPVTG {
};

/* ************************************************************************* */
struct UbxNMEA_GPDTM {
	float lat = 0.0;
	byte ns_ind = 0;
	float lon = 0.0;
	byte ew_ind = 0;
	float alt = 0.0;
	byte checksum[2] = { 0 };
};

/* ************************************************************************* */
struct UbxNMEA_GPZDA {

};

#endif