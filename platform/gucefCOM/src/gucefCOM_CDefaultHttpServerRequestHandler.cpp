/*
 *  gucefCOM: GUCEF module providing communication implementations 
 *  for standardized protocols
 *
 *  Copyright (C) 1998 - 2020.  Dinand Vanvelzen
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_CORE_LOGGING_H
#include "gucefCORE_Logging.h"
#define GUCEF_CORE_LOGGING_H
#endif /* GUCEF_CORE_LOGGING_H ? */

#include "gucefCOM_CDefaultHttpServerRequestHandler.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      IMPLEMENTATION                                                     //
//                                                                         //
//-------------------------------------------------------------------------*/

CDefaultHttpServerRequestHandler::CDefaultHttpServerRequestHandler( CIHTTPServerRouterController* routerController /* = GUCEF_NULL */ )
    : CIHttpServerRequestHandler()
    , m_routerController( routerController )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDefaultHttpServerRequestHandler::CDefaultHttpServerRequestHandler( const CDefaultHttpServerRequestHandler& src )
    : CIHttpServerRequestHandler( src )
    , m_routerController( src.m_routerController )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CDefaultHttpServerRequestHandler::~CDefaultHttpServerRequestHandler()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::OnRead( const CHttpRequestData& request )
{GUCEF_TRACE;

    return PerformReadOperation( request, true );
}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::OnReadMetaData( const CHttpRequestData& request )
{GUCEF_TRACE;

    return PerformReadOperation( request, false );
}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::PerformReadOperation( const CHttpRequestData& request ,
                                                        bool contentRequested           )
{GUCEF_TRACE;

    CHttpResponseData* returnData = new CHttpResponseData();

    if ( NULL == m_routerController )
    {
        returnData->statusCode = 500; // Improperly configured
        return returnData;
    }

    try
    {
        const CString& resourceURI = request.requestUri;

        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No handler is mounted to handle this Uri
            returnData->statusCode = 404;
            return returnData;
        }

        // Obtain the relative Uri for the given handler
        CString uriAfterBaseAddress = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );

        // Obtain access to the resource based of the relative Uri
        CIHTTPServerRouter::THTTPServerResourcePtr resource = resourceRouter->ResolveUriToResource( uriAfterBaseAddress );
        if ( 0 == resource )
        {
            // Not found
            returnData->statusCode = 404;
            return returnData;
        }

        returnData->contentType = resource->GetBestMatchedSerializationRepresentation( request.resourceRepresentations );

        // Did we find a supported type match?
        if ( returnData->contentType.IsNULLOrEmpty() )
        {
            // Unsupported media type
            returnData->statusCode = 415;
            returnData->acceptedTypes = resource->GetSupportedSerializationRepresentations();
            return returnData;
        }

        // Check if we need to perform a version check for efficiency purposes
        // If a specific version was requested we should only send the resource if the resource has
        // been altered thus saving bandwidth
        if ( MatchResourceVersion( resource->GetResourceVersion(), request.resourceVersions ) )
        {
            // The resource has not been changed thus no need to send the resource to the client
            returnData->statusCode = 304;
            returnData->eTag = resource->GetResourceVersion();
            return returnData;
        }

        if ( contentRequested )
        {
            // Set the serialized resource as the return data content
            if ( !resource->Serialize( returnData->content, returnData->contentType ) )
            {
                // Something went wrong in the handler while serializing the resource
                returnData->statusCode = 415;
                returnData->acceptedTypes = resource->GetSupportedSerializationRepresentations();
                return returnData;
            }
        }

        returnData->eTag = resource->GetResourceVersion();
        returnData->lastModified = resource->GetLastModifiedTime();
        returnData->cacheability = resource->GetCacheability();

        // Inform the client of the actual location of the resource if the request Uri was
        // actually an alias
        CString actualResourceUri = m_routerController->MakeUriAbsolute( *resourceRouter, resourceURI, resource->GetURL() );
        if ( resourceURI != actualResourceUri )
        {         //@TODO: should the status code be 3xx instead of 200??
            returnData->location = actualResourceUri;
        }

        // Set the StatusCode to OK
        returnData->statusCode = 200;
        return returnData;
    }
    catch ( std::exception& )
    {
        // Unhandled exception during execution
        returnData->statusCode = 500;
        return returnData;
    }
}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::OnUpdate( const CHttpRequestData& request ,
                                            bool isDeltaUpdateOnly          )
{GUCEF_TRACE;

    CHttpResponseData* returnData = new CHttpResponseData();

    if ( NULL == m_routerController )
    {
        // No system has been set for providing handlers
        returnData->statusCode = 500;
        return returnData;
    }
    try
    {
        const CString& resourceURI = request.requestUri;

        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No system has been set for providing handlers
            returnData->statusCode = 500;
            return returnData;
        }

        // Check if the client send the content for the update
        if ( request.content.GetDataSize() == 0 )
        {
            returnData->statusCode = 400; // Bad request
            return returnData;
        }

        CString remainderUri = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );
        CIHTTPServerRouter::THTTPServerResourcePtr resource = resourceRouter->ResolveUriToResource( remainderUri );

        if ( 0 == resource )
        {
            // Not found
            returnData->statusCode = 404;
            return returnData;
        }

        CString inputRepresentation;
        if ( request.resourceRepresentations.size() > 0 )
        {
            inputRepresentation = request.resourceRepresentations.front();
            bool isSupportedMimeType = false;

            const TStringVector& supportedRepresentations = resource->GetSupportedDeserializationRepresentations();
            TStringVector::const_iterator i = supportedRepresentations.begin();
            while ( i != supportedRepresentations.end() )
            {
                if ( (*i).Equals( inputRepresentation, false ) )
                {
                    isSupportedMimeType = true;
                    break;
                }
                ++i;
            }

            if ( !isSupportedMimeType )
            {
                returnData->statusCode = 415;
                returnData->acceptedTypes = resource->GetSupportedDeserializationRepresentations();
                return returnData;
            }
        }

        // Check if we need to perform a version (concurrency) check
        if ( request.resourceVersions.size() > 0 )
        {
            if ( !MatchResourceVersion( resource->GetResourceVersion(), request.resourceVersions ) )
            {
                // The resource has been changed while the client was making its changes,..
                // We have a race condition and should abort the update
                returnData->statusCode = 412; // Pre condition failed
                return returnData;
            }
        }

        // Assign to the entry
        CIHTTPServerResource::TDeserializeState deserializeState = resource->Deserialize( request.content, inputRepresentation, isDeltaUpdateOnly );
        if ( CIHTTPServerResource::DESERIALIZESTATE_CORRUPTEDINPUT == deserializeState  )
        {
            // There was a problem in the backend deserializing the content in the negotiated format
            returnData->statusCode = 400; // <- corrupt data,.. bad request
            returnData->acceptedTypes = resource->GetSupportedDeserializationRepresentations();
            return returnData;
        }
        else
        if ( CIHTTPServerResource::DESERIALIZESTATE_UNABLETOUPDATE == deserializeState  )
        {
            // There was a problem in the backend actually updating the resource with the deserialized content
            returnData->statusCode = 304; // <- resource not modified
        }
        else
        {
            // Update to the existing entry was successfull
            returnData->statusCode = 200;
        }

        returnData->eTag = resource->GetResourceVersion();
        returnData->lastModified = resource->GetLastModifiedTime();

        // Make sure we only send back absolute Uri's for resource locations
        returnData->location = m_routerController->MakeUriAbsolute( *resourceRouter, resourceURI, resource->GetURL() );

        // Send the serverside representation back to the client in the representation of the update
        resource->Serialize( returnData->content, inputRepresentation );
        returnData->contentType = inputRepresentation;

        return returnData;
    }
    catch ( std::exception& )
    {
        // Unhandled exception during execution
        returnData->statusCode = 500;
        return returnData;
    }
}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::OnCreate( const CHttpRequestData& request )
{GUCEF_TRACE;

    CHttpResponseData* returnData = new CHttpResponseData();

    if ( request.content.GetDataSize() == 0 )
    {
        //Either the client request could not be parsed or the client has not provided any data
        //to create. So throw bad request.
        returnData->statusCode = 400;
        return returnData;
    }

    if ( NULL == m_routerController )
    {
        returnData->statusCode = 500; // Improperly configured
        return returnData;
    }

    try
    {
        const CString& resourceURI = request.requestUri;

        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No system has been set for providing handlers
            // There is nothing the client can do here.
            returnData->statusCode = 500;
            return returnData;
        }

        // Get relative collection URI and subsequently the collection itself
        CString containerUri = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );
        CIHTTPServerRouter::THTTPServerResourcePtr containerResource = resourceRouter->ResolveUriToResource( containerUri );

        if ( 0 == containerResource )
        {
            // Unable to locate the collection in which to place the entry
            returnData->statusCode = 404;
            return returnData;
        }

        // create resource in preperation for deserialization
        CIHTTPServerRouter::THTTPServerResourcePtr resource;
        TStringVector supportedRepresentations;
        CString createRepresentation = request.resourceRepresentations.front();

        CIHTTPServerResource::TCreateState createState = containerResource->CreateResource( request.transactionID    ,
                                                                                            request.content          ,
                                                                                            createRepresentation     ,
                                                                                            resource                 ,
                                                                                            supportedRepresentations );

        if ( ( CIHTTPServerResource::CREATESTATE_FAILED == createState ) || ( 0 == resource ) )
        {
            // Failed to finalize (thus persist) the data
            returnData->statusCode = 500;
            return returnData;
        }
        // See if the handler was able to already provide access to the placeholder as a resource
        if ( CIHTTPServerResource::CREATESTATE_CONFLICTING == createState )
        {
            returnData->statusCode = 409; // Conflict: already exists and it is not the same item
            return returnData;
        }
        if ( CIHTTPServerResource::CREATESTATE_UNSUPPORTEDREPRESENTATION == createState )
        {
            returnData->statusCode = 415;
            returnData->acceptedTypes = supportedRepresentations;
            return returnData;
        }

        if ( CIHTTPServerResource::CREATESTATE_DESERIALIZATIONFAILED == createState )
        {
            // tell client about the error,.. cannot create because of corrupted stream
            returnData->statusCode = 400;
            returnData->acceptedTypes = supportedRepresentations;
            return returnData;
        }

        if ( CIHTTPServerResource::CREATESTATE_CREATED == createState )
        {
            returnData->location = resource->GetURL();

            // To comply with protocols like ATOM-pub we need to stream out whatever content is already present
            // as the new resource as part of our reply regardless of whether content was send to the server

            // Perform the serialization
            resource->Serialize( returnData->content, createRepresentation );

            // Make sure the client gets an absolute path to the resource
            // which may be a placeholder
            returnData->location = m_routerController->MakeUriAbsolute( *resourceRouter, resourceURI, returnData->location );

            // tell client what representation the resource is stored as plus its version
            returnData->contentType = createRepresentation;
            returnData->eTag = resource->GetResourceVersion();
            returnData->lastModified = resource->GetLastModifiedTime();

            // Tell the client we succeeded in creating a new resource
            returnData->statusCode = 201;
            return returnData;
        }

        // We should not be able to get here,.. bad create state
        returnData->statusCode = 500;
        return returnData;

    }
    catch ( std::exception& )
    {
        // Unhandled exception during execution
        returnData->statusCode = 500;
        return returnData;
    }
}

/*-------------------------------------------------------------------------*/

CHttpResponseData*
CDefaultHttpServerRequestHandler::OnDelete( const CHttpRequestData& request )
{GUCEF_TRACE;

    CHttpResponseData* returnData = new CHttpResponseData();

    if ( NULL == m_routerController )
    {
        returnData->statusCode = 500; // Improperly configured
        return returnData;
    }

    try
    {
        const CString& resourceURI = request.requestUri;

        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No system has been set for providing handlers
            returnData->statusCode = 404;
            return returnData;
        }

        CString remainderUri = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );
        CIHTTPServerRouter::THTTPServerResourcePtr resource = resourceRouter->ResolveUriToResource( remainderUri );

        if ( 0 == resource )
        {
            // Not fount
            returnData->statusCode = 404;
            return returnData;
        }

        if ( request.resourceVersions.size() > 0 )
        {
            // Check if we need to perform a version (concurrency) check
            // Be determine this by checking whether client entered a specific resource version to delete
            if ( !MatchResourceVersion( resource->GetResourceVersion(), request.resourceVersions ) )
            {
                // The resource it of a different version then what the client expected,..
                // We will abort the delete
                returnData->statusCode = 412;
                returnData->eTag = resource->GetResourceVersion();
                return returnData;
            }
        }

        if ( resource->DeleteResource() )
        {
            // tell client the delete succeeded
            returnData->statusCode = 200;
            return returnData;
        }
        else
        {
            returnData->statusCode = 405; //
            returnData->allowedMethods.push_back( "GET" ); // Hmm, we're not quite sure are we...
            returnData->allowedMethods.push_back( "POST" );
            return returnData;
        }
    }
    catch ( std::exception& )
    {
        // Unhandled exception during execution
        returnData->statusCode = 500;
        return returnData;
    }
}

/*-------------------------------------------------------------------------*/

bool
CDefaultHttpServerRequestHandler::MatchResourceVersion( const CString& resourceVersion  ,
                                                        const TStringVector& searchList )
{GUCEF_TRACE;

    if ( searchList.size() > 0 && resourceVersion.Length() > 0 )
    {
        TStringVector::const_iterator i = searchList.begin();
        while( i != searchList.end() )
        {
            if ( resourceVersion.Equals( *i, false ) )
            {
                return true;
            }
            ++i;
        }
    }
    return false;
}

/*-------------------------------------------------------------------------*/

bool 
CDefaultHttpServerRequestHandler::SetRouterController( CIHTTPServerRouterController* routerController )
{GUCEF_TRACE;

    m_routerController = routerController;
    return true;
}

/*-------------------------------------------------------------------------*/
    
CIHTTPServerRouterController* 
CDefaultHttpServerRequestHandler::GetRouterController( void ) const
{GUCEF_TRACE;

    return m_routerController;
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
