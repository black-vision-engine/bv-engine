                            VideoInputChannel           ( RenderChannel * wrappedChannel );

public:
                            ~VideoInputChannel          ();

    bool                    ContainsValidData           () const;
    void                    SetContainsValidData        ( bool isDataValid );

    Texture2DPtr            ReadColorTexture            ( Renderer * renderer ) const;
    void                    InvalidateCachedTexture     () const;
