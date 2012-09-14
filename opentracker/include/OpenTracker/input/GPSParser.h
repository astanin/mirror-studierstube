/* ========================================================================
 * Copyright (c) 2006,
 * Institute for Computer Graphics and Vision
 * Graz University of Technology
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Graz University of Technology nor the names of
 * its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ========================================================================
 * PROJECT: OpenTracker
 * ======================================================================== */
/** header file for GPSParser class and helper data types
 *
 * @author Gerhard Reitmayr
 * 
 * $Id: GPSParser.h 1621 2006-11-13 12:48:35Z schall $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifndef _GPSPARSER_H_
#define _GPSPARSER_H_

#include <cmath>

#include "../OpenTracker.h"

/**
 * The abstract base class for representing results of the GPSParser class.
 * The type field tells the user about the actual data represented by the
 * instance. Don't mess with the type field :). Instances of this base class
 * with type = INVALID will be used to denote errors during parsing.
 *
 * @ingroup input
 * @author Gerhard Reitmayr
 */



/***************************************************************************/
/*
 *                              DEFINES
 */

  #define UTM_NO_ERROR            0x0000
  #define UTM_LAT_ERROR           0x0001
  #define UTM_LON_ERROR           0x0002
  #define UTM_EASTING_ERROR       0x0004
  #define UTM_NORTHING_ERROR      0x0008
  #define UTM_ZONE_ERROR          0x0010
  #define UTM_HEMISPHERE_ERROR    0x0020
  #define UTM_ZONE_OVERRIDE_ERROR 0x0040
  #define UTM_A_ERROR             0x0080
  #define UTM_INV_F_ERROR         0x0100



/***************************************************************************/
/*                               DEFINES 
 *
 */

#define TRANMERC_NO_ERROR           0x0000
#define TRANMERC_LAT_ERROR          0x0001
#define TRANMERC_LON_ERROR          0x0002
#define TRANMERC_EASTING_ERROR      0x0004
#define TRANMERC_NORTHING_ERROR     0x0008
#define TRANMERC_ORIGIN_LAT_ERROR   0x0010
#define TRANMERC_CENT_MER_ERROR     0x0020
#define TRANMERC_A_ERROR            0x0040
#define TRANMERC_INV_F_ERROR        0x0080
#define TRANMERC_SCALE_FACTOR_ERROR 0x0100
#define TRANMERC_LON_WARNING        0x0200


#define PI              3.14159265358979323e0 /* PI     */
#define PI_OVER         (PI/2.0e0)            /* PI over 2 */
#define MAX_LAT         ((PI * 89.99)/180.0)    /* 90 degrees in radians */
#define MAX_DELTA_LONG  ((PI * 90)/180.0)    /* 90 degrees in radians */
#define MIN_SCALE_FACTOR  0.3
#define MAX_SCALE_FACTOR  3.0


#define MIN_LAT      ( (-80.5 * PI) / 180.0 ) /* -80.5 degrees in radians    */
#define MIN_EASTING  100000
#define MAX_EASTING  900000
#define MIN_NORTHING 0
#define MAX_NORTHING 10000000



#define SPHTMD(lat) ((double) (TranMerc_ap * lat \
      - TranMerc_bp * sin(2.e0 * lat) + TranMerc_cp * sin(4.e0 * lat) \
      - TranMerc_dp * sin(6.e0 * lat) + TranMerc_ep * sin(8.e0 * lat) ) )

#define SPHSN(lat) ((double) (TranMerc_a / sqrt( 1.e0 - TranMerc_es * \
      pow(sin(lat), 2))))

#define SPHSR(lat) ((double) (TranMerc_a * (1.e0 - TranMerc_es) / \
    pow(DENOM(lat), 3)))

#define DENOM(lat) ((double) (sqrt(1.e0 - TranMerc_es * pow(sin(lat),2))))


/**************************************************************************/
/*                               GLOBAL DECLARATIONS
 *
 */


static double UTM_a = 6378137.0;         /* Semi-major axis of ellipsoid in meters  */
static double UTM_f = 1 / 298.257223563; /* Flattening of ellipsoid                 */
static long   UTM_Override = 0;          /* Zone override flag                      */

/* Ellipsoid Parameters, default to WGS 84  */
static double TranMerc_a = 6378137.0;              /* Semi-major axis of ellipsoid i meters */
static double TranMerc_f = 1 / 298.257223563;      /* Flattening of ellipsoid  */
static double TranMerc_es = 0.0066943799901413800; /* Eccentricity (0.08181919084262188000) squared */
static double TranMerc_ebs = 0.0067394967565869;   /* Second Eccentricity squared */

/* Transverse_Mercator projection Parameters */
static double TranMerc_Origin_Lat = 0.0;           /* Latitude of origin in radians */
static double TranMerc_Origin_Long = 0.285070444;  /* Longitude of origin in radians */
												   /* 16 Grad 20 Minuten == 16,333333 Grad */	
static double TranMerc_False_Northing = -5000000.0;/* False northing in meters */
static double TranMerc_False_Easting = 0.0;        /* False easting in meters */
static double TranMerc_Scale_Factor = 1.0;         /* Scale factor  */



/* Isometeric to geodetic latitude parameters, default to WGS 84 */
static double TranMerc_ap = 6367449.1458008;
static double TranMerc_bp = 16038.508696861;
static double TranMerc_cp = 16.832613334334;
static double TranMerc_dp = 0.021984404273757;
static double TranMerc_ep = 3.1148371319283e-005;

/* Maximum variance for easting and northing values for WGS 84. */
static double TranMerc_Delta_Easting = 40000000.0;
static double TranMerc_Delta_Northing = 40000000.0;

/* These state variables are for optimization purposes. The only function
 * that should modify them is Set_Tranverse_Mercator_Parameters.         */



#ifndef OT_NO_GPS_SUPPORT

// defines the model used for ECEF transformation

#include "../misc/GeoidModel.h"

namespace ot {

    class GPResult {
    public :
        enum Type {
            GPGGA,
            GPVTG, 
            HCHDG, 
            PGRMZ,
            INVALID
        } type;
       
        GPResult() {
            type = GPResult::INVALID;
        }
    };

    class GPGGA : public GPResult {
    public :
        double  time;
        double  lat;
        double  lon;
        int     fix;
        int     numsats;
        double  hdop;
        double  altitude;    
        double  height;
        double  diffdelay;
        int     statid;

        // add additional XYZ coordinate (ECEF format)
        // see
        // http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif
        double xECEF;
        double yECEF;
        double zECEF;
        
        // add additional coordinates, that hold the Gauss-Krüger Projection for Styria (M34)
	// or in BMN (Bundesmeldeamt) format or in UTM format
	// these values are delivered after a transversal Mercator transformation has been 
	// applied to lat and lon
	double lat_gk_m34;
	double lon_gk_m34;

	double lat_utm_33n;
	double lon_utm_33n;
	long zone_utm_33n;
	char hem_utm_33n;

	// coordinates in Bundesmeldeamt format
	double lat_bmn_34;	
	double lon_bmn_34;


        static const GPResult * parse( const char * );

        /** Convert to Earth-centered, Earth-fixed XYZ coordinates. */
        // see
        // http://www.colorado.edu/geography/gcraft/notes/datum/gif/llhxyz.gif

        void convert2ECEF(GeoidModel *geoid)
        {
            double rad_cur, gdlat, gdlon;
            double gdalt = altitude;
            
            // convert angles to radians
            gdlat = MathUtils::GradToRad * lat;
            gdlon = MathUtils::GradToRad * lon;
            
            // radius of curvature in the prime vertical
            rad_cur  = geoid->a() /
                sqrt(1.0-geoid->eccSquared()*pow((sin(gdlat)),2.0));
            
                        //printf("lat = %2.24f\n", lat);
             //printf("lon = %2.24f\n", lon);
            
            xECEF = static_cast<float>((rad_cur + gdalt) * ::cos(gdlat) * ::cos(gdlon));
            yECEF = static_cast<float>((rad_cur + gdalt) * ::cos(gdlat) * ::sin(gdlon));
            zECEF = static_cast<float>(((1.0 - geoid->eccSquared()) * rad_cur + gdalt) * ::sin(gdlat));
              //printf("xECEF = %08.2f\n", xECEF);
              //printf("yECEF = %f08.2\n", yECEF);
                          //printf("zECEF = %f08.2\n", zECEF);
			  //getchar();
            
        }


		/*long Set_Transverse_Mercator_Parameters(double a,double f, double Origin_Latitude, double Central_Meridian,double False_Easting, double False_Northing, double Scale_Factor){};

		void Convert_Geodetic_To_Transverse_Mercator (double gdlat,double gdlon,double *Easting,double *Northing){};
	
		long Set_UTM_Parameters(double a,   double f, long oride){};

		long Convert_Geodetic_To_UTM (double Latitude, double Longitude,long *Zone, char *Hemisphere,double *Easting,double *Northing){};
		*/

		
 long Set_Transverse_Mercator_Parameters(double a,
                                        double f,
                                        double Origin_Latitude,
                                        double Central_Meridian,
                                        double False_Easting,
                                        double False_Northing,
                                        double Scale_Factor)

{ 
	
  /* BEGIN Set_Tranverse_Mercator_Parameters */
  /*
   * The function Set_Tranverse_Mercator_Parameters receives the ellipsoid
   * parameters and Tranverse Mercator projection parameters as inputs, and
   * sets the corresponding state variables. If any errors occur, the error
   * code(s) are returned by the function, otherwise TRANMERC_NO_ERROR is
   * returned.
   *
   *    a                 : Semi-major axis of ellipsoid, in meters    (input)
   *    f                 : Flattening of ellipsoid					   (input)
   *    Origin_Latitude   : Latitude in radians at the origin of the   (input)
   *                         projection
   *    Central_Meridian  : Longitude in radians at the center of the  (input)
   *                         projection
   *    False_Easting     : Easting/X at the center of the projection  (input)
   *    False_Northing    : Northing/Y at the center of the projection (input)
   *    Scale_Factor      : Projection scale factor                    (input) 
   */

  double tn;        /* True Meridianal distance constant  */
  double tn2;
  double tn3;
  double tn4;
  double tn5;
  double dummy_northing;
  double TranMerc_b; /* Semi-minor axis of ellipsoid, in meters */
  double inv_f = 1 / f;
  long Error_Code = TRANMERC_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= TRANMERC_A_ERROR;
  }
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= TRANMERC_INV_F_ERROR;
  }
  if ((Origin_Latitude < -MAX_LAT) || (Origin_Latitude > MAX_LAT))
  { /* origin latitude out of range */
    Error_Code |= TRANMERC_ORIGIN_LAT_ERROR;
  }
  if ((Central_Meridian < -PI) || (Central_Meridian > (2*PI)))
  { /* origin longitude out of range */
    Error_Code |= TRANMERC_CENT_MER_ERROR;
  }
  if ((Scale_Factor < MIN_SCALE_FACTOR) || (Scale_Factor > MAX_SCALE_FACTOR))
  {
    Error_Code |= TRANMERC_SCALE_FACTOR_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    TranMerc_a = a;
    TranMerc_f = f;
    TranMerc_Origin_Lat = 0;
    TranMerc_Origin_Long = 0;
    TranMerc_False_Northing = 0;
    TranMerc_False_Easting = 0; 
    TranMerc_Scale_Factor = 1;

    /* Eccentricity Squared */
    TranMerc_es = 2 * TranMerc_f - TranMerc_f * TranMerc_f;
    /* Second Eccentricity Squared */
    TranMerc_ebs = (1 / (1 - TranMerc_es)) - 1;

    TranMerc_b = TranMerc_a * (1 - TranMerc_f);    
    /*True meridianal constants  */
    tn = (TranMerc_a - TranMerc_b) / (TranMerc_a + TranMerc_b);
    tn2 = tn * tn;
    tn3 = tn2 * tn;
    tn4 = tn3 * tn;
    tn5 = tn4 * tn;

    TranMerc_ap = TranMerc_a * (1.e0 - tn + 5.e0 * (tn2 - tn3)/4.e0
                                + 81.e0 * (tn4 - tn5)/64.e0 );
    TranMerc_bp = 3.e0 * TranMerc_a * (tn - tn2 + 7.e0 * (tn3 - tn4)
                                       /8.e0 + 55.e0 * tn5/64.e0 )/2.e0;
    TranMerc_cp = 15.e0 * TranMerc_a * (tn2 - tn3 + 3.e0 * (tn4 - tn5 )/4.e0) /16.0;
    TranMerc_dp = 35.e0 * TranMerc_a * (tn3 - tn4 + 11.e0 * tn5 / 16.e0) / 48.e0;
    TranMerc_ep = 315.e0 * TranMerc_a * (tn4 - tn5) / 512.e0;
	Convert_Geodetic_To_Transverse_Mercator(MAX_LAT,
                                            MAX_DELTA_LONG,
                                            &TranMerc_Delta_Easting,
                                            &TranMerc_Delta_Northing);
	Convert_Geodetic_To_Transverse_Mercator(0,
                                            MAX_DELTA_LONG,
                                            &TranMerc_Delta_Easting,
                                            &dummy_northing);
    TranMerc_Origin_Lat = Origin_Latitude;
    if (Central_Meridian > PI)
      Central_Meridian -= (2*PI);
    TranMerc_Origin_Long = Central_Meridian;
    TranMerc_False_Northing = False_Northing;
    TranMerc_False_Easting = False_Easting; 
    TranMerc_Scale_Factor = Scale_Factor;
  } /* END OF if(!Error_Code) */
  return (Error_Code);
}  /* END of Set_Transverse_Mercator_Parameters  */



	   

 void Convert_Geodetic_To_Transverse_Mercator (double gdlat,
                                              double gdlon,
                                              double *Easting,
                                              double *Northing)
{											  
	   /* BEGIN Convert_Geodetic_To_Transverse_Mercator 
	   see: http://earth-info.nga.mil/GandG/geotrans/
  
	   * The function Convert_Geodetic_To_Transverse_Mercator converts geodetic
	   * (latitude and longitude) coordinates to Transverse Mercator projection
	   * (easting and northing) coordinates, according to the current ellipsoid
	   * and Transverse Mercator projection coordinates. 
	   *
	   *    Latitude      : lat in radians                         (input)
       *    Longitude     : lon in radians                         (input)
       *    Easting       : lon_gk_m34 Easting/X in meters                         (output)
       *    Northing      : lat_gk_m34 Northing/Y in meters                        (output)
		
		A particular ellipsoid is specified in terms of the following parameters:
		Semi-Major Axis (a):	Radius (in meters) at the equator, and
		Flattening (f):	Ratio of the difference between the semi-major axis 
		and polar radius of the Earth to its semi-major axis.
		   
		A particular variation of the Transverse Mercator projection is 
		specified in terms of the following parameters:
			
		Central Meridian – Longitude (in radians) at the origin of the projection,
		Origin Latitude – Latitude (in radians) at the origin of the projection,
		False Easting – A coordinate value (in meters) assigned to the 
						central meridian of the projection to avoid the 
						inconvenience of using negative coordinates, and
		False Northing – A coordinate value (in meters) assigned to the 
					  	 origin latitude of the projection to avoid the 
						 inconvenience of using negative coordinates.
		Scale Factor – a multiplier for reducing a distance in projected 
					   coordinates to the actual distance along the central meridian.
   
   */
   
  double c;       /* Cosine of latitude                          */
  double c2;
  double c3;
  double c5;
  double c7;
  double dlam;    /* Delta longitude - Difference in Longitude       */
  double eta;     /* constant - TranMerc_ebs *c *c                   */
  double eta2;
  double eta3;
  double eta4;
  double s;       /* Sine of latitude                        */
  double sn;      /* Radius of curvature in the prime vertical       */
  double t;       /* Tangent of latitude                             */
  double tan2;
  double tan3;
  double tan4;
  double tan5;
  double tan6;
  double t1;      /* Term in coordinate conversion formula - GP to Y */
  double t2;      /* Term in coordinate conversion formula - GP to Y */
  double t3;      /* Term in coordinate conversion formula - GP to Y */
  double t4;      /* Term in coordinate conversion formula - GP to Y */
  double t5;      /* Term in coordinate conversion formula - GP to Y */
  double t6;      /* Term in coordinate conversion formula - GP to Y */
  double t7;      /* Term in coordinate conversion formula - GP to Y */
  double t8;      /* Term in coordinate conversion formula - GP to Y */
  double t9;      /* Term in coordinate conversion formula - GP to Y */
  double tmd;     /* True Meridional distance                        */
  double tmdo;    /* True Meridional distance for latitude of origin */
  long    Error_Code = TRANMERC_NO_ERROR;
  double temp_Origin;
  double temp_Long;

  //double gdlat, gdlon;

  // convert angles to radians
  //gdlat = MathUtils::GradToRad * lat;
  //gdlon = MathUtils::GradToRad * lon;

  if ((gdlat < -MAX_LAT) || (gdlat > MAX_LAT))
  {  /* Latitude out of range */
    Error_Code|= TRANMERC_LAT_ERROR;
  }
  if (gdlon > PI)
    gdlon -= (2 * PI);
  if ((gdlon < (TranMerc_Origin_Long - MAX_DELTA_LONG))
      || (gdlon > (TranMerc_Origin_Long + MAX_DELTA_LONG)))
  {
    if (gdlon < 0)
      temp_Long = gdlon + 2 * PI;
    else
      temp_Long = gdlon;
    if (TranMerc_Origin_Long < 0)
      temp_Origin = TranMerc_Origin_Long + 2 * PI;
    else
      temp_Origin = TranMerc_Origin_Long;
    if ((temp_Long < (temp_Origin - MAX_DELTA_LONG))
        || (temp_Long > (temp_Origin + MAX_DELTA_LONG)))
      Error_Code|= TRANMERC_LON_ERROR;
  }
  if (!Error_Code)
  { /* no errors */

    /* 
     *  Delta Longitude
     */
    dlam = gdlon - TranMerc_Origin_Long;

    if (fabs(dlam) > (9.0 * PI / 180))
    { /* Distortion will result if lon is more than 9 degrees from the Central Meridian */
      Error_Code |= TRANMERC_LON_WARNING;
    }

    if (dlam > PI)
      dlam -= (2 * PI);
    if (dlam < -PI)
      dlam += (2 * PI);
    if (fabs(dlam) < 2.e-10)
      dlam = 0.0;

    s = sin(gdlat);
    c = cos(gdlat);
    c2 = c * c;
    c3 = c2 * c;
    c5 = c3 * c2;
    c7 = c5 * c2;
    t = tan (gdlat);
    tan2 = t * t;
    tan3 = tan2 * t;
    tan4 = tan3 * t;
    tan5 = tan4 * t;
    tan6 = tan5 * t;
    eta = TranMerc_ebs * c2;
    eta2 = eta * eta;
    eta3 = eta2 * eta;
    eta4 = eta3 * eta;

    /* radius of curvature in prime vertical */
    sn = SPHSN(gdlat);

    /* True Meridianal Distances */
    tmd = SPHTMD(gdlat);

    /*  Origin  */
    tmdo = SPHTMD (TranMerc_Origin_Lat);

    /* northing */
    t1 = (tmd - tmdo) * TranMerc_Scale_Factor;
    t2 = sn * s * c * TranMerc_Scale_Factor/ 2.e0;
    t3 = sn * s * c3 * TranMerc_Scale_Factor * (5.e0 - tan2 + 9.e0 * eta 
                                                + 4.e0 * eta2) /24.e0; 

    t4 = sn * s * c5 * TranMerc_Scale_Factor * (61.e0 - 58.e0 * tan2
                                                + tan4 + 270.e0 * eta - 330.e0 * tan2 * eta + 445.e0 * eta2
                                                + 324.e0 * eta3 -680.e0 * tan2 * eta2 + 88.e0 * eta4 
                                                -600.e0 * tan2 * eta3 - 192.e0 * tan2 * eta4) / 720.e0;

    t5 = sn * s * c7 * TranMerc_Scale_Factor * (1385.e0 - 3111.e0 * 
                                                tan2 + 543.e0 * tan4 - tan6) / 40320.e0;

    *Northing = TranMerc_False_Northing + t1 + pow(dlam,2.e0) * t2
                + pow(dlam,4.e0) * t3 + pow(dlam,6.e0) * t4
                + pow(dlam,8.e0) * t5; 

    /* Easting */
    t6 = sn * c * TranMerc_Scale_Factor;
    t7 = sn * c3 * TranMerc_Scale_Factor * (1.e0 - tan2 + eta ) /6.e0;
    t8 = sn * c5 * TranMerc_Scale_Factor * (5.e0 - 18.e0 * tan2 + tan4
                                            + 14.e0 * eta - 58.e0 * tan2 * eta + 13.e0 * eta2 + 4.e0 * eta3 
                                            - 64.e0 * tan2 * eta2 - 24.e0 * tan2 * eta3 )/ 120.e0;
    t9 = sn * c7 * TranMerc_Scale_Factor * ( 61.e0 - 479.e0 * tan2
                                             + 179.e0 * tan4 - tan6 ) /5040.e0;

    *Easting = TranMerc_False_Easting + dlam * t6 + pow(dlam,3.e0) * t7 
               + pow(dlam,5.e0) * t8 + pow(dlam,7.e0) * t9;


	
	 /*printf("lat = %2.24f\n", gdlat);
     printf("lon = %2.24f\n", gdlon);    
     printf("lon_gk_m34 = %08.2f\n", lon_gk_m34);
     printf("lat_gk_m34 = %f08.2\n", lat_gk_m34);
	 getchar();
	 */
	
  }
  
} /* END OF Convert_Geodetic_To_Transverse_Mercator */

		


 long Set_UTM_Parameters(double a,      
                        double f,
                        long oride)
{
/*
 * The function Set_UTM_Parameters receives the ellipsoid parameters and
 * UTM zone override parameter as inputs, and sets the corresponding state
 * variables.  If any errors occur, the error code(s) are returned by the 
 * function, otherwise UTM_NO_ERROR is returned.
 *
 *    a                 : Semi-major axis of ellipsoid, in meters       (input)
 *    f                 : Flattening of ellipsoid						            (input)
 *    oride          : UTM override zone, zero indicates no override (input)
 */

  double inv_f = 1 / f;
  long Error_Code = UTM_NO_ERROR;

  if (a <= 0.0)
  { /* Semi-major axis must be greater than zero */
    Error_Code |= UTM_A_ERROR;
  }
  if ((inv_f < 250) || (inv_f > 350))
  { /* Inverse flattening must be between 250 and 350 */
    Error_Code |= UTM_INV_F_ERROR;
  }
  if ((oride < 0) || (oride > 60))
  {
    Error_Code |= UTM_ZONE_OVERRIDE_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    UTM_a = a;
    UTM_f = f;
    UTM_Override = oride;
  }
  return (Error_Code);
} /* END OF Set_UTM_Parameters */




 long Convert_Geodetic_To_UTM (double Latitude,
                              double Longitude,
                              long   *Zone,
                              char   *Hemisphere,
                              double *Easting,
                              double *Northing)
{ 
/*
 * The function Convert_Geodetic_To_UTM converts geodetic (latitude and
 * longitude) coordinates to UTM projection (zone, hemisphere, easting and
 * northing) coordinates according to the current ellipsoid and UTM zone
 * override parameters.  If any errors occur, the error code(s) are returned
 * by the function, otherwise UTM_NO_ERROR is returned.
 *
 *    Latitude          : Latitude in radians                 (input)
 *    Longitude         : Longitude in radians                (input)
 *    Zone              : UTM zone                            (output)
 *    Hemisphere        : North or South hemisphere           (output)
 *    Easting           : Easting (X) in meters               (output)
 *    Northing          : Northing (Y) in meters              (output)
 */

  long Lat_Degrees;
  long Long_Degrees;
  long temp_zone;
  long Error_Code = UTM_NO_ERROR;
  double Origin_Latitude = 0;
  double Central_Meridian = 0;
  double False_Easting = 500000;
  double False_Northing = 0;
  double Scale = 0.9996;

  if ((Latitude < MIN_LAT) || (Latitude > MAX_LAT))
  { /* Latitude out of range */
    Error_Code |= UTM_LAT_ERROR;
  }
  if ((Longitude < -PI) || (Longitude > (2*PI)))
  { /* Longitude out of range */
    Error_Code |= UTM_LON_ERROR;
  }
  if (!Error_Code)
  { /* no errors */
    if (Longitude < 0)
      Longitude += (2*PI) + 1.0e-10;
    Lat_Degrees = (long)(Latitude * 180.0 / PI);
    Long_Degrees = (long)(Longitude * 180.0 / PI);

    if (Longitude < PI)
      temp_zone = (long)(31 + ((Longitude * 180.0 / PI) / 6.0));
    else
      temp_zone = (long)(((Longitude * 180.0 / PI) / 6.0) - 29);
    if (temp_zone > 60)
      temp_zone = 1;
    /* UTM special cases */
    if ((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > -1)
        && (Long_Degrees < 3))
      temp_zone = 31;
    if ((Lat_Degrees > 55) && (Lat_Degrees < 64) && (Long_Degrees > 2)
        && (Long_Degrees < 12))
      temp_zone = 32;
    if ((Lat_Degrees > 71) && (Long_Degrees > -1) && (Long_Degrees < 9))
      temp_zone = 31;
    if ((Lat_Degrees > 71) && (Long_Degrees > 8) && (Long_Degrees < 21))
      temp_zone = 33;
    if ((Lat_Degrees > 71) && (Long_Degrees > 20) && (Long_Degrees < 33))
      temp_zone = 35;
    if ((Lat_Degrees > 71) && (Long_Degrees > 32) && (Long_Degrees < 42))
      temp_zone = 37;

    if (UTM_Override)
    {
      if ((temp_zone == 1) && (UTM_Override == 60))
        temp_zone = UTM_Override;
      else if ((temp_zone == 60) && (UTM_Override == 1))
        temp_zone = UTM_Override;
      else if (((temp_zone-1) <= UTM_Override) && (UTM_Override <= (temp_zone+1)))
        temp_zone = UTM_Override;
      else
        Error_Code = UTM_ZONE_OVERRIDE_ERROR;
    }
    if (!Error_Code)
    {
      if (temp_zone >= 31)
        Central_Meridian = (6 * temp_zone - 183) * PI / 180.0;
      else
        Central_Meridian = (6 * temp_zone + 177) * PI / 180.0;
      *Zone = temp_zone;
      if (Latitude < 0)
      {
        False_Northing = 10000000;
        *Hemisphere = 'S';
      }
      else
        *Hemisphere = 'N';
      
	  /*printf("%08.2f %08.2f\n", UTM_a, UTM_f);
	  getchar();
	  */

	  Set_Transverse_Mercator_Parameters(UTM_a, UTM_f, Origin_Latitude,
                                         Central_Meridian, False_Easting, False_Northing, Scale);
      Convert_Geodetic_To_Transverse_Mercator(Latitude, Longitude, Easting,
                                              Northing);
      if ((*Easting < MIN_EASTING) || (*Easting > MAX_EASTING))
        Error_Code = UTM_EASTING_ERROR;
      if ((*Northing < MIN_NORTHING) || (*Northing > MAX_NORTHING))
        Error_Code |= UTM_NORTHING_ERROR;
    }
  } /* END OF if (!Error_Code) */

	/*
     printf("lon_utm = %08.2f\n", lon_utm_33n);
     printf("lat_utm = %f08.2\n", lat_utm_33n);
	 printf("zone_utm = %i\n", zone_utm_33n);
	 printf("hem_utm = %c\n", hem_utm_33n);
	 getchar();
	 */
	 

  return (Error_Code);
} /* END OF Convert_Geodetic_To_UTM */



       

    protected:
        GPGGA(){
            type = GPResult::GPGGA;
        }


    };

    class GPVTG : public GPResult {
    public :
        double  trueCourse;
        double  magneticCourse;
        double  speedKnots;
        double  speedKlm;

        static const GPResult * parse( const char * );

    protected:
        GPVTG(){
            type = GPResult::GPVTG;
        }
    };

    class HCHDG : public GPResult {
    public :
        double  heading;
        double  variation;
    
        static const GPResult * parse( const char * );
    
    protected:
        HCHDG(){
            type = GPResult::HCHDG;
        }
    };

    class PGRMZ: public GPResult {
    public :
        double  altitude;
    
        static const GPResult * parse( const char * );
    
    protected:
        PGRMZ(){
            type = GPResult::PGRMZ;
        }
    };

    /**
     * This is a GPS helper class for parsing the different output strings returned
     * by the GPS receiver module. It will output a class that represents the result
     * and needs to be tested for the correct type. See GPResult. It 
     * implements the following NMEA and extension messages: GPGGA, GPVTG. PGRMZ, HCHDG.
     *
     * @ingroup input
     * @author Gerhard Reitmayr
     */
    class GPSParser  
    {
    public :
        static const GPResult * parse( const char * line );
        static bool checkSum( const char * line );
    private:
        GPSParser(){};
        static const GPResult * (*parsers[])(const char *);
    };

} // namespace ot


#endif // OT_NO_GPS_SUPPORT


#endif // !defined(_GPSPARSER_H_)

/* 
 * ------------------------------------------------------------
 *   End of GPSParser.h
 * ------------------------------------------------------------
 *   Automatic Emacs configuration follows.
 *   Local Variables:
 *   mode:c++
 *   c-basic-offset: 4
 *   eval: (c-set-offset 'substatement-open 0)
 *   eval: (c-set-offset 'case-label '+)
 *   eval: (c-set-offset 'statement 'c-lineup-runin-statements)
 *   eval: (setq indent-tabs-mode nil)
 *   End:
 * ------------------------------------------------------------ 
 */
