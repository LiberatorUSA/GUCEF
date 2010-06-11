/*
 *  gucefMATH: GUCEF module providing basic math classes
 *  Copyright (C) 2002 - 2007.  Dinand Vanvelzen
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 
 */

#ifndef GUCEF_MATH_CTPOINT3D_H
#define GUCEF_MATH_CTPOINT3D_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include "common.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MATH {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename T >
class CTPoint3D
{
    friend class Vector;

    //----------------------------------------------------------
    // Lots of Constructors (add more as needed)
    CTPoint3D( void );

    // 3D Point
    Point( int a, int b, int c) {
    dimn=3; x=a; y=b; z=c; err=Enot; }
    Point( double a, double b, double c) {
    dimn=3; x=a; y=b; z=c; err=Enot; }
    // n-dim Point
    Point( int n, int a[]);
    Point( int n, double a[]);
    // Destructor
    ~Point() {};

    //----------------------------------------------------------
    // Input/Output streams
    friend istream& operator>>( istream&, Point&);
    friend ostream& operator<<( ostream&, Point);

    //----------------------------------------------------------
    // Comparison (dimension must match, or not)
    int operator==( Point);
    int operator!=( Point);

    //----------------------------------------------------------
    // Point and Vector Operations (always valid) 
    Vector operator-( Point);       // Vector difference
    Point  operator+( Vector);      // +translate
    Point  operator-( Vector);      // -translate
    Point& operator+=( Vector);     // inc translate
    Point& operator-=( Vector);     // dec translate

    //----------------------------------------------------------
    // Point Scalar Operations (convenient but often illegal)
    // using any type of scalar (int, float, or double)
    //    are not valid for points in general,
    //    unless they are 'affine' as coeffs of 
    //    a sum in which all the coeffs add to 1,
    //    such as: the sum (a*P + b*Q) with (a+b == 1).
    //    The programmer must enforce this (if they want to).

    // Scalar Multiplication
    friend Point operator*( int, Point);
    friend Point operator*( double, Point);
    friend Point operator*( Point, int);
    friend Point operator*( Point, double);
    // Scalar Division
    friend Point operator/( Point, int);
    friend Point operator/( Point, double);

    //----------------------------------------------------------
    // Point Addition (also convenient but often illegal)
    //    is not valid unless part of an affine sum.
    //    The programmer must enforce this (if they want to).
    friend Point operator+( Point, Point);     // add points

    // Affine Sum
    // Returns weighted sum, even when not affine, but...
    // Tests if coeffs add to 1.  If not, sets: err = Esum.
    friend Point asum( int, int[], Point[]);
    friend Point asum( int, double[], Point[]);

    //----------------------------------------------------------
    // Point Relations
    friend double d( Point, Point);         // Distance
    friend double d2( Point, Point);        // Distance^2
    double isLeft( Point, Point);           // 2D only
    double Area( Point, Point); 		// any dim for triangle PPP

    // Collinearity Conditions (any dim n)
    boolean isOnLine( Point, Point, char);  // is On line (char= flag)
    boolean isOnLine( Point, Point);        // is On line (flag= all)
    boolean isBefore( Point, Point);        // is On line (flag= before)
    boolean isBetween( Point, Point);       // is On line (flag= between)
    boolean isAfter( Point, Point);         // is On line (flag= after)
    boolean isOnRay( Point, Point);         // is On line (flag= between|after)

    //----------------------------------------------------------
    // Error Handling
    void  clerr() { err = Enot;}            // clear error
    int   geterr() { return err;}           // get error
    char* errstr();                         // return error string
    
    private:
    
    T m_x;
    T m_y;
    T m_z;
    
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      UTILITIES                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

template < typename T >
CTPoint3D< T >::CTPoint3D( void )
    : m_x( 0 )  ,
      m_y( 0 )  ,
      m_z( 0 )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MATH */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MATH_CTVECTOR3D_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 02-03-2007 :
        - Dinand: re-added this header

---------------------------------------------------------------------------*/
