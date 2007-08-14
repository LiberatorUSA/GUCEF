



class CEditbox : public CWidget
{
    public:
        
    static const CORE::CEvent SelectionChangedEvent;
    static const CORE::CEvent CaratPositionChangedEvent;
    static const CORE::CEvent TextChangedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    virtual bool SetText( const CString& text );
    
    virtual bool GetText( CString& text );
    
    virtual bool SetSelectedText( const CString& text );
    
    virtual bool GetSelectedText( CString& text );    
};