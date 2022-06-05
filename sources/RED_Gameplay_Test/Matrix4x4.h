#ifndef REACTPHYSICS3D_MATRIX4X4_H
#define REACTPHYSICS3D_MATRIX4X4_H

// Libraries
#include <cassert>
#include <reactphysics3d/mathematics/Quaternion.h>

/// ReactPhysics3D namespace
namespace reactphysics3d
{
    // Class Matrix4x4
    /**
     * This class represents a 4x4 matrix.
     */
    inline decimal& Get(Quaternion& q, int i)
    {
        assert(i >= 0 && i < 4);
        if(i == 0) return q.x;
        if(i == 1) return q.y;
        if(i == 2) return q.z;

        return q.w;
    }

    inline const decimal& Get(const Quaternion& q, int i)
    {
        assert(i >= 0 && i < 4);
        if(i == 0) return q.x;
        if(i == 1) return q.y;
        if(i == 2) return q.z;

        return q.w;
    }
    
    class Matrix4x4 {

        private :

            // -------------------- Attributes -------------------- //

            /// Rows of the matrix;
            Quaternion mRows[4];

        public :

            // -------------------- Methods -------------------- //

            /// Constructor
            Matrix4x4();

        /// Constructor
        Matrix4x4(decimal value);

        /// Constructor
        Matrix4x4(decimal a1, decimal a2, decimal a3, decimal a4, decimal b1, decimal b2, decimal b3, decimal b4,
                  decimal c1, decimal c2, decimal c3, decimal c4, decimal d1, decimal d2, decimal d3, decimal d4);

        /// Set all the values in the matrix
        void setAllValues(decimal a1, decimal a2, decimal a3, decimal a4, decimal b1, decimal b2, decimal b3, decimal b4,
                  decimal c1, decimal c2, decimal c3, decimal c4, decimal d1, decimal d2, decimal d3, decimal d4);

        /// Set the matrix to zero
        void setToZero();

        /// Return a column
        Quaternion getColumn(int i) const;

        /// Return a row
        Quaternion getRow(int i) const;

        /// Return the transpose matrix
        Matrix4x4 getTranspose() const;

        /// Return the determinant of the matrix
        decimal getDeterminant() const;

        /// Return the trace of the matrix
        decimal getTrace() const;

        /// Return the inverse matrix
        Matrix4x4 getInverse() const;

        /// Return the inverse matrix
        Matrix4x4 getInverse(decimal determinant) const;

        /// Return the matrix with absolute values
        Matrix4x4 getAbsoluteMatrix() const;

        /// Set the matrix to the identity matrix
        void setToIdentity();

        /// Return the 4x4 identity matrix
        static Matrix4x4 identity();

        /// Return the 4x4 zero matrix
        static Matrix4x4 zero();

        /// Return a skew-symmetric matrix using a given vector that can be used
        /// to compute cross product with another vector using matrix multiplication
        static Matrix4x4 computeSkewSymmetricMatrixForCrossProduct(const Quaternion& vector);

        /// Overloaded operator for addition
        friend Matrix4x4 operator+(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

        /// Overloaded operator for substraction
        friend Matrix4x4 operator-(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

        /// Overloaded operator for the negative of the matrix
        friend Matrix4x4 operator-(const Matrix4x4& matrix);

        /// Overloaded operator for multiplication with a number
        friend Matrix4x4 operator*(decimal nb, const Matrix4x4& matrix);

        /// Overloaded operator for multiplication with a matrix
        friend Matrix4x4 operator*(const Matrix4x4& matrix, decimal nb);

        /// Overloaded operator for matrix multiplication
        friend Matrix4x4 operator*(const Matrix4x4& matrix1, const Matrix4x4& matrix2);

        /// Overloaded operator for multiplication with a vector
        friend Quaternion operator*(const Matrix4x4& matrix, const Quaternion& vector);

        /// Overloaded operator for equality condition
        bool operator==(const Matrix4x4& matrix) const;

        /// Overloaded operator for the is different condition
        bool operator!= (const Matrix4x4& matrix) const;

        /// Overloaded operator for addition with assignment
        Matrix4x4& operator+=(const Matrix4x4& matrix);

        /// Overloaded operator for substraction with assignment
        Matrix4x4& operator-=(const Matrix4x4& matrix);

        /// Overloaded operator for multiplication with a number with assignment
        Matrix4x4& operator*=(decimal nb);

        /// Overloaded operator to read element of the matrix.
        const Quaternion& operator[](int row) const;

        /// Overloaded operator to read/write element of the matrix.
        Quaternion& operator[](int row);

        /// Return the string representation
        std::string to_string() const;
    };

    // Constructor of the class Matrix4x4
    RP3D_FORCE_INLINE Matrix4x4::Matrix4x4() {
        // Initialize all values in the matrix to zero
        setAllValues(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    // Constructor
    RP3D_FORCE_INLINE Matrix4x4::Matrix4x4(decimal value) {
        setAllValues(value, value, value, value, value, value,  value, value, value,value, value, value, value, value, value, value);
    }

    // Constructor with arguments
    RP3D_FORCE_INLINE Matrix4x4::Matrix4x4(decimal a1, decimal a2, decimal a3, decimal a4, decimal b1, decimal b2, decimal b3, decimal b4, decimal c1, decimal c2, decimal c3, decimal c4, decimal d1, decimal d2, decimal d3, decimal d4) {
        // Initialize the matrix with the values
        setAllValues(a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4);
    }

    // Method to set all the values in the matrix
    RP3D_FORCE_INLINE void Matrix4x4::setAllValues(decimal a1, decimal a2, decimal a3, decimal a4, decimal b1, decimal b2, decimal b3, decimal b4, decimal c1, decimal c2, decimal c3, decimal c4, decimal d1, decimal d2, decimal d3, decimal d4) {
        Get(mRows[0], 0) = a1; Get(mRows[0], 1) = a2; Get(mRows[0], 2) = a3; Get(mRows[0], 3) = a4;
        Get(mRows[1], 0) = b1; Get(mRows[1], 1) = b2; Get(mRows[1], 2) = b3; Get(mRows[0], 3) = b4;
        Get(mRows[2], 0) = c1; Get(mRows[2], 1) = c2; Get(mRows[2], 2) = c3; Get(mRows[0], 3) = c4;
        Get(mRows[3], 0) = d1; Get(mRows[3], 1) = d2; Get(mRows[3], 2) = d3; Get(mRows[3], 3) = d4;
    }

    // Set the matrix to zero
    RP3D_FORCE_INLINE void Matrix4x4::setToZero() {
        mRows[0].setToZero();
        mRows[1].setToZero();
        mRows[2].setToZero();
        mRows[3].setToZero();
    }

    // Return a column
    RP3D_FORCE_INLINE Quaternion Matrix4x4::getColumn(int i) const {
        assert(i>= 0 && i<3);
        return Quaternion(Get(mRows[0], i), Get(mRows[1], i), Get(mRows[2], i), Get(mRows[3], i));
    }

    // Return a row
    RP3D_FORCE_INLINE Quaternion Matrix4x4::getRow(int i) const {
        assert(i>= 0 && i<3);
        return mRows[i];
    }

    // Return the transpose matrix
    RP3D_FORCE_INLINE Matrix4x4 Matrix4x4::getTranspose() const {

        // Return the transpose matrix
        return Matrix4x4(Get(mRows[0], 0), Get(mRows[1], 0), Get(mRows[2], 0), Get(mRows[3], 0),
                         Get(mRows[0], 1), Get(mRows[1], 1), Get(mRows[2], 1), Get(mRows[3], 1),
                         Get(mRows[0], 2), Get(mRows[1], 2), Get(mRows[2], 2), Get(mRows[3], 2),
                         Get(mRows[0], 3), Get(mRows[1], 3), Get(mRows[2], 3), Get(mRows[3], 3));
    }

    // Set the matrix to the identity matrix
    RP3D_FORCE_INLINE void Matrix4x4::setToIdentity() {
        Get(mRows[0], 0) = 1.0; Get(mRows[0], 1) = 0.0; Get(mRows[0], 2) = 0.0; Get(mRows[0], 3) = 0.0;
        Get(mRows[1], 0) = 0.0; Get(mRows[1], 1) = 1.0; Get(mRows[1], 2) = 0.0; Get(mRows[1], 3) = 0.0;
        Get(mRows[2], 0) = 0.0; Get(mRows[2], 1) = 0.0; Get(mRows[2], 2) = 1.0; Get(mRows[2], 3) = 0.0;
        Get(mRows[3], 0) = 0.0; Get(mRows[3], 1) = 0.0; Get(mRows[3], 2) = 0.0; Get(mRows[3], 3) = 1.0;
    }

    // Return the 4x4 identity matrix
    RP3D_FORCE_INLINE Matrix4x4 Matrix4x4::identity() {
        return Matrix4x4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    }

    // Return the 4x4 zero matrix
    RP3D_FORCE_INLINE Matrix4x4 Matrix4x4::zero() {
        return Matrix4x4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }

    // Return the matrix with absolute values
    RP3D_FORCE_INLINE Matrix4x4 Matrix4x4::getAbsoluteMatrix() const {
        return Matrix4x4(std::abs(Get(mRows[0], 0)), std::abs(Get(mRows[0], 1)), std::abs(Get(mRows[0], 2)), std::abs(Get(mRows[0], 3)),
                         std::abs(Get(mRows[1], 0)), std::abs(Get(mRows[1], 1)), std::abs(Get(mRows[1], 2)), std::abs(Get(mRows[1], 3)),
                         std::abs(Get(mRows[2], 0)), std::abs(Get(mRows[2], 1)), std::abs(Get(mRows[2], 2)), std::abs(Get(mRows[2], 3)),
                         std::abs(Get(mRows[3], 0)), std::abs(Get(mRows[3], 1)), std::abs(Get(mRows[3], 2)), std::abs(Get(mRows[3], 3)));
    }

    // Overloaded operator for addition
    RP3D_FORCE_INLINE Matrix4x4 operator+(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
        return Matrix4x4(Get(matrix1.mRows[0], 0) + Get(matrix2.mRows[0], 0), Get(matrix1.mRows[0], 1) +
                         Get(matrix2.mRows[0], 1), Get(matrix1.mRows[0], 2) + Get(matrix2.mRows[0], 2),
                         Get(matrix1.mRows[0], 3) + Get(matrix2.mRows[0], 3), Get(matrix1.mRows[1], 0) +
                         Get(matrix2.mRows[1], 0), Get(matrix1.mRows[1], 1) + Get(matrix2.mRows[1], 1),
                         Get(matrix1.mRows[1], 2) + Get(matrix2.mRows[1], 2), Get(matrix1.mRows[1], 3) +
                         Get(matrix2.mRows[1], 3), Get(matrix1.mRows[2], 0) + Get(matrix2.mRows[2], 0),
                         Get(matrix1.mRows[2], 1) + Get(matrix2.mRows[2], 1), Get(matrix1.mRows[2], 2) +
                         Get(matrix2.mRows[2], 2), Get(matrix1.mRows[2], 3) + Get(matrix2.mRows[2], 3),
                         Get(matrix1.mRows[3], 0) + Get(matrix2.mRows[3], 0), Get(matrix1.mRows[3], 1) +
                         Get(matrix2.mRows[3], 1), Get(matrix1.mRows[3], 2) + Get(matrix2.mRows[4], 2),
                         Get(matrix1.mRows[4], 3) + Get(matrix2.mRows[4], 3));
    }

    // Overloaded operator for substraction
    RP3D_FORCE_INLINE Matrix4x4 operator-(const Matrix4x4& matrix1, const Matrix4x4& matrix2) {
        return Matrix4x4(Get(matrix1.mRows[0], 0) - Get(matrix2.mRows[0], 0), Get(matrix1.mRows[0], 1) -
                         Get(matrix2.mRows[0], 1), Get(matrix1.mRows[0], 2) - Get(matrix2.mRows[0], 2),
                         Get(matrix1.mRows[0], 3) - Get(matrix2.mRows[0], 3), Get(matrix1.mRows[1], 0) -
                         Get(matrix2.mRows[1], 0), Get(matrix1.mRows[1], 1) - Get(matrix2.mRows[1], 1),
                         Get(matrix1.mRows[1], 2) - Get(matrix2.mRows[1], 2), Get(matrix1.mRows[1], 3) -
                         Get(matrix2.mRows[1], 3), Get(matrix1.mRows[2], 0) - Get(matrix2.mRows[2], 0),
                         Get(matrix1.mRows[2], 1) - Get(matrix2.mRows[2], 1), Get(matrix1.mRows[2], 2) -
                         Get(matrix2.mRows[2], 2), Get(matrix1.mRows[2], 3) - Get(matrix2.mRows[2], 3),
                         Get(matrix1.mRows[3], 0) - Get(matrix2.mRows[3], 0), Get(matrix1.mRows[3], 1) -
                         Get(matrix2.mRows[3], 1), Get(matrix1.mRows[3], 2) - Get(matrix2.mRows[4], 2),
                         Get(matrix1.mRows[4], 3) - Get(matrix2.mRows[4], 3));
    }
}
#endif