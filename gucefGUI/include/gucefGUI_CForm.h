

class CForm
{
    public:    
    
    static const CORE::CEvent LayoutLoadedEvent;
    static const CORE::CEvent LayoutSavedEvent;
    
    static void RegisterEvents( void );
    
    public:
    
    virtual bool LoadLayout( CIOAccess& layoutStorage );
    
    virtual bool SaveLayout( CIOAccess& layoutStorage );
    
    virtual CWidget* GetRootWidget( void ) const;
};