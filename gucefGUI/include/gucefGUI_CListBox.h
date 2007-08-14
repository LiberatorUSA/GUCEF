



class CListbox : public CWidget
{
    public:
        
    static const CORE::CEvent SelectionChangedEvent;
    static const CORE::CEvent LineAddedEvent;
    static const CORE::CEvent LineRemovedEvent;
    static const CORE::CEvent LineSelectedEvent;
    static const CORE::CEvent LineDeselectedEvent;
    static const CORE::CEvent LineClickedEvent;
    static const CORE::CEvent LineDoubleClickedEvent;
    static const CORE::CEvent LineTripleClickedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    typedef std::vector< CString > TStringList;
    
    virtual bool SetAllowMultiSelect( bool allowMultiselect );
    
    virtual bool GetAllowMultiSelect( bool allowMultiselect );
    
    virtual UInt32 GetLineCount( void );
    
    virtual bool Clear( void );
    
    virtual bool GetLineAtIndex( const UInt32 lineIndex ,
                                 CString& lineText      ) const;
    
    virtual bool InsertLineAtIndex( const UInt32 lineIndex ,
                                    const CString& text    );
    
    virtual bool AppendLine( const CString& text );
    
    virtual bool GetText( CString& text ) const;
    
    virtual bool SetLineSelection( const UInt32 lineIndex ,
                                   const bool isSelected  );

    virtual bool GetLineSelection( const UInt32 lineIndex ,
                                   bool& isSelected       ) const;
                                       
    virtual bool GetLastSelectedLine( CString& text ) const;    
    
    virtual bool GetSelectedLines( TStringList& selectedLines ) const;
};