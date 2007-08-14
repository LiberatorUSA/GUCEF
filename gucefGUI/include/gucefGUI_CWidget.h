

class CWidget : public CORE::CObservingNotifier
{
    public:
    
    static const CORE::CEvent PropertiesLoadedEvent;
    static const CORE::CEvent PropertiesSavedEvent;
    static const CORE::CEvent PropertyChangedEvent;
    
    static const CORE::CEvent MouseMoveEvent;
    static const CORE::CEvent MouseDownEvent;
    static const CORE::CEvent MouseUpEvent;
    static const CORE::CEvent MouseClickEvent;
    static const CORE::CEvent MouseDoubleClickEvent;
    static const CORE::CEvent MouseTripleClickEvent;
    
    static const CORE::CEvent KeyDownEvent;
    static const CORE::CEvent KeyUpEvent;
    
    static const CORE::CEvent FocusReceivedEvent;
    static const CORE::CEvent FocusLostEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::set< CWidget* > TWidgetSet;
    
    CWidget( void );
    
    virtual bool SetVisibility( const bool isVisible );
    
    virtual bool IsVisible( void ) const;
    
    virtual bool SetEnabled( const bool isEnabled );
     
    virtual bool IsEnabled( void ) const;
    
    virtual bool SetPosition( const CUVector& newPos );
    
    virtual bool GetPosition( CUVector& pos );
    
    virtual bool SetSize( const CUVector& newSize );
    
    virtual bool GetSize( CUVector& size ) const;
    
    virtual bool SetWidth( const CUDim& newWidth );
    
    virtual bool GetWidth( CUDim& width );
    
    virtual bool SetHeight( const CUDim& newHeight );
    
    virtual bool GetHeight( CUDim& width );    
    
    virtual bool GetParentWidget( CWidget** parentWidget );
    
    virtual bool GetChildWidgetSet( TWidgetSet& childSet );
    
    virtual bool SetPropertyValue( const CString& widgetProperty ,
                                   const CString& propertyValue  );

    virtual bool GetPropertyValue( const CString& widgetProperty ,
                                   CString& propertyValue        ) const;

    virtual bool SetProperties( const CORE::CValueList& list );

    virtual bool GetProperties( CORE::CValueList& list ) const;
    
    virtual bool HasProperty( const CString& widgetProperty ) const;
    
    virtual bool LoadProperties( CORE::CIOAccess& propertyStorage );
    
    virtual bool SaveProperties( CORE::CIOAccess& propertyStorage ) const;
};