/*
** GPSGeoid.h
** 
** Made by Bernhard Reitinger
** Login   <breiting@localhost.localdomain>
** 
** Started on  Wed Jul  5 22:48:46 2006 Bernhard Reitinger
** Last update Wed Jul  5 22:48:46 2006 Bernhard Reitinger
*/

/**
 * @file GPSGeoid.hpp
 * The GPS geoid defined in ICD-GPS-200
 */

#ifndef   	GPSGEOID_H_
# define   	GPSGEOID_H_

#include "WGS84Geoid.hpp"

/**
 * This class represents the geodetic model defined in
 * ICD-GPS-200.  This geodetic model was based on the WGS 84
 * model, which has changed since the ICD-GPS-200 was written.
 * To save the cost of having to update all of the fielded
 * receivers, these constants were kept for GPS purposes.
 *
 * Only some of the WGS 84 geodetic model parameters are defined
 * as part of the 200.  These parameters are explicitly defined
 * here.  Other parameters are inherited from the WGS84Geoid
 * definition.
 */
class GPSGeoid : public WGS84Geoid
{
public:
    /// defined in ICD-GPS-200C, 20.3.3.4.3.3 and Table 20-IV
    /// @return angular velocity of Earth in radians/sec.
    virtual double angVelocity() const throw()
    { return 7.2921151467e-5; }

    /// defined in ICD-GPS-200C, 20.3.3.4.3.3 and Table 20-IV
    /// @return geocentric gravitational constant in m**3 / s**2
    virtual double gm() const throw()
    { return 3.986005e14; }

    /// derived from ICD-GPS-200C, 20.3.3.4.3.3 and Table 20-IV
    /// @return geocentric gravitational constant in m**3 / s**2
    virtual double gm_km() const throw()
    { return 3.9860034e5; }

    /// defined in ICD-GPS-200C, 20.3.4.3
    /// @return Speed of light in m/s.
    virtual double c() const throw()
    { return 2.99792458e8; }

    /// derived from ICD-GPS-200C, 20.3.4.3
    /// @return Speed of light in km/s
    virtual double c_km() const throw()
    { return 2.99792458e5; }

}; // class GPSGeoid

#endif 	    /* !GPSGEOID_H_ */
