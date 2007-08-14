


class CWindow : public CWidget
{
    public:
    
    bool SetTitleBarText( const CString& title );
    
    bool GetTitleBarText( CString& title ) const;
    
    bool SetTitleBarVisible( const bool isVisible );
    
    bool GetTitleBarVisible( bool& isVisible );
};