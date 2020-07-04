/*
 *  gucefCORE: GUCEF module providing O/S abstraction and generic solutions
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

#ifndef GUCEF_CORE_CVALUELIST_H
#define GUCEF_CORE_CVALUELIST_H

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#include <map>
#include <vector>

#ifndef GUCEF_CORE_EXCEPTIONMACROS_H
#include "ExceptionMacros.h"
#define GUCEF_CORE_EXCEPTIONMACROS_H
#endif /* GUCEF_CORE_EXCEPTIONMACROS_H ? */

#ifndef GUCEF_CORE_CDVSTRING_H
#include "CDVString.h"
#define GUCEF_CORE_CDVSTRING_H
#endif /* GUCEF_CORE_CDVSTRING_H ? */

#ifndef GUCEF_CORE_CICONFIGURABLE_H
#include "CIConfigurable.h"
#define GUCEF_CORE_CICONFIGURABLE_H
#endif /* GUCEF_CORE_CICONFIGURABLE_H ? */

#ifndef GUCEF_CORE_MACROS_H
#include "gucefCORE_macros.h"     /* often used gucef macros */
#define GUCEF_CORE_MACROS_H
#endif /* GUCEF_CORE_MACROS_H ? */

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace CORE {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

class GUCEF_CORE_PUBLIC_CPP CValueList : public CIConfigurable
{
    public:

    typedef std::vector< CString >              TStringVector;
    typedef std::map< CString, TStringVector >  TValueMap;

    CValueList( void );

    CValueList( const CValueList& src );

    CValueList& operator=( const CValueList& src );

    virtual ~CValueList();

    /**
     *  Returns the first value associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    CString& operator[]( const CString& key );

    /**
     *  Returns the first value associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    const CString& operator[]( const CString& key ) const;

    void SetMultiple( int argc    ,
                      char** argv );

    void SetMultiple( const CString& keyandvalue ,
                      const char seperator       );

    void Set( const CString& keyAndValue );

    void Set( const CString& key   ,
              const CString& value );

    /**
     *  Returns the first value associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    CString& GetValue( const CString& key );

    /**
     *  Returns the first value associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    const CString& GetValue( const CString& key ) const;

    /**
     *  Returns the first value associated with the
     *  given key. This GetValue version differs from the others in
     *  that no exception will be thrown if the key does not exist
     *  instead it will return an empty string.
     */
    CString GetValueAlways( const CString& key ) const;

    /**
     *  Returns the first value associated with the
     *  given key. This GetValue version differs from the others in
     *  that no exception will be thrown if the key does not exist
     *  instead it will return an empty string.
     */
    CString GetValueAlways( const CString& key          ,
                            const CString& defaultValue ) const;

    /**
     *  Returns the first value associated with the
     *  given key index.
     *
     *  @exception EIndexOutOfRange thrown if the given key index is invalid
     */
    CString& GetValue( const UInt32 index );

    /**
     *  Returns the first value associated with the
     *  given key index.
     *
     *  @exception EIndexOutOfRange thrown if the given key index is invalid
     */
    const CString& GetValue( const UInt32 index ) const;

    /**
     *  Returns the value vector associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    TStringVector& GetValueVector( const CString& key );

    /**
     *  Returns the value vector associated with the
     *  given key if any. Otherwise returns an empty list and will not throw an exception
     */
    TStringVector GetValueVectorAlways( const CString& key ) const;

    /**
     *  Returns the value vector associated with the
     *  given key if any. Otherwise returns an empty list and will not throw an exception
     *  Aside from returning the multiple entries for the same key (if permitted) it will also
     *  perform sub string parsing for values per value entry
     */
    TStringVector GetValueVectorAlways( const CString& key, char valueSepChar ) const;

    TStringVector GetValueVectorAlways( const CString& key, char valueSepChar, const TStringVector& default ) const;

    /**
     *  Returns all keys matching the key wildcard search 
     *  Will not throw an exception
     */
    TStringVector GetKeysWithWildcardKeyMatch( const CString& searchStr  , 
                                               char wildCardChar = '*'   ,
                                               bool caseSensitive = true ) const;

    /**
     *  Returns the value vector associated with the
     *  given key.
     *
     *  @exception EUnknownKey thrown if the given key is unknown
     */
    const TStringVector& GetValueVector( const CString& key ) const;

    /**
     *  Returns the value vector associated with the
     *  given index.
     *
     *  @exception EIndexOutOfRange thrown if the given index is out of bounds
     */
    const TStringVector& GetValueVector( const UInt32 index ) const;

    CString GetPair( const CString& key ) const;

    CString GetPair( const UInt32 index ) const;
    
    CString GetAllPairs( const UInt32 index, const CString& seperatorStr ) const;
    
    CString GetAllPairs( const CString& seperatorStr                , 
                         bool envelopElements = false               , 
                         const CString& envelopStr = CString::Empty ) const;

    /**
     *  Returns the key associated with the given index.
     *
     *  @exception EIndexOutOfRange thrown if the given key index is invalid
     */
    const CString& GetKey( const UInt32 index ) const;

    bool HasKey( const CString& key ) const;

    void Delete( const CString& key );

    void DeleteAll( void );

    UInt32 GetCount( void ) const;

    void SetAllowDuplicates( const bool allowDuplicates );

    bool GetAllowDuplicates( void ) const;

    void SetAllowMultipleValues( const bool allowMultipleValues );

    bool GetAllowMultipleValues( void ) const;

    void Clear( void );

    /**
     *      Attempts to store the given tree in the value list
     */
    virtual bool SaveConfig( CDataNode& tree ) const;

    /**
     *      Attempts to load data 
     *
     *      @param treeroot pointer to the node that is to act as root of the data tree
     *      @return whether building the tree from the given file was successfull.
     */
    virtual bool LoadConfig( const CDataNode& treeroot );

    /**
     *  When loading from config data this will limit the scope
     *  of the information loaded to just the namespace given
     */
    void SetConfigNamespace( const CString& configNamespace );

    const CString& GetConfigNamespace( void ) const;

    /**
     *  When loading from config data this will limit the scope
     *  of the information loaded to just the keys that are 
     *  namespaced with by having the exact 'configKeyNamespace' prefix
     */
    void SetConfigKeyNamespace( const CString& configKeyNamespace );

    const CString& GetConfigKeyNamespace( void ) const;
    
    TValueMap::const_iterator GetDataBeginIterator( void ) const;
    
    TValueMap::const_iterator GetDataEndIterator( void ) const;

    virtual const CString& GetClassTypeName( void ) const;
    
    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_CORE_PUBLIC_CPP, EUnknownKey );
    GUCEF_DEFINE_MSGEXCEPTION( GUCEF_CORE_PUBLIC_CPP, EIndexOutOfRange );

    private:

    TValueMap m_list;
    bool m_allowDuplicates;
    bool m_allowMultipleValues;
    CString m_configNamespace;
    CString m_configKeyNamespace;
};

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

}; /* namespace CORE */
}; /* namespace GUCEF */

/*-------------------------------------------------------------------------*/

#endif /* GUCEF_CORE_CVALUELIST_H */

/*-------------------------------------------------------------------------//
//                                                                         //
//      Info & Changes                                                     //
//                                                                         //
//-------------------------------------------------------------------------//

- 18-08-2007 :
        - Updated to use STL containers and added the ability to map multiple
          values to a single key.
        - Instead of empty strings an exception will now be thrown if an invalid
          key name or index is used to access an entry.
- 21-09-2005 :
        - initial version

--------------------------------------------------------------------------*/
