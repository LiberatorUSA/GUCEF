

class CUDim
{
    public:
    
    void SetRelativeValue( const Float32 relativeValue ) const
    
    Float32 GetRelativeValue( void ) const;
    
    void SetPixelValue( const UInt32 pixelValue ) const;
    
    UInt32 GetPixelValue( void ) const;
    
    private:
    
    Float32 m_relativeValue;
    UInt32 m_pixelValue;
};
