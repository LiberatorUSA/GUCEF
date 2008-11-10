/*
 *  gucefCOM: GUCEF module providing communication 
 *  implementations for standardized protocols.
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

/*-------------------------------------------------------------------------//
//                                                                         //
//      INCLUDES                                                           //
//                                                                         //
//-------------------------------------------------------------------------*/

#ifndef GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#include "gucefCOM_CDefaultHTTPServerRouterController.h"
#define GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H
#endif /* GUCEF_COM_CDEFAULTHTTPSERVERROUTERCONTROLLER_H ? */

#include "gucefCOM_CHTTPServer.h"

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

namespace GUCEF {
namespace COM {

/*-------------------------------------------------------------------------//
//                                                                         //
//      CLASSES                                                            //
//                                                                         //
//-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( CIHTTPServerRouterController* routerController /* = NULL */ )
    : CObserver()                ,
      m_tcpServerSocket( false ) ,
      m_routerController( NULL ) ,
      m_lastRequestUri()
{GUCEF_TRACE;

    if ( NULL == routerController )
    {
        m_routerController = new CDefaultHTTPServerRouterController( *this );
    }
    else
    {
        m_routerController = routerController;
    }
    
    m_tcpServerSocket.Subscribe( this );
}

/*-------------------------------------------------------------------------*/

CHTTPServer::CHTTPServer( CORE::CPulseGenerator& pulsGenerator                        ,
                          CIHTTPServerRouterController* routerController /* = NULL */ )
    : CObserver()                               ,
      m_tcpServerSocket( pulsGenerator, false ) ,
      m_routerController( NULL )                ,
      m_lastRequestUri()
{GUCEF_TRACE;

    if ( NULL == routerController )
    {
        m_routerController = new CDefaultHTTPServerRouterController( *this );
    }
    else
    {
        m_routerController = routerController;
    }
    
    m_tcpServerSocket.Subscribe( this );
}

/*-------------------------------------------------------------------------*/
    
CHTTPServer::~CHTTPServer()
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

bool
CHTTPServer::MatchResourceVersion( const CString& resourceVersion  ,
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

CHTTPServer::THttpReturnData*
CHTTPServer::OnRead( const THttpRequestData& request )
{GUCEF_TRACE;
    
    return PerformReadOperation( request, true );
}

/*-------------------------------------------------------------------------*/

CHTTPServer::THttpReturnData*
CHTTPServer::OnReadMetaData( const THttpRequestData& request )
{GUCEF_TRACE;

    return PerformReadOperation( request, false );
}

/*-------------------------------------------------------------------------*/

CHTTPServer::THttpReturnData*
CHTTPServer::PerformReadOperation( const THttpRequestData& request , 
                                   bool contentRequested           )
{GUCEF_TRACE;

    THttpReturnData* returnData = new THttpReturnData;

    if ( NULL == m_routerController )
    {
        returnData->statusCode = 500; // Improperly configured
        return returnData;
    }

    try
    {
        CString resourceURI;
        if ( request.fullResourceUri.Length() > 0 )
        {
            resourceURI = request.requestUri;
        }
        m_lastRequestUri = resourceURI; 
        
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
        if ( NULL == resource )
        {
            // Not found
            returnData->statusCode = 404;
            return returnData;
        }

        returnData->contentType = resource->GetBestMatchedSerializationRepresentation( request.resourceRepresentations );

        // Did we find a supported type match?
        if ( returnData->contentType.Length() > 0 )
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
    
CHTTPServer::THttpReturnData*
CHTTPServer::OnUpdate( const THttpRequestData& request )
{GUCEF_TRACE;

    THttpReturnData* returnData = new THttpReturnData;

    if ( NULL == m_routerController )
    {
        // No system has been set for providing handlers
        returnData->statusCode = 500;
        return returnData;
    }
    try
    {            
        CString resourceURI;
        if ( request.fullResourceUri.Length() > 0 )
        {
            resourceURI = request.fullResourceUri;
        }
        else
        {
            resourceURI = request.requestUri;
        }
        m_lastRequestUri = resourceURI;
        
        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No system has been set for providing handlers
            returnData->statusCode = 500;
            return returnData;
        } 

        // Check if the client send the content for the update
        if ( request.content.GetDataSize() > 0 )
        {
            returnData->statusCode = 400; // Bad request
            return returnData;
        }

        CString remainderUri = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );                    
        CIHTTPServerRouter::THTTPServerResourcePtr resource = resourceRouter->ResolveUriToResource( remainderUri );
                                                       
        if ( NULL == resource )
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
        CIHTTPServerResource::TDeserializeState deserializeState = resource->Deserialize( request.content, inputRepresentation );
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

CHTTPServer::THttpReturnData*
CHTTPServer::OnCreate( const THttpRequestData& request )
{GUCEF_TRACE;

    THttpReturnData* returnData = new THttpReturnData;

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
        CString resourceURI;
        if ( request.fullResourceUri.Length() > 0 )
        {
            resourceURI = request.fullResourceUri;
        }
        else
        {
            resourceURI = request.requestUri;
        }
        m_lastRequestUri = resourceURI;
        
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

        if ( NULL == containerResource ) 
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

        if ( ( CIHTTPServerResource::CREATESTATE_FAILED == createState ) || ( NULL == resource ) )
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
    
CHTTPServer::THttpReturnData*
CHTTPServer::OnDelete( const THttpRequestData& request )
{GUCEF_TRACE;

    THttpReturnData* returnData = new THttpReturnData;

    if ( NULL == m_routerController )
    {
        returnData->statusCode = 500; // Improperly configured
        return returnData;
    }

    try
    {
        CString resourceURI;
        if ( request.fullResourceUri.Length() > 0 )
        {
            resourceURI = request.fullResourceUri;
        }
        else
        {
            resourceURI = request.requestUri;
        }
        m_lastRequestUri = resourceURI;
        
        CIHTTPServerRouter* resourceRouter = m_routerController->GetHandler( resourceURI );
        if ( NULL == resourceRouter )
        {
            // No system has been set for providing handlers
            returnData->statusCode = 404;
            return returnData;
        } 

        CString remainderUri = m_routerController->GetUriAfterTheBaseAddress( *resourceRouter, resourceURI );
        CIHTTPServerRouter::THTTPServerResourcePtr resource = resourceRouter->ResolveUriToResource( remainderUri );

        if ( NULL == resource )
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
    
CString
CHTTPServer::GetLastRequestUri( void ) const
{GUCEF_TRACE;
    
    return m_lastRequestUri;
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::OnNotify( CORE::CNotifier* notifier                 ,
                       const CORE::CEvent& eventid               ,
                       CORE::CICloneable* eventdata /* = NULL */ )
{GUCEF_TRACE;

}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::ExtractCommaSeparatedValues( const CString& stringToExtractFrom ,
                                          TStringVector& list                ) const
{GUCEF_TRACE;

    // Create a list and make sure we trim each element
    TStringVector elements = stringToExtractFrom.ParseElements( ',' );
    TStringVector::iterator i = elements.begin();
    while ( i != elements.end() )
    {                    
        list.push_back( (*i).RemoveChar( ' ' ) );
    }
}

/*-------------------------------------------------------------------------*/

CHTTPServer::THttpRequestData*
CHTTPServer::ParseRequest( CORE::CDynamicBuffer& inputBuffer )
{GUCEF_TRACE;
 /*
    HttpRequestData request = new HttpRequestData();

    WebHeaderCollection requestHeaders = WebOperationContext.Current.IncomingRequest.Headers;
    IncomingWebRequestContext incomingRequest = WebOperationContext.Current.IncomingRequest;

    //Common implementation goes here
    string httpMethod = WebOperationContext.Current.IncomingRequest.Method;// GET, PUT, POST , DELETE etc.

    //Read authorization token here and any other thing related to authorization.
    //Similarly other headers can be read
    // @TODO: string authorizationToken = requestProp.Headers[ HttpRequestHeader.Authorization ];            

    string contentType = WebOperationContext.Current.IncomingRequest.ContentType;
    if (null != contentType) {
        request.ResourceRepresentations.Add(contentType.Trim());
    } else {
        //Gets hold of the mime types that the client can handle
        string acceptTypes = incomingRequest.Accept;
        request.ResourceRepresentations = ExtractCommaSeparatedValues(acceptTypes);

    }

    if (null != body) {
        int readBytes = 0;
        byte[] readBuffer = new byte[1024];
        MemoryStream memoryStream = new MemoryStream();
        do {
            readBytes = body.Read(readBuffer, 0, 1024);
            memoryStream.Write(readBuffer, 0, readBytes);
        } while (readBytes == 1024);

        memoryStream.Seek(0, SeekOrigin.Begin);
        if (memoryStream.Length > 0) {
            request.Content = memoryStream;
        }
    }

                
    // Get the resource versions desired by the client
    request.ResourceVersions = null;
    request.ResourceVersions = ExtractCommaSeparatedValues( requestHeaders[ HttpRequestHeader.IfMatch ] );
    
    //If it is null, it could also mean that the values are in If-None-Match
    if ( null == request.ResourceVersions )
    {
        request.ResourceVersions = ExtractCommaSeparatedValues( requestHeaders[HttpRequestHeader.IfNoneMatch ] );
    }
             
    request.TransactionID = requestHeaders[ HttpRequestHeader.Cookie ];
    request.RequestUri = incomingRequest.UriTemplateMatch.RequestUri;
    request.FullResourceUri = request.RequestUri;

    return request;  */
    
    return NULL;
}

/*-------------------------------------------------------------------------*/

void
CHTTPServer::ParseResponse( const THttpReturnData& returnData  ,
                            CORE::CDynamicBuffer& outputBuffer )
{  /*
    try
    {

        Stream body = null;
        WebHeaderCollection headers = WebOperationContext.Current.OutgoingResponse.Headers;

        headers[HttpResponseHeader.ContentType] = returnData.ContentType;
        if (returnData.Content != null) {
            body = returnData.Content;
        }
        if (returnData.Content != null && returnData.Content.CanSeek) {
            headers[HttpResponseHeader.ContentLength] = returnData.Content.Length.ToString();
        }

        
        WebOperationContext.Current.OutgoingResponse.StatusCode = (HttpStatusCode) returnData.StatusCode;
        if ( null != returnData.Location ) 
        {
            headers[ HttpResponseHeader.Location ] = returnData.Location.ToString();
        }
        if ( !String.IsNullOrEmpty( returnData.ETag ) )
        {
            headers[ HttpResponseHeader.ETag ] = returnData.ETag;
        }
        if ( !String.IsNullOrEmpty( returnData.Cacheability ) )
        {
            headers[ HttpResponseHeader.CacheControl ] = returnData.Cacheability;
        }
        
        // You cannot send back accept types to the client using a standard HTTP header since
        // it is a HTTP request only header. We don't want the client to aimlessly retry different
        // representations either on PUT/POST so we do want to inform the client of the accepted types.
        if ( returnData.StatusCode == 415 )
        {
            // Make sure we don't overwrite anything or do work for nothing
            if ( returnData.AcceptedTypes.Count > 0 && null == body )
            {
                // Build the accept header
                string acceptHeader = "Accept:";
                foreach ( string typeName in returnData.AcceptedTypes )
                {
                    acceptHeader += typeName + ',';
                }                           
                // Turn the string into a stream we can send back trough the WFC pipeline
                byte[] buffer = Encoding.GetEncoding( "iso-8859-1" ).GetBytes(acceptHeader);
                body = new MemoryStream( buffer );
                
                returnData.ContentType = "plain/text";
            }
        }
        if (returnData.StatusCode == 405) {
            if (returnData.AllowedMethods.Count > 0) {
                string allow = String.Empty;
                foreach (string allowedMethod in returnData.AllowedMethods) {
                    allow = allow + allowedMethod + ",";
                }
                headers[HttpResponseHeader.Allow] = allow.Remove(allow.Length-1, 1);
            }
        }
        if (body == null) {
            //This code is to overcome the limitation of the ISAPI extension
            //suppressing the content type header when there is no body.
            //In many scenarios such as 201(Created), Head request etc, it is necessary that the
            //meta data be conveyed, with out the body. 
            //so whenever there is no body irrespective of the kind of request made, we always send a dummy buffer
            byte[] dummyBuffer = new byte[1];
            body = new MemoryStream(dummyBuffer);
        }
        return body;
    }
    catch ( Exception )
    {
        return null;
    }     */
}

/*-------------------------------------------------------------------------//
//                                                                         //
//      NAMESPACE                                                          //
//                                                                         //
//-------------------------------------------------------------------------*/

} /* namespace COM */
} /* namespace GUCEF */

/*-------------------------------------------------------------------------*/
