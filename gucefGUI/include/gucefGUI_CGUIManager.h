


class CGUIManager
{
    public:
    
    static CGUIManager* Instance( void );
    
    CWidgetFactory& GetWidgetFactory( void );
    
    CFormFactory& GetFormFactory( void );
    
    private:
    
    static void Deinstance( void );
};