/*
** GeoidModel.h
** 
** Made by Bernhard Reitinger
** Login   <breiting@localhost.localdomain>
** 
** Started on  Wed Jul  5 22:45:22 2006 Bernhard Reitinger
** Last update Wed Jul  5 22:45:22 2006 Bernhard Reitinger
*/

/**
 * @file GeoidModel.h
 * Abstract base class modeling a geoid (used for transforming GPS data)
 */

/**
 * This abstract class encapsulates geodetic models (e.g. WGS84,
 * GPS, etc).
 */

#ifndef   	GEOIDMODEL_H_
# define   	GEOIDMODEL_H_

class GeoidModel
{
public:

    // default ctor
    virtual ~GeoidModel() {}

    /// @return semi-major axis of Earth in meters.
    virtual double a() const throw() = 0;
    
    /// @return semi-major axis of Earth in km.
    virtual double a_km() const throw() = 0;
    
    /// @return flattening (ellipsoid parameter).
    virtual double flattening() const throw() = 0;
    
    /// @return eccentricity (ellipsoid parameter).
    virtual double eccentricity() const throw() = 0;
    
    /// @return eccentricity squared (ellipsoid parameter).
    virtual double eccSquared() const throw()
    { return eccentricity() * eccentricity(); }
    
    /// @return angular velocity of Earth in radians/sec.
    virtual double angVelocity() const throw() = 0;
    
    /// @return geocentric gravitational constant in m**3 / s**2
    virtual double gm() const throw() = 0;
    
    /// @return geocentric gravitational constant in m**3 / s**2
    virtual double gm_km() const throw() = 0;
    
    /// @return Speed of light in m/s.
    virtual double c() const throw() = 0;
    
    /// @return Speed of light in km/s
    virtual double c_km() const throw() = 0;
    
}; // class GeoidModel

#endif 	    /* !GEOIDMODEL_H_ */
