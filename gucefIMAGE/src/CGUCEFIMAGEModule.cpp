/*
 * Copyright (C) Dinand Vanvelzen. 2002 - 2005.  All rights reserved.
 *
 * All source code herein is the property of Dinand Vanvelzen. You may not sell
 * or otherwise commercially exploit the source or things you created based on
 * the source.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL DINAND VANVELZEN BE LIABLE FOR ANY SPECIAL, INCIDENTAL, 
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF 
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT 
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

//#ifndef CIMGCODECMANAGER_H
//#include "CIMGCodecManager.h"      /* header for the main image codec manager */
//#define CIMGCODECMANAGER_H
//#endif /* CIMGCODECMANAGER_H ? */

#include "CGUCEFIMAGEModule.h"  /* definition of the class implemented here */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace IMAGE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CGUCEFIMAGEModule::CGUCEFIMAGEModule( void )
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::CGUCEFIMAGEModule( void )" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFIMAGEModule::CGUCEFIMAGEModule( const CGUCEFIMAGEModule& src )
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::CGUCEFIMAGEModule( const CGUCEFIMAGEModule& src )" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFIMAGEModule::~CGUCEFIMAGEModule()
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::~CGUCEFIMAGEModule()" );
        
        /* dummy, should never be used */
}

/*-------------------------------------------------------------------------*/

CGUCEFIMAGEModule&
CGUCEFIMAGEModule::operator=( const CGUCEFIMAGEModule& src )
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::operator=( const CGUCEFIMAGEModule& src )" );
        
        /* dummy, should never be used */
        return *this;
}
        
/*-------------------------------------------------------------------------*/        

bool 
CGUCEFIMAGEModule::Load( void )
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::Load( void )" );
        
        /* simply instantiate our codec manager when the module is loaded */
        //CIMGCodecManager::Instance();
        return true;
}

/*-------------------------------------------------------------------------*/
        
bool 
CGUCEFIMAGEModule::Unload( void )
{
        DEBUGOUTPUT( "CGUCEFIMAGEModule::Unload( void )" );
        
        return true;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace IMAGE */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
