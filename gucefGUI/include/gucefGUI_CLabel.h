


class CLabel : public CWidget
{
    public:

    static const CORE::CEvent TextChangedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    virtual bool SetText( const CString& text );
    
    virtual bool GetText( CString& text );
};