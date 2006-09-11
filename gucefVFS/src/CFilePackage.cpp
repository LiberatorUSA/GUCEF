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

#ifndef CFPCODEC_H
#include "CFPCodec.h"
#define CFPCODEC_H
#endif /* CFPCODEC_H ? */

#include "CFilePackage.h"    /* definition of the class implemented here */

#ifdef ACTIVATE_MEMORY_MANAGER
  #ifndef GUCEF_NEW_ON_H
  #include "gucef_new_on.h"   /* Use the GUCEF memory manager instead of the standard manager ? */
  #define GUCEF_NEW_ON_H
  #endif /* GUCEF_NEW_ON_H ? */
#endif /* ACTIVATE_MEMORY_MANAGER ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

GUCEF_NAMESPACE_BEGIN
VFS_NAMESPACE_BEGIN

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CFilePackage::CFilePackage( const CORE::CString& abspackfilepath ,
                            const CORE::CString& packfilepath    ,
                            CFPCodec* codec                      )
        : _packabspath( abspackfilepath )                            
{TRACE;
        if ( packfilepath.Length() )
        {
              /*  for ( UInt32 i=packfilepath.Length()-1; i>0; --i )
                {        
                        if ( ( packfilepath[ i ] == '\\' ) ||
                             ( packfilepath[ i ] == '/' )
                        {
                                _packpath.Set( packfilepath.C_String() ,
                                               packfilepath.Length()-i );
                                               
                                _packfilename = packfilepath.C_String()+(packfilepath.Length()-i);
                                return;
                        }
                }           */
                _packfilename = packfilepath;               
        }                
}

/*--------------------------------------------------------------------------*/                            

CFilePackage::CFilePackage( void )
{TRACE;
        /* dummy, do not use */
}

/*--------------------------------------------------------------------------*/

CFilePackage::CFilePackage( const CFilePackage& src )
{TRACE;
        /* dummy, do not use */
}

/*--------------------------------------------------------------------------*/

CFilePackage::~CFilePackage()
{TRACE;
}

/*--------------------------------------------------------------------------*/

CFilePackage&
CFilePackage::operator=( const CFilePackage& src )
{TRACE;
        /* dummy, do not use */
        return *this;
}

/*--------------------------------------------------------------------------*/

CORE::CString 
CFilePackage::GetAbsolutePackPath( void ) const
{TRACE;
        return _packabspath;
}

/*--------------------------------------------------------------------------*/

CORE::CString 
CFilePackage::GetPackPath( void ) const
{TRACE;
        return _packpath;
}
        
/*--------------------------------------------------------------------------*/        
        
CORE::CString 
CFilePackage::GetPackFilename( void ) const
{TRACE;
        return _packfilename;
}

/*--------------------------------------------------------------------------*/

CVFSHandle* 
CFilePackage::GetFile( const CORE::CString& file ,
                       UInt32& errorcode         )
{TRACE;
        
        
                  return NULL;
        CVFSHandle* fh ;/*= _codec->GetFile( _access    ,
                                          file       ,
                                          &_packdata ,
                                          errorcode  ); */
        if ( fh )
        {
                //_loadcount
        }
        return fh;                                            
}

/*--------------------------------------------------------------------------*/                       
        
CVFSHandle* 
CFilePackage::GetWriteableFile( const CORE::CString& file ,
                                UInt32& errorcode         )
{TRACE;
       /* return codec->GetWriteableFile( _access   ,
                                        file      ,
                                        errorcode );         */
        return NULL;                                        
}

/*--------------------------------------------------------------------------*/                                

void 
CFilePackage::Unload( CVFSHandle* )
{TRACE;
        
}

/*--------------------------------------------------------------------------*/

void 
CFilePackage::SetMemloadSize( UInt32 bytesize )
{TRACE;
}

/*--------------------------------------------------------------------------*/
        
UInt32 
CFilePackage::GetMemloadSize( void ) const
{TRACE;
        return 0;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

VFS_NAMESPACE_END
GUCEF_NAMESPACE_END

/*-------------------------------------------------------------------------*/
