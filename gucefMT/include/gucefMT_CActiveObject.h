/*
 *  gucefMT: GUCEF module providing multithreading solutions
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

struct SThreadData;

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

        virtual ~CActiveObject();

        bool IsActive( void ) const;

        void Activate( void* taskdata = NULL               ,
                       const UInt32 cycleDelay = 10        ,
                       const UInt32 minimalCycleDelta = 10 );

        void Deactivate( bool force );

        void Pause( void );

        void Resume( void );
        
        UInt32 GetThreadID( void ) const;

        protected:

        virtual bool OnTaskStart( void* taskdata ) = 0;

        virtual bool OnTaskCycle( void* taskdata ) = 0;

        virtual void OnTaskEnd( void* taskdata ) = 0;

        void* GetTaskData( void ) const;

        private:
        CActiveObject& operator=( const CActiveObject& src );
        static UInt32 GUCEF_CALLSPEC_STD_PREFIX OnActivate( void* thisobject ) GUCEF_CALLSPEC_STD_SUFFIX;

        struct SThreadData* _td;
        void* _taskdata;
        UInt32 _delay;
        Float64 m_minimalCycleDelta;
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
