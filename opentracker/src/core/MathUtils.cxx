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
/** source file for MathUtils class, a collection of mathematical functions.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MathUtils.cxx 1555 2006-10-25 11:33:09Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#include <OpenTracker/core/MathUtils.h>
#include <OpenTracker/core/Event.h>
#include <cmath>
#include <cstdio>
#include <cassert>

/* tolerance for quaternion operations */
const double Q_EPSILON = (1e-10);

/**
 * a collection of numerically safer implementations of various terms.
 * The class is a template to provide inline implementations of the
 * actual calculations. This allows maximum speed :). See
 * http://www.hadron.org/~hatch/rightway.php for details on the
 * formulas.
 * @ingroup core
 * @author Gerhard Reitmayr
 */

namespace ot {

    template <typename T> class MathStuff {

    public:
        static T sin_over_x( T x ) {
            if ((T)1. + x*x == (T)1.)
                return (T)1.;
            else
                return sin(x)/x;
        };
    };

    typedef MathStuff<double> MD;
    typedef MathStuff<float> MF;

    const double MathUtils::Pi = 3.1415926535897932385;

    const double MathUtils::E = 2.7182818284590452354;

    const double MathUtils::GradToRad = MathUtils::Pi / 180.0;

    const MathUtils::Matrix4x4 MathUtils::matrix4x4_flipY = {{1, 0, 0, 0},
                                                             {0,-1, 0, 0},
                                                             {0, 0, 1, 0},
                                                             {0, 0, 0, 1}};

    const MathUtils::Matrix4x4 MathUtils::matrix4x4_identity = {{1, 0, 0, 0},
                                                                {0, 1, 0, 0},
                                                                {0, 0, 1, 0},
                                                                {0, 0, 0, 1}};

    // converts an axis angle representation into a quaternion

    float* MathUtils::axisAngleToQuaternion(const float* axisa, float* qResult)
    {
        double s = sin( axisa[3]/2 );
        qResult[3] = (float)cos( axisa[3]/2 );
        qResult[0] = (float)(axisa[0]*s);
        qResult[1] = (float)(axisa[1]*s);
        qResult[2] = (float)(axisa[2]*s);
        return qResult;
    }

    std::vector<float>& MathUtils::axisAngleToQuaternion(const std::vector<float> &axisa, std::vector<float> &qResult)
    {
        float axisaA[4];
        float qResultA[4];

        copyV2A(axisa, axisaA);
        axisAngleToQuaternion(axisaA, qResultA);
        copyA2V(qResultA, 4, qResult);

        return qResult;
    }

    double* MathUtils::axisAngleToQuaternion(const double* axisa, Quaternion qResult)
    {
        double s = sin( axisa[3]/2 );
        qResult[3] = cos( axisa[3]/2 );
        qResult[0] = (axisa[0]*s);
        qResult[1] = (axisa[1]*s);
        qResult[2] = (axisa[2]*s);
        return qResult;
    }

    // computes a quaternion from euler angles representing a rotation.

    float* MathUtils::eulerToQuaternion(const float roll, const float pitch, const float yaw,
                                        float* qResult)
    {
        double cr, cp, cy, sr, sp, sy, cpcy, spsy;

        // calculate trig identities
        cr = cos(roll/2);
        cp = cos(pitch/2);
        cy = cos(yaw/2);

        sr = sin(roll/2);
        sp = sin(pitch/2);
        sy = sin(yaw/2);

        cpcy = cp * cy;
        spsy = sp * sy;

        qResult[0] = (float)(sr * cpcy - cr * spsy);
        qResult[1] = (float)(cr * sp * cy + sr * cp * sy);
        qResult[2] = (float)(cr * cp * sy - sr * sp * cy);
        qResult[3] = (float)(cr * cpcy + sr * spsy);
        return qResult;
    }

    std::vector<float>& MathUtils::eulerToQuaternion(const float roll, const float pitch, const float yaw, std::vector<float> &qResult)
    {
        float qResultA[4];
        eulerToQuaternion(roll, pitch, yaw, qResultA);
        copyA2V(qResultA, 4, qResult);
        return qResult;
    }

    // computes a quaternion from euler angles representing a rotation. new version

    float * MathUtils::eulerToQuaternion( const double alpha, const double beta,
                                          const double gamma, const enum MathUtils::EULER sequence,
                                          float * qResult )
    {
        float q1[4] = {0,0,0,0}, q2[4] = {0,0,0,0}, q3[4] = {0,0,0,0};

        int axis1 = sequence >> 4;
        assert( 0 <= axis1 && axis1 <= 2 );
        int axis2 = (sequence >> 2) & 3;
        assert( 0 <= axis2 && axis2 <= 2 );
        int axis3 = sequence & 3;
        assert( 0 <= axis3 && axis3 <= 2 );

        q1[axis1] = (float)sin(alpha/2);
        q1[3] = (float)cos(alpha/2);

        q2[axis2] = (float)sin(beta/2);
        q2[3] = (float)cos(beta/2);

        q3[axis3] = (float)sin(gamma/2);
        q3[3] = (float)cos(gamma/2);

        float temp[4];

        MathUtils::multiplyQuaternion(q1, q2, temp);
        MathUtils::multiplyQuaternion(temp, q3, qResult);
        return qResult;
    }

    // Inverts a Quaternion. Returns result in the second parameter.

    float* MathUtils::invertQuaternion(const float* q, float* qResult)
    {
        double mod = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
        qResult[0] = (float)( -q[0] / mod );
        qResult[1] = (float)( -q[1] / mod );
        qResult[2] = (float)( -q[2] / mod );
        qResult[3] = (float)( q[3] / mod );
        return qResult;
    }

    std::vector<float>& MathUtils::invertQuaternion(const std::vector<float> &q, std::vector<float> &qResult)
    {
        float qA[4];
        float qResultA[4];

        copyV2A(q, qA);
        invertQuaternion(qA, qResultA);
        copyA2V(qResultA, 4, qResult);

        return qResult;
    }

    // computes a quaternion from a passed rotation matrix.

    float* MathUtils::matrixToQuaternion(const float matrix[3][3], float* qResult)
    {
        double tr, s;
        int i, j, k;
        int nxt[3] = {1, 2, 0};

        tr = matrix[0][0] + matrix[1][1] + matrix[2][2];

        // check the diagonal
        if (tr > 0.0)
        {
            s = sqrt (tr + 1.0);
            qResult[3] =(float)( s / 2.0 );
            s = 0.5 / s;
            qResult[0] = (float)((matrix[2][1] - matrix[1][2]) * s);
            qResult[1] = (float)((matrix[0][2] - matrix[2][0]) * s);
            qResult[2] = (float)((matrix[1][0] - matrix[0][1]) * s);
        }
        else
        {
            // diagonal is negative
            i = 0;
            if (matrix[1][1] > matrix[0][0]) i = 1;
            if (matrix[2][2] > matrix[i][i]) i = 2;
            j = nxt[i];
            k = nxt[j];

            s = sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0);

            qResult[i] = (float)( s * 0.5 );

            if (s != 0.0) s = 0.5 / s;

            qResult[3] =(float)((matrix[k][j] - matrix[j][k]) * s );
            qResult[j] =(float)( (matrix[j][i] + matrix[i][j]) * s );
            qResult[k] =(float)( (matrix[k][i] + matrix[i][k]) * s );
        }
        return qResult;
    }

    std::vector<float>& MathUtils::matrixToQuaternion(const float matrix[3][3], std::vector<float> &qResult)
    {
        float qResultA[4];
        matrixToQuaternion(matrix, qResultA);
        copyA2V(qResultA, 4, qResult);
        return qResult;
    }

    // multiplies two quaternions and returns result in a third.

    float* MathUtils::multiplyQuaternion(const float* q1,const float* q2, float* qResult)
    {
        qResult[0] = q1[3]*q2[0]+q1[0]*q2[3]+q1[1]*q2[2]-q1[2]*q2[1];
        qResult[1] = q1[3]*q2[1]-q1[0]*q2[2]+q1[1]*q2[3]+q1[2]*q2[0];
        qResult[2] = q1[3]*q2[2]+q1[0]*q2[1]-q1[1]*q2[0]+q1[2]*q2[3];
        qResult[3] = q1[3]*q2[3]-q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2];
        return qResult;
    }

    std::vector<float>& MathUtils::multiplyQuaternion(const std::vector<float> &q1, const std::vector<float> &q2, std::vector<float> &qResult)
    {
        float q1A[4];
        float q2A[4];
        float qResultA[4];

        copyV2A(q1, q1A);
        copyV2A(q2, q2A);
        multiplyQuaternion(q1A, q2A, qResultA);
        copyA2V(qResultA, 4, qResult);

        return qResult;
    }

    double* MathUtils::multiplyQuaternion(const ot::MathUtils::Quaternion q1,
                                          const ot::MathUtils::Quaternion q2,
                                          ot::MathUtils::Quaternion qResult)
    {
        qResult[0] = q1[3]*q2[0]+q1[0]*q2[3]+q1[1]*q2[2]-q1[2]*q2[1];
        qResult[1] = q1[3]*q2[1]-q1[0]*q2[2]+q1[1]*q2[3]+q1[2]*q2[0];
        qResult[2] = q1[3]*q2[2]+q1[0]*q2[1]-q1[1]*q2[0]+q1[2]*q2[3];
        qResult[3] = q1[3]*q2[3]-q1[0]*q2[0]-q1[1]*q2[1]-q1[2]*q2[2];
        return qResult;
    }

    // normalizes a quaternion to a unit quaternion.

    float* MathUtils::normalizeQuaternion(float* q)
    {
        double mod = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
        q[0] = (float)( q[0] / mod );
        q[1] = (float)( q[1] / mod );
        q[2] = (float)( q[2] / mod );
        q[3] = (float)( q[3] / mod );
        return q;
    }

    // normalizes a quaternion to a unit quaternion.

    std::vector<float>& MathUtils::normalizeQuaternion(std::vector<float> &q)
    {
        float qA[4];

        copyV2A(q, qA);
        normalizeQuaternion(qA);
        copyA2V(qA, 4, q);

        return q;
    }

    // normalizes a quaternion to a unit quaternion.

    double* MathUtils::normalizeQuaternion(Quaternion q)
    {
        double mod = sqrt(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]+q[3]*q[3]);
        q[0] = q[0] / mod;
        q[1] = q[1] / mod;
        q[2] = q[2] / mod;
        q[3] = q[3] / mod;
        return q;
    }

    // rotates a vector by a given quaternion.

    float* MathUtils::rotateVector(const float* q, const float* v, float* vResult)
    {
        float t1[4], t2[4], t3[4];
        // vector in t2
        t2[0] = v[0];
        t2[1] = v[1];
        t2[2] = v[2];
        t2[3] = 0;
        // inverse in t1
        invertQuaternion( q, t1 );
        // v * t1 -> t3
        multiplyQuaternion( t2, t1, t3 );
        // q * t3 -> t1
        multiplyQuaternion( q, t3, t1 );
        // t1 result -> vResult
        vResult[0] = t1[0];
        vResult[1] = t1[1];
        vResult[2] = t1[2];
        return vResult;
    }

    std::vector<float>& MathUtils::rotateVector(const std::vector<float> &q, const std::vector<float> &v, std::vector<float> &vResult)
    {
        float qA[4];
        float vA[3];
        float vResultA[3];

        copyV2A(q, qA);
        copyV2A(v, vA);
        rotateVector(qA, vA, vResultA);
        copyA2V(vResultA, 3, vResult);

        return vResult;
    }

    // computes determinant of a matrix

    float MathUtils::determinant( const float matrix[3][3] )
    {
        double res = 0;
        res += matrix[0][0]*matrix[1][1]*matrix[2][2];
        res += matrix[0][1]*matrix[1][2]*matrix[2][0];
        res += matrix[0][2]*matrix[1][0]*matrix[2][1];
        res -= matrix[0][0]*matrix[1][2]*matrix[2][1];
        res -= matrix[0][1]*matrix[1][0]*matrix[2][2];
        res -= matrix[0][2]*matrix[1][1]*matrix[2][0];
        return (float)res;
    }

    // compute angle in a more robust fashion

    double MathUtils::angle( const float * v1, const float * v2, const int dim )
    {
        int i;
        double dot = MathUtils::dot( v1, v2, dim );
        /* Numerically stable implementation:
         *  if (dot(u,v) < 0.)
         *      return M_PI - 2*asin(|v+u)|/2)
         *  else
         *      return 2*asin(|v-u|/2)
         */
        if(dot < 0.0)
        {
            dot = 0;
            for( i = 0; i < dim; i++)
                dot += (v2[i]+v1[i])*(v2[i]+v1[i]);
            dot = sqrt(dot) / 2.0;
            return MathUtils::Pi - 2*asin( dot );
        }
        else
        {
            dot = 0;
            for( i = 0; i < dim; i++)
                dot += (v2[i]-v1[i])*(v2[i]-v1[i]);
            dot = sqrt( dot ) / 2.0;
            return 2*asin(dot);
        }
        return 0;
    }

    double MathUtils::angle( const std::vector<float> &v1, const std::vector<float> & v2, const int dim )
    {
        float *v1A = (float*)malloc(dim * sizeof(float));
        float *v2A = (float*)malloc(dim * sizeof(float));
        double retValue;

        copyV2A(v1, v1A);
        copyV2A(v2, v2A);
        retValue = angle(v1A, v2A, dim);
        free(v1A);
        free(v2A);
        return retValue;
    }

    // computes slerp

    float * MathUtils::slerp( const float * q1, const float *q2, const float t, float * qResult )
    {

        const float*    r1q = q2;

        float           rot1q[4];
        double          omega, cosom, sinom;
        double          scalerot0, scalerot1;
        int             i;

        // Calculate the cosine
        cosom = q1[0]*q2[0] + q1[1]*q2[1]
            + q1[2]*q2[2] + q1[3]*q2[3];

        // adjust signs if necessary
        if ( cosom < 0.0 ) {
            cosom = -cosom;
            for ( int j = 0; j < 4; j++ )
                rot1q[j] = -r1q[j];
        } else  {
            for ( int j = 0; j < 4; j++ )
                rot1q[j] = r1q[j];
        }

        // calculate interpolating coeffs
        if ( (1.0 - cosom) > 0.00001 ) {
            // standard case
            omega = acos(cosom);
            sinom = sin(omega);
            scalerot0 = sin((1.0 - t) * omega) / sinom;
            scalerot1 = sin(t * omega) / sinom;
        } else {
            // rot0 and rot1 very close - just do linear interp.
            scalerot0 = 1.0 - t;
            scalerot1 = t;
        }

        // build the new quarternion
        for (i = 0; i <4; i++)
            qResult[i] = (float)(scalerot0 * q1[i] + scalerot1 * rot1q[i]);

        MathUtils::normalizeQuaternion( qResult );

        return qResult;

        /*
          double angle = MathUtils::angle( q1, q2, 4);
          float temp[4];
          if( angle < 0)
          {
          temp[0] = -q2[0];
          temp[1] = -q2[1];
          temp[2] = -q2[2];
          temp[3] = -q2[3];
          angle = MathUtils::angle( q1, temp, 4);
          }
          else
          {
          temp[0] = q2[0];
          temp[1] = q2[1];
          temp[2] = q2[2];
          temp[3] = q2[3];
          }
          double c1 = MD::sin_over_x((1-t)*angle) * (t-1) / MD::sin_over_x(angle);
          double c2 = MD::sin_over_x(t*angle) * t / MD::sin_over_x(angle);
          int i;
          for( i = 0; i < 4; i++)
          {
          qResult[i] = (float)(q1[i] * c1 + temp[i]*c2);
          }
        */

        return qResult;
    }

    std::vector<float>& MathUtils::slerp( const std::vector<float> &q1, const std::vector<float> &q2, const float t, std::vector<float> &qResult )
    {
        float q1A[4];
        float q2A[4];
        float qResultA[4];

        copyV2A(q1, q1A);
        copyV2A(q2, q2A);
        slerp(q1A, q2A, t, qResultA);
        copyA2V(qResultA, 4, qResult);

        return qResult;
    }
    
   void MathUtils::quaternionToMatrix( Quaternion& q, Matrix4x4& m)
    {
    	// also see p. 363 of the book "Advanced Animation and Rendering Techniques 
    	// from A. and M. Watt from Addison-Wesley

        MathUtils::normalizeQuaternion( q );
    
    	const double x(q[0]);
    	const double y(q[1]);
    	const double z(q[2]);
    	const double w(q[3]);
    
    	const double s( 2.0 / (x * x + y * y + z * z + w * w) );
    
    	const double xx(x * x * s);
    	const double xy(x * y * s);
    	const double xz(x * z * s);
    	const double xw(x * w * s);
    
    	const double yy(y * y * s);
    	const double yz(y * z * s);
    	const double yw(y * w * s);
    
    	const double zz(z * z * s);
    	const double zw(z * w * s);
    	
    	m[0][0] = 1.0 - (yy + zz);
    	m[1][0]  = xy + zw;
    	m[2][0] = xz - yw;
    
    	m[0][1] = xy - zw;
    	m[1][1] = 1.0 - (xx + zz);
    	m[2][1] = yz + xw;
    
    	m[0][2] = xz + yw;
    	m[1][2] = yz - xw;
    	m[2][2] = 1.0 - (xx + yy);

		m[3][3] = 1;
    }

   void MathUtils::matrixToQuaternion(Matrix4x4& matrix, Quaternion& qResult)
   {
	   double tr, s;
	   int i, j, k;
	   int nxt[3] = {1, 2, 0};

	   tr = matrix[0][0] + matrix[1][1] + matrix[2][2];

	   // check the diagonal
	   if (tr > 0.0)
	   {
		   s = sqrt (tr + 1.0);
		   qResult[3] = ( s / 2.0 );
		   s = 0.5 / s;
		   qResult[0] = ((matrix[2][1] - matrix[1][2]) * s);
		   qResult[1] = ((matrix[0][2] - matrix[2][0]) * s);
		   qResult[2] = ((matrix[1][0] - matrix[0][1]) * s);
	   }
	   else
	   {
		   // diagonal is negative
		   i = 0;
		   if (matrix[1][1] > matrix[0][0]) i = 1;
		   if (matrix[2][2] > matrix[i][i]) i = 2;
		   j = nxt[i];
		   k = nxt[j];

		   s = sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0);

		   qResult[i] = ( s * 0.5 );

		   if (s != 0.0) s = 0.5 / s;

		   qResult[3] = ((matrix[k][j] - matrix[j][k]) * s );
		   qResult[j] = ( (matrix[j][i] + matrix[i][j]) * s );
		   qResult[k] = ( (matrix[k][i] + matrix[i][k]) * s );
	   }
   }

    // ----------------------------------------------------------------------------------------
    void MathUtils::matrixMultiply(const Matrix4x4 m1, const Matrix4x4 m2, Matrix4x4 &m)
    {
        m[0][0] = m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0] + m1[0][3]*m2[3][0];
        m[0][1] = m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1] + m1[0][3]*m2[3][1];
        m[0][2] = m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2] + m1[0][3]*m2[3][2];
        m[0][3] = m1[0][0]*m2[0][3] + m1[0][1]*m2[1][3] + m1[0][2]*m2[2][3] + m1[0][3]*m2[3][3];
        m[1][0] = m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0] + m1[1][3]*m2[3][0];
        m[1][1] = m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1] + m1[1][3]*m2[3][1];
        m[1][2] = m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2] + m1[1][3]*m2[3][2];
        m[1][3] = m1[1][0]*m2[0][3] + m1[1][1]*m2[1][3] + m1[1][2]*m2[2][3] + m1[1][3]*m2[3][3];
        m[2][0] = m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0] + m1[2][3]*m2[3][0];
        m[2][1] = m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1] + m1[2][3]*m2[3][1];
        m[2][2] = m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2] + m1[2][3]*m2[3][2];
        m[2][3] = m1[2][0]*m2[0][3] + m1[2][1]*m2[1][3] + m1[2][2]*m2[2][3] + m1[2][3]*m2[3][3];
        m[3][0] = m1[3][0]*m2[0][0] + m1[3][1]*m2[1][0] + m1[3][2]*m2[2][0] + m1[3][3]*m2[3][0];
        m[3][1] = m1[3][0]*m2[0][1] + m1[3][1]*m2[1][1] + m1[3][2]*m2[2][1] + m1[3][3]*m2[3][1];
        m[3][2] = m1[3][0]*m2[0][2] + m1[3][1]*m2[1][2] + m1[3][2]*m2[2][2] + m1[3][3]*m2[3][2];
        m[3][3] = m1[3][0]*m2[0][3] + m1[3][1]*m2[1][3] + m1[3][2]*m2[2][3] + m1[3][3]*m2[3][3];
    }

    // ----------------------------------------------------------------------------------------
    void MathUtils::matrixMultiply(const Matrix3x3 m1, const Matrix3x3 m2, Matrix3x3 &m)
    {
        m[0][0] = m1[0][0]*m2[0][0] + m1[0][1]*m2[1][0] + m1[0][2]*m2[2][0];
        m[0][1] = m1[0][0]*m2[0][1] + m1[0][1]*m2[1][1] + m1[0][2]*m2[2][1];
        m[0][2] = m1[0][0]*m2[0][2] + m1[0][1]*m2[1][2] + m1[0][2]*m2[2][2];
        m[1][0] = m1[1][0]*m2[0][0] + m1[1][1]*m2[1][0] + m1[1][2]*m2[2][0];
        m[1][1] = m1[1][0]*m2[0][1] + m1[1][1]*m2[1][1] + m1[1][2]*m2[2][1];
        m[1][2] = m1[1][0]*m2[0][2] + m1[1][1]*m2[1][2] + m1[1][2]*m2[2][2];
        m[2][0] = m1[2][0]*m2[0][0] + m1[2][1]*m2[1][0] + m1[2][2]*m2[2][0];
        m[2][1] = m1[2][0]*m2[0][1] + m1[2][1]*m2[1][1] + m1[2][2]*m2[2][1];
        m[2][2] = m1[2][0]*m2[0][2] + m1[2][1]*m2[1][2] + m1[2][2]*m2[2][2];
    }

    // ----------------------------------------------------------------------------------------

    void MathUtils::MatrixToEuler(Vector3 angles, const Matrix4x4 colMatrix)
    {

        double sinPitch, cosPitch, sinRoll, cosRoll, sinYaw, cosYaw;


        sinPitch = -colMatrix[2][0];
        cosPitch = sqrt(1 - sinPitch*sinPitch);

        if ( fabs(cosPitch) > Q_EPSILON )
        {
            sinRoll = colMatrix[2][1] / cosPitch;
            cosRoll = colMatrix[2][2] / cosPitch;
            sinYaw = colMatrix[1][0] / cosPitch;
            cosYaw = colMatrix[0][0] / cosPitch;
        }
        else
        {
            sinRoll = -colMatrix[1][2];
            cosRoll = colMatrix[1][1];
            sinYaw = 0;
            cosYaw = 1;
        }

        /* yaw */
        angles[0] = atan2(sinYaw, cosYaw);

        /* pitch */
        angles[1] = atan2(sinPitch, cosPitch);

        /* roll */
        angles[2] = atan2(sinRoll, cosRoll);

    } /* MatrixToEuler */

    ////////////////////////////////////////////////////////////////////////
    //
    //   Heavily based on code by Ken Shoemake and code by Gavin Bell
    float *MathUtils::quaternionToAxisAngle(const float q[4], float axisa[4])
    {
        double	len;

        len = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
        // check quaternion
        if (len  > Q_EPSILON) {
            // if valid compute vec and angle
            // normalize vec
            float invLen = (float)(1.0/ len);
            axisa[0]	= q[0] * invLen;
            axisa[1]	= q[1] * invLen;
            axisa[2]	= q[2] * invLen;

            axisa[3]	=(float)( 2.0f * (float)acos(q[3]));
        }

        else {
            // return identity quaternion
            axisa[0] = 0.0;
            axisa[1] = 0.0;
            axisa[2] = 1.0;
            axisa[3] = 0.0;
        }
        return axisa;
    }   

    void MathUtils::quaternionToAxisAngle(std::vector<float>& q, std::vector<float>& axisa)
    {
        double	len;

        len = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2]);
        // check quaternion
        if (len  > Q_EPSILON) {
            // if valid compute vec and angle
            // normalize vec
            float invLen = (float)(1.0/ len);
            axisa[0]	= q[0] * invLen;
            axisa[1]	= q[1] * invLen;
            axisa[2]	= q[2] * invLen;

            axisa[3]	=(float)( 2.0f * (float)acos(q[3]));
        }

        else {
            // return identity quaternion
            axisa[0] = 0.0;
            axisa[1] = 0.0;
            axisa[2] = 1.0;
            axisa[3] = 0.0;
        }
        //return axisa;
    }


    void MathUtils::translationQuaternionTo4x4Matrix(std::vector<float>& pos, std::vector<float>& quat, MathUtils::Matrix4x4& mat)
{
    float    aa = quat[0]*quat[0];
    float    ab = quat[0]*quat[1];
    float    ac = quat[0]*quat[2];
    float    ad = quat[0]*quat[3];
    float    bb = quat[1]*quat[1];
    float    bc = quat[1]*quat[2];
    float    bd = quat[1]*quat[3];
    float    cc = quat[2]*quat[2];
    float    cd = quat[2]*quat[3];
    float    dd = quat[3]*quat[3];
    
    float    norme_carre = aa+bb+cc+dd;
    
    if    (norme_carre <= Q_EPSILON)
    {
        // Quaternion values too small
        // Set rotational part to identity matrix
        // first row
        mat[0][0] = 1.0;
        mat[0][1] = 0.0;
        mat[0][2] = 0.0;
        mat[0][3] = pos[0];

        // second row
        mat[1][0] = 0.0;
        mat[1][1] = 1.0;
        mat[1][2] = 0.0;
        mat[1][3] = 0.0;

        // third row
        mat[2][0] = 0.0;
        mat[2][1] = 0.0;
        mat[2][2] = 1.0;
        mat[2][3] = 0.0;
        
        // fourth row
        mat[3][0] = 0.0;
        mat[3][1] = 0.0;
        mat[3][2] = 0.0;
        mat[3][3] = 1.0;

        return;
    }
    
    mat[0][0] = (aa+bb-cc-dd)/norme_carre;
    mat[0][1] = 2*(-ad+bc)/norme_carre;
    mat[0][2] = 2*(ac+bd)/norme_carre;
    mat[0][3] = pos[0];

    mat[1][0] = 2*(ad+bc)/norme_carre;
    mat[1][1] = (aa-bb+cc-dd)/norme_carre;
    mat[1][2] = 2*(-ab+cd)/norme_carre;
    mat[1][3] = pos[1];

    mat[2][0] = 2*(-ac+bd)/norme_carre;
    mat[2][1] = 2*(ab+cd)/norme_carre;
    mat[2][2] = (aa-bb-cc+dd)/norme_carre;
    mat[2][3] = pos[2];

    mat[3][0] = 0.0;
    mat[3][1] = 0.0;
    mat[3][2] = 0.0;
    mat[3][3] = 1.0;
}


    double MathUtils::dot( const float * v1, const float * v2, const int dim )
    {
        double result = 0;
        int i;
        for( i = 0; i < dim; i++ )
        {
            result += v1[i] * v2[i];
        }
        return result;
    }

    double MathUtils::dot( const std::vector<float> &v1, const std::vector<float> &v2, const int dim )
    {
        float *v1A = (float*)malloc(dim * sizeof(float));
        float *v2A = (float*)malloc(dim * sizeof(float));
        double retValue;

        copyV2A(v1, v1A);
        copyV2A(v2, v2A);
        retValue = dot(v1A, v2A, dim);
        free(v1A);
        free(v2A);
        return retValue;
    }


} // namespace ot

/* 
 * ------------------------------------------------------------
 *   End of MathUtils.cxx
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
