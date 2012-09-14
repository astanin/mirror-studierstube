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
 * $Id: GPSParser.cxx 1977 2007-08-27 11:48:30Z bornik $
 *
 * @file                                                                   */
/* ======================================================================= */

#ifdef WIN32
#pragma warning(disable:4244)
#endif

#include <ace/SString.h>

const int NMEABUFSZ = 1024;

#include <OpenTracker/input/GPSParser.h>

#include <OpenTracker/misc/WGS84Geoid.h>

#ifndef OT_NO_GPS_SUPPORT

/* False easting for transformation from GK_M34 to BMN 34 format  */
static double TranMerc_False_Easting_BMN = 750000.0;        /* False easting in meters */




namespace ot {

    const GPResult * (* GPSParser::parsers[])(const char *) = {
        GPGGA::parse,
        GPVTG::parse,
        HCHDG::parse,
        PGRMZ::parse
    };

    const GPResult * GPSParser::parse( const char * line )
    {
        if( checkSum( line ) )
        {
            const GPResult * result = NULL;
            for( int i = 0; i < 4; i++ )
            {
                result = parsers[i](line);
                if( result != NULL )
                    return result;
            }
        }
        return new GPResult;
    }

    bool GPSParser::checkSum( const char * line )
    {
        const char * cp;
        unsigned int csum, checksum;

        cp = strchr(line, '*');
        if (cp) {
            cp++;
            csum = ACE_OS::strtol(cp, NULL, 16);
        
            checksum = 0;
            for (cp = line + 1; *cp; cp++) {
                if (*cp == '*')	/* delimiter */
                    break;
                checksum ^= *cp;
            }
            return (checksum == csum);
        } 
        return false;
    }

    const GPResult * GPGGA::parse( const char * line )
    {
        char         buffer[NMEABUFSZ];
        int          fix = 0,
            statid = 0,
            numsats = 0;
        double       time = 0, 
            lat = 0,
            lon = 0,
            hdop = 0,
            alt = 0,
            height = 0,
            diffdelay = 0;
            


    
        // copy string into work buffer
        ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
        /*
         * $GPGGA,011243,3743.000,N,12214.000,W,2,07,0.4,2.8,M,-27.9,M,0,0269*4
         * 5
         *
         * NOTE time is in hours minutes fractional_minutes
         *
         * time, lat, ns, lon, ew, fix, numsats, hdop, alt, M, height, M,
         * diff-delay, statid, csum
         */   
        if ( ACE_OS::strncmp("$GPGGA,", buffer, 7) == 0) {

            ACE_Tokenizer tok(ACE_TEXT_CHAR_TO_TCHAR(buffer));
            tok.delimiter_replace(',', 0);
            ACE_TCHAR * token, * oldtoken;

            token = tok.next();
            if (!token)
                return new GPResult;

            /* time */
            token = tok.next();
            if (!token)
                return new GPResult;

            // not all receivers output a time, if there is no fix !
            if( token == ACE_TEXT_CHAR_TO_TCHAR(buffer + 7) )
            {            
                oldtoken = token + ACE_OS::strlen( token ) + 1;
            
                /* 012345.0 --> 01:23:45.0 */
                time = atof(ACE_TEXT_ALWAYS_CHAR(token) + 4);
                token[4] = 0;
                time += 60 * atoi(ACE_TEXT_ALWAYS_CHAR(token) + 2);
                token[2] = 0;
                time += 3600 * atoi(ACE_TEXT_ALWAYS_CHAR(token));
            
                token = tok.next();
                if( !token )
                    return new GPResult;
            
                // test for fix, 
                if( token == oldtoken  )
                {            
                    // we have a fix and parse the position
                    /* latitude */
                    lat = atof(ACE_TEXT_ALWAYS_CHAR(token) + 2) / 60.0;
                    token[2] = 0;
                    lat += atof(ACE_TEXT_ALWAYS_CHAR(token));

                    /* ns */
                    token = tok.next();
                    if (!token)
                        return new GPResult;
                    if (*token == 'S') {
                        lat = -lat;
                    }
                
                    /* longitude */
                    token = tok.next();
                    if (!token)
                        return new GPResult;
                    lon = atof(ACE_TEXT_ALWAYS_CHAR(token) + 3) / 60.0;
                    token[3] = 0;
                    lon += atof(ACE_TEXT_ALWAYS_CHAR(token));
                
                    /* ew */
                    token = tok.next();
                    if (!token)
                        return new GPResult;
                    if (*token == 'W') {
                        lon = -lon;
                    }

                    // go to the next token
                    token = tok.next();
                    if (!token)
                        return new GPResult;
                }
            }

            /* fix 0,1,2 */
            fix = ACE_OS::atoi(token);
        
            /* number of sats */
            token = tok.next();
            if (!token)
                return new GPResult;
            numsats = ACE_OS::atoi(token);

            // the following will be only valid, if we have a fix
            if( fix > 0 )
            {
                /* HDOP */
                token = tok.next();
                if (!token)
                    return new GPResult;
                hdop = atof(ACE_TEXT_ALWAYS_CHAR(token));
            
                /* altitude */
                token = tok.next();
                if (!token)
                    return new GPResult;
                alt = atof(ACE_TEXT_ALWAYS_CHAR(token));
            
                /* M - discard  */
                token = tok.next();
                if (!token)
                    return new GPResult;
            
                /* height above geode */
                token = tok.next();
                if (!token)
                    return new GPResult;
                height = atof(ACE_TEXT_ALWAYS_CHAR(token));
            
                /* M - discard */
                token = tok.next();
                if (!token)
                    return new GPResult;
            
                /* differential delay -- optional value */
                token = tok.next();
                if (!token)
                    return new GPResult;
                if( *token != '*' )
                {
                    diffdelay = atof(ACE_TEXT_ALWAYS_CHAR(token));
                    token = tok.next();
                }
            
                /* diff station ID - optional */
                if (!token)
                    return new GPResult;
                if( *token != '*' )
                {
                    statid = ACE_OS::atoi(token);
                    token = tok.next();
                }
            }
            /* ----------- */

            GPGGA * result = new GPGGA;

            result->lat = lat;
            result->lon = lon;
            result->altitude = alt; 
            result->height = height;
            result->time = time;
            result->hdop = hdop;
            result->statid = statid;
            result->numsats = numsats;
            result->diffdelay = diffdelay;
            result->fix = fix;

            // convert lat/lon values to ECEF
            result->convert2ECEF(new WGS84Geoid);
            
            // convert lat/lon values to GK_M34 
			
            double gk_east, gk_north;
            double utm_east, utm_north;
            double gdlat, gdlon;
            long zone = 0;
            char hem = 0x0;

            // convert angles to radians
            gdlat = MathUtils::GradToRad * lat;
            gdlon = MathUtils::GradToRad * lon;

            // For Gauss Krüger projetion the Datum has to be MGI for Austria, not WGS84
            // parameters: double a, double f, double Origin_Latitude, double Central_Meridian,
            // double False_Easting, double False_Northing, double Scale_Factor
            /*long e = */result->Set_Transverse_Mercator_Parameters(6378137.0,
                                                                    (1 / 298.257223563),
                                                                    0,
                                                                    0.285070444,
                                                                    0,
                                                                    -5000000,
                                                                    1);			

            // project coordinates from WGS84 datum to Gauss Krüger format
            result->Convert_Geodetic_To_Transverse_Mercator(gdlat, gdlon, &gk_east, &gk_north);
            result->lon_gk_m34 = gk_east;
            result->lat_gk_m34 = gk_north;

            /*
              printf("\nlon_gk_m34 = %08.2f\n", result->lon_gk_m34);
              printf("lat_gk_m34 = %08.2f\n\n", result->lat_gk_m34);
            */

            // convert values values from GK_M34 to BMN34 format	
            result->lon_bmn_34 = result->lon_gk_m34 + TranMerc_False_Easting_BMN;
            result->lat_bmn_34 = result->lat_gk_m34;

            /*
              printf("\nlon_bmn_34 = %08.2f\n", result->lon_bmn_34);
              printf("lat_bmn_34 = %08.2f\n\n", result->lat_bmn_34);
            */

            /* project coordinates from WGS84 datum to UTM format */

            // parameters: double a, double f, long   override

            long f = result->Set_UTM_Parameters(6378137.0,
                                                (1 / 298.257223563),
                                                0);


            //parameters: double Latitude, double Longitude, long   *Zone,
            //            char   *Hemisphere, double *Easting, double *Northing
            f = result->Convert_Geodetic_To_UTM (gdlat, gdlon, &zone, &hem, &utm_east, &utm_north);

            result->lon_utm_33n = utm_east;
            result->lat_utm_33n = utm_north;
            result->zone_utm_33n = zone;
            result->hem_utm_33n = hem;
			
            /*
              printf("lon_utm = %08.2f\n", result->lon_utm_33n);
              printf("lat_utm = %08.2f\n", result->lat_utm_33n);
              printf("zone_utm = %i\n", result->zone_utm_33n);
              printf("hem_utm = %c\n", result->hem_utm_33n);
              getchar();
            */


            return result;
        }
        // only return null, if we this is not for us.
        return NULL;
    }

    const GPResult * GPVTG::parse( const char * line )
    {
        char    *cp,
            *ep;
        char    buffer[NMEABUFSZ];
        double  trueCourse,
            magneticCourse,
            speedKnots,
            speedKlm;
    
        // copy string into work buffer
        ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
        /*
         * $GPVTG,360.0,T,348.7,M,000.0,N,000.0,K*43
         */    
        if (ACE_OS::strncmp("$GPVTG,", buffer, 7) == 0)
        {        
            /* header */
            cp = ep = (char *)buffer;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
        
            /* true course */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
            trueCourse = atof( cp );
        
            /* T - discard  */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
        
            /* magnetic course */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
            magneticCourse = atof( cp );

            /* M - discard  */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';

            /* speed in knots */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
            speedKnots = atof( cp );
        
            /* N - discard  */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';

            /* speed in klms */
            cp = ep;
            ep = strchr(cp, ',');
            if (!ep)
                return new GPResult;
            *ep++ = '\0';
            speedKlm = atof( cp );
    
            GPVTG * result = new GPVTG;
            result->trueCourse = trueCourse;
            result->magneticCourse = magneticCourse;
            result->speedKnots = speedKnots;
            result->speedKlm = speedKlm;
            return result;
        }
        return NULL;
    }

    const GPResult * HCHDG::parse( const char * line )
    {
        char    buffer[NMEABUFSZ];
    
        // copy string into work buffer
        ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
        /*
         * $HCHDG,101.1,,,7.1,W*3C
         */    
        if (ACE_OS::strncmp("$HCHDG,", buffer, 7) == 0)
        {        
            ACE_Tokenizer tok( ACE_TEXT_CHAR_TO_TCHAR(buffer) );
            tok.delimiter_replace(',', 0);
            ACE_TCHAR * token;
            double  heading,
                variation;
        
            /* skip header */
            token = tok.next();
            if( !token )
                return new GPResult;

            /* heading */
            token = tok.next();
            if (!token)
                return new GPResult;
            heading = atof( ACE_TEXT_ALWAYS_CHAR(token) );

            /* next two ',' are skipped by tokenizing */

            /* variation */
            token = tok.next();
            if (!token)
                return new GPResult;
            variation = atof( ACE_TEXT_ALWAYS_CHAR(token) );

            /* direction */
            token = tok.next();
            if (!token)
                return new GPResult;
        
            if( ACE_OS::strcmp(token,  ACE_TEXT_CHAR_TO_TCHAR("E") ))
            {
                variation = - variation;
            }

            HCHDG * result = new HCHDG;
            result->heading = heading;
            result->variation = variation;
            return result;
        }
        return NULL;
    }

    const GPResult * PGRMZ::parse( const char * line )
    {
        char buffer[NMEABUFSZ];
    
        // copy string into work buffer
        ACE_OS::strncpy( buffer, line, NMEABUFSZ);
    
        /*
         * $PGRMZ,246,f,3*1B
         */    
        if (ACE_OS::strncmp("$PGRMZ,", buffer, 7) == 0)
        {        
            ACE_Tokenizer tok( ACE_TEXT_CHAR_TO_TCHAR(buffer) );
            tok.delimiter_replace(',', 0);
            ACE_TCHAR * token;
            double altitude;
        
            /* skip header */
            token = tok.next();
            if( !token )
                return new GPResult;

            /* altitude in feet */
            token = tok.next();
            if (!token)
                return new GPResult;
            altitude = atof( ACE_TEXT_ALWAYS_CHAR(token) );        
        
            PGRMZ * result = new PGRMZ;
            result->altitude = altitude; 
            return result;
        }
        return NULL;
    }

} // namespace ot


#else
#pragma message(">>> OT_NO_GPS_SUPPORT")
#endif // OT_NO_GPS_SUPPORT

/* 
 * ------------------------------------------------------------
 *   End of GPSParser.cxx
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
