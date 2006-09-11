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
 
#ifndef GUCEF_MT_CACTIVEOBJECT_H
#define GUCEF_MT_CACTIVEOBJECT_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_MT_ETYPES_H
#include "gucefMT_ETypes.h"             /* Simple types used */
#define GUCEF_MT_ETYPES_H
#endif /* GUCEF_MT_ETYPES_H */

#ifndef GUCEF_MT_GUCEFMT_MACROS_H
#include "gucefMT_macros.h"             /* often used gucef macros */
#define GUCEF_MT_GUCEFMT_MACROS_H
#endif /* GUCEF_MT_GUCEFMT_MACROS_H ? */ 

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace MT {

/*-------------------------------------------------------------------------//
//                                                                         //
//      TYPES                                                              //
//                                                                         //
//-------------------------------------------------------------------------*/

struct TThreadData;

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEFMT_EXPORT_CPP CActiveObject
{
        public:
        
        CActiveObject( void );
        
        CActiveObject( const CActiveObject& src );
        
        CActiveObject( void* taskdata    ,
                       UInt32 delay = 10 );
        
        virtual ~CActiveObject();
        
        bool IsActive( void ) const;

        void Activate( void );

        void Activate( void* taskdata );
        
        void Deactivate( bool force ); 
        
        void Pause( void ); 
        
        void Resume( void );      
        
        protected:
        
        virtual bool OnTaskStart( void* taskdata ) = 0;
        
        virtual bool OnTaskCycle( void* taskdata ) = 0;
        
        virtual void OnTaskEnd( void* taskdata ) = 0;
        
        void* GetTaskData( void ) const;
        
        private:
        CActiveObject& operator=( const CActiveObject& src );        
        static UInt32 OnActivate( void* thisobject );
        
        struct SThreadData* _td;
        void* _taskdata;
        UInt32 _delay;
        bool _suspend;
        bool _active;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace MT */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_MT_CACTIVEOBJECT_H ? */        

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 07-02-2005 :
       - Initial implementation.

---------------------------------------------------------------------------*/