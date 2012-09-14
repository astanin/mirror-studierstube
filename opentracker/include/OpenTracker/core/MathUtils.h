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
/** Header file for MathUtils class, a collection of mathematical functions.
 *
 * @author Gerhard Reitmayr
 *
 * $Id: MathUtils.h 1968 2007-08-20 10:23:22Z bornik $
 * @file                                                                   */
/* ======================================================================= */

#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#ifndef SWIG
#include <vector>
#endif

#include "../dllinclude.h"

/* for accessing the elements of Quaternion and Vector3 */
#define Q_X    0
#define Q_Y    1
#define Q_Z    2
#define Q_W    3

/**
 * This class implements some utility classes for matrix and quaternion
 * calculations. These are static methods, I just used a class to have
 * a common place for such things, like in Java. These do not handle any
 * memory issues, all arrays etc. have to be created and managed by the
 * calling code.
 * @author Gerhard Reitmayr
 * @ingroup core
 */

namespace ot {

    class OPENTRACKER_API MathUtils
    {
    public:
        /// 3x3 double matrix type
        typedef double Matrix3x3[3][3];
        /// 4x4 double matrix type
        typedef double Matrix4x4[4][4];
        /// 3 double vector
        typedef double Vector3[3];
        /// 4 double vector used as quaternion
        typedef double Quaternion[4];

        /// encodes all possible euler angle sequences
        enum EULER {
            // the encoding is as follows :
            // the two lsb bits encode the last axis
            // the next two the middle axis
            // and the next two the first axis
            // X = 0, Y = 1, Z = 2
            // the enums are ordered by integer value
            XYX =  0 + 4 + 0,
            XYZ =  0 + 4 + 2,
            XZX =  0 + 8 + 0,
            XZY =  0 + 8 + 1,
            YXY = 16 + 0 + 1,
            YXZ = 16 + 0 + 2,
            YZX = 16 + 8 + 0,
            YZY = 16 + 8 + 1,
            ZXY = 32 + 0 + 1,
            ZXZ = 32 + 0 + 2,
            ZYX = 32 + 4 + 0,
            ZYZ = 32 + 4 + 2
        };

        /** converts an axis angle representation into a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param axisa float[4] containing axis and angle in radiants
         * @param qResult float[4] where the result is stored
         * @return pointer to result array */
        static float* axisAngleToQuaternion(const float* axisa, float* qResult);
        /** converts an axis angle representation into a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param axisa vector<float> (assumes length 4) containing axis and angle in radiants
         * @param qResult vector<float> (assumes length 4) where the result is stored
         * @return reference to result vector */
        static std::vector<float>& axisAngleToQuaternion(const std::vector<float> &axisa, std::vector<float> &qResult);
        /** converts an axis angle representation into a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param axisa double[4] containing axis and angle in radiants
         * @param qResult double[4] where the result is stored
         * @return pointer to result array */
        static double* axisAngleToQuaternion(const double* axisa, Quaternion qResult);
        /** computes a quaternion from euler angles representing a rotation.
         * @deprecated use the other overloaded variant instead.
         * @param roll rotation around looking axis
         * @param pitch rotation around up axis
         * @param yaw rotation around side axis
         * @param qResult float[4] where the result is stored
         * @return pointer to result array */
        static float* eulerToQuaternion(const float roll, const float pitch, const float yaw, float* qResult);
        /** computes a quaternion from euler angles representing a rotation.
         * @deprecated use the other overloaded variant instead.
         * @param roll rotation around looking axis
         * @param pitch rotation around up axis
         * @param yaw rotation around side axis
         * @param qResult vector<float> where the result is stored
         * @return reference to result vector */
        static std::vector<float>& eulerToQuaternion(const float roll, const float pitch, const float yaw, std::vector<float> &qResult);
        /** computes a quaternion from euler angles representing a rotation. This
         * is the general method and supports all euler angle sequences. Use this instead
         * of the old one for more control.
         * @param alpha first euler angle
         * @param beta second euler angle
         * @param gamma third euler angle
         * @param sequence enum defining the sequence to use
         * @param qResult pointer to the result quaternion
         * @return returns the pointer to the result quaternion
         */
        static float * eulerToQuaternion( const double alpha, const double beta,
                                          const double gamma, const enum MathUtils::EULER sequence,
                                          float * qResult );
        /** inverts a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q float[4] storing the quaternion
         * @param qResult float[4] where the result is stored
         * @return pointer to result array */
        static float* invertQuaternion(const float* q, float* qResult);
        /** inverts a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q vector<float> (assumes length 4) storing the quaternion
         * @param qResult vector<float> where the result is stored
         * @return reference to result vector */
        static std::vector<float>& invertQuaternion(const std::vector<float> &q, std::vector<float> &qResult);
        /** converts a rotational matrix to a quaternion.
         * @param matrix float[3][3] storing the rotational matrix
         * @param qResult float[4] where the result is stored
         * @return pointer to result array */
        static float* matrixToQuaternion(const float matrix[3][3], float* qResult);
        /** converts a rotational matrix to a quaternion.
         * @param matrix float[3][3] storing the rotational matrix
         * @param qResult vector<float>  where the result is stored
         * @return reference to result vector */
        static std::vector<float>& matrixToQuaternion(const float matrix[3][3], std::vector<float> &qResult);
        /** multiplies two quaternions and stores result in a third. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q1 float[4] storing first quaternion
         * @param q2 float[4] storing second quaternion
         * @param qResult float[4] where the result is stored
         * @return pointer to result array */
        static float* multiplyQuaternion(const float* q1, const float* q2, float* qResult);
        /** multiplies two quaternions and stores result in a third. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q1 vector<float> (assumes length 4) storing first quaternion
         * @param q2 vector<float> (assumes length 4) storing second quaternion
         * @param qResult vector<float> where the result is stored
         * @return pointer to result vector */
        static std::vector<float>& multiplyQuaternion(const std::vector<float> &q1, const std::vector<float> &q2, std::vector<float> &qResult);
        /** multiplies two quaternions and stores result in a third. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q1 Quaternion storing first quaternion
         * @param q2 Quaternion storing second quaternion
         * @param qResult Quaternion where the result is stored
         * @return pointer to result array */
        static double* multiplyQuaternion(const Quaternion q1, const Quaternion q2, Quaternion qResult);
        /** normalizes quaternion to unit length. Here the computation is
         * done in place and the parameter is changed !
         * @param q float[4] storing quaternion
         * @return pointer to result array */
        static float* normalizeQuaternion(float* q);
        /** normalizes quaternion to unit length. Here the computation is
         * done in place and the parameter is changed !
         * @param q vector<float> (assumes length 4) storing quaternion
         * @return reference to result vector */
        static std::vector<float>& normalizeQuaternion(std::vector<float> &q);
        /** normalizes quaternion to unit length. Here the computation is
         * done in place and the parameter is changed !
         * @param q Quaternion storing quaternion
         * @return pointer to result array */
        static double* normalizeQuaternion(Quaternion q);
        /** rotates a vector using a given unit quaternion. It does
         * not normalize the quaternion or check for unit length ! This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q float[4] storing quaternion
         * @param v float[3] storing vector
         * @param vResult float[3] where the result is stored
         * @return pointer to result array */
        static float* rotateVector(const float* q, const float* v, float* vResult);
        /** rotates a vector using a given unit quaternion. It does
         * not normalize the quaternion or check for unit length ! This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q vector<float> (assumes length 4) storing quaternion
         * @param v vector<float> (assumes length 3) storing vector
         * @param vResult vector<float> where the result is stored
         * @return pointer to result array */
        static std::vector<float>& rotateVector(const std::vector<float> &q, const std::vector<float> &v, std::vector<float> &vResult);

        /** computes the determinant of a 3x3 matrix.
         * @param matrix the 3x3 matrix to use
         * @return determinant of the matrix */
        static float determinant( const float matrix[3][3] );

        /** computes the vector and angle representation of a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q float[4] storing the quaternion
         * @param axisa float[4] storing the vector and angle of the given quaternion
         * @return pointer to result array (axisa) */
        static float* quaternionToAxisAngle(const float *q, float *axisa);

        /** computes the vector and angle representation of a quaternion. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q std::vector<float> storing the quaternion
         * @param axisa std::vector<float> storing the vector and angle of the given quaternion
         */
        static void  quaternionToAxisAngle(std::vector<float> &q, std::vector<float> &axisa);


        void translationQuaternionTo4x4Matrix(std::vector<float>& pos, std::vector<float>& quat, MathUtils::Matrix4x4& mat);

        /**
         * computes the angle between two n-dimensional vectors.
         * @param v1 float[n] first vector
         * @param v2 float[n] second vector
         * @param dim dimension n of the two vectors
         * @return angle between the two vectors
         */
        static double angle( const float * v1, const float * v2, const int dim );
        /**
         * computes the angle between two n-dimensional vectors.
         * @param v1 vector<float> first vector
         * @param v2 vector<float> second vector
         * @param dim dimension n of the two vectors
         * @return angle between the two vectors
         */
        static double angle( const std::vector<float> &v1, const std::vector<float> & v2, const int dim );
        /**
         * computes the dot product between two n-dimensional vectors.
         * @param v1 float[n] first vector
         * @param v2 float[n] second vector
         * @param dim dimension n of the two vectors
         * @return dot product between the two vectors
         */
        static double dot( const float * v1, const float * v2, const int dim );
        /**
         * computes the dot product between two n-dimensional vectors.
         * @param v1 vector<float> first vector
         * @param v2 vector<float> second vector
         * @param dim dimension n of the two vectors
         * @return dot product between the two vectors
         */
        static double dot( const std::vector<float> &v1, const std::vector<float> &v2, const int dim );
        /**
         * computes the spherical linear interpolation between two quaternions. The
         * formulas used are probably only stable for t in [0,1], but it can be
         * used to calculate extrapolations as well. Moreover the slerp is always
         * computed on the shorter path between the quaternions. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q1 float[4] first quaternion
         * @param q2 float[4] second quaternion
         * @param t interpolation parameter
         * @param qResult float[4] stores result
         * @return pointer to result = qResult
         */
        static float * slerp( const float * q1, const float *q2, const float t, float * qResult );
        /**
         * computes the spherical linear interpolation between two quaternions. The
         * formulas used are probably only stable for t in [0,1], but it can be
         * used to calculate extrapolations as well. Moreover the slerp is always
         * computed on the shorter path between the quaternions. This method
         * operates directly on the arguments. Therefore using the same pointers
         * for several arguments will produce wrong results !
         * @param q1 vector<float> (assumes length 4) first quaternion
         * @param q2 vector<float> (assumes length 4) second quaternion
         * @param t interpolation parameter
         * @param qResult vector<float> stores result
         * @return reference to result = qResult
         */
        static std::vector<float>& slerp( const std::vector<float> &q1, const std::vector<float> &q2, const float t, std::vector<float> &qResult );

        static void quaternionToMatrix( Quaternion &q, Matrix4x4 &m);
        static void matrixToQuaternion( Matrix4x4& matrix, Quaternion& qResult);
        
        static void matrixMultiply(const Matrix4x4 m1, const Matrix4x4 m2, Matrix4x4 &m);
        static void matrixMultiply(const Matrix3x3 m1, const Matrix3x3 m2, Matrix3x3 &m);

        /// mirror at the x-z-plane
        static const Matrix4x4 matrix4x4_flipY;
        /// identity matrix
        static const Matrix4x4 matrix4x4_identity;

        /*****************************************************************************
         * MatrixToEuler - convert a column matrix to euler angles
         *
         *  input:
         *	- vector to hold euler angles
         *  - src column matrix
         *  Vector3  	angles    :      Holds outgoing roll, pitch, yaw
         *  Matrix4x4 	colMatrix :      Holds incoming rotation
         *
         *  output:
         *  - euler angles in radians in the range -pi to pi;
         *  vec[0] = yaw, vec[1] = pitch, vec[2] = roll
         *  yaw is rotation about Z axis, pitch is about Y, roll -> X rot.
         *
         * notes:
         *	- originally written by Gary Bishop
         *
         *****************************************************************************/
        static void MatrixToEuler(Vector3 angles, const Matrix4x4 colMatrix);

        /// the nice constant Pi
        static const double Pi;

        /// the also nice constant e
        static const double E;

        /// another nice constant to transform grad to radiants
        static const double GradToRad;
    };

} // namespace ot

#endif

/* 
 * ------------------------------------------------------------
 *   End of MathUtils.h
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
