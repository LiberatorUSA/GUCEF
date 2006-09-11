class CInputHandlerMapper : public CIInputHandler
{
        public:
        
        bool SetActiveHandlerByID( const UInt32 handlerid );
        
        UInt32 GetActiveHandlerByID( void ) const;
        
        
        
        UInt32 AddHandlerRef( CMappedInputHandler* handler );
        
        void DelHandlerRef( const UInt32 handlerid );
        
        public:        
        
        virtual void OnMouseButtonDown( const UInt32 buttonindex );
        
        virtual void OnMouseButtonUp( const UInt32 buttonindex );
        
        virtual void OnKeyboardButtonDown( const UInt32 buttonindex );
        
        virtual void OnKeyboardButtonUp( const UInt32 buttonindex );
        
        virtual void OnDeviceBooleanOn( const UInt32 deviceid   ,
                                        const UInt32 stateindex );
                                        
        virtual void OnDeviceBooleanOff( const UInt32 deviceid   ,
                                         const UInt32 stateindex );
                                         
        virtual void OnDeviceVariableChanged( const UInt32 deviceid   ,
                                              const UInt32 stateindex ,
                                              const Float32 value     );        
};