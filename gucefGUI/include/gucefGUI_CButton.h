


class CButton : public CWidget
{
    public:
    
    static const CORE::CEvent ButtonDownEvent;
    static const CORE::CEvent ButtonUpEvent;
    static const CORE::CEvent ButtonClickedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    virtual bool SetButtonText( const CString& newText );
    
    virtual bool GetButtonText( CString& text ) const;
    
};