

class CUVector
{
    public:
    
    void SetWidth( const CUDim& newWidth );
    
    const CUDim& GetWidth( void ) const;
    
    void SetHeight( const CUDim& newHeight );
    
    const CUDim& GetHeight( void ) const;    
    
    private:
    
    CUDim m_width;
    CUDim m_height;
};