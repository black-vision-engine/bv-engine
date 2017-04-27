#version 400

layout (location = 0) out vec4 FragColor;

in vec2 uvCoord;

layout (binding = 0) uniform sampler2D Texture;


void main()
{
	vec4 col = vec4(0.0, 0.0, 0.0, 1.0);

	if( uvCoord.y > 0.5 )
	{
		float u = 2.0 * uvCoord.x;
		vec2 uv = vec2(u - floor(u), 2.0 * uvCoord.y - 1.0);
		
		col = texture( Texture, uv );
		
		if ( u >= 1.0 )
		{
			col = col.aaaa;
		}
	}

    FragColor = col.rgba;
}

float lin_2_sRGB    ( float val )
{
    const float a = 0.055;
    
    if ( val < 0.0031308 )
    {
        return 12.92 * val;
    }
    else
    {
        return ( 1.0 + a ) * pow( val, 1.0 / 2.4 ) - a;
    }
}

float sRGB_2_lin    ( float val )
{
    const float a = 0.055;

    if ( val <= 0.04045 )
    {
        return = val / 12.92;
    }
    else
    {
        return pow( ( val + a ) / ( 1.0 + a ), 2.4 );
    }
}

vex3 lin_2_sRGB( vec3 col )
{
    return vec3( lin_2_sRGB( col.r ), lin_2_sRGB( col.g ), lin_2_sRGB( col.b ) );
}

vex3 sRGB_2_lin( vec3 col )
{
    return vec3( sRGB_2_lin( col.r ), sRGB_2_lin( col.g ), sRGB_2_lin( col.b ) );
}

vec3 rgb_2_xyz( in vec3 c )
{
    // Illuminant: D65
    float xn = 0.95047;
    float yn = 1.0;
    float zn = 1.08883;

    const mat3 RGB_to_XYZ = mat3(
        0.4124564 / xn, 0.3575761 / xn, 0.1804375 / xn,
        0.2126729 / yn, 0.7151522 / yn, 0.0721750 / yn,
        0.0193339 / zn, 0.1191920 / zn, 0.9503041 / zn
    );

    return RGB_to_XYZ * sRGB_2_lin( c );

}

vec3 xyz_2_rgb( in vec3 c )
{
    // Illuminant: D65
    float xn = 0.95047;
    float yn = 1.0;
    float zn = 1.08883;

    const mat3 XYZ_to_RGB = mat3(
         3.2404542 * xn,-1.5371385 * yn,-0.4985314 * zn,
        -0.9692660 * xn, 1.8760108 * yn, 0.0415560 * zn,
         0.0556434 * xn,-0.2040259 * yn, 1.0572252 * zn
    );

    return lin_2_sRGB( XYZ_to_RGB * c );
}

float xyz_2_lab_component( float x )
{
    
}

// http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Lab.html
vec3 xyz_2_lab( in vec3 c )
{
DEF bintercept = 4.0 / 29  # 0.137931
DEF delta = 6.0 / 29  # 0.206896
DEF t0 = delta ** 3  # 0.008856
DEF alpha = (delta ** -2) / 3  # 7.787037
DEF third = 1.0 / 3
DEF kappa = (29.0 / 3) ** 3  # 903.3
DEF gamma = 2.2
DEF xn = 0.95047
DEF yn = 1.0
DEF zn = 1.08883
DEF denom_n = xn + (15 * yn) + (3 * zn)
DEF uprime_n = (4 * xn) / denom_n
DEF vprime_n = (9 * yn) / denom_n

    cdef double fx, fy, fz
    cdef double L, a, b
    cdef color color

    if(
    if x > t0:
        fx = x ** third
    else:
        fx = (alpha * x) + bintercept

    if y > t0:
        fy = y ** third
    else:
        fy = (alpha * y) + bintercept

    if z > t0:
        fz = z ** third
    else:
        fz = (alpha * z) + bintercept

    L = (116 * fy) - 16
    a = 500 * (fx - fy)
    b = 200 * (fy - fz)

    color.one = L
    color.two = a
    color.three = b
    return color
}

vec3 lab_2_xyz( in vec3 c )
{
   cdef double x, y, z
    cdef color color

    tx = ((L + 16) / 116.0) + (a / 500.0)
    if tx > delta:
        x = tx ** 3
    else:
        x = 3 * delta * delta * (tx - bintercept)

    ty = (L + 16) / 116.0
    if ty > delta:
        y = ty ** 3
    else:
        y = 3 * delta * delta * (ty - bintercept)

    tz = ((L + 16) / 116.0) - (b / 200.0)
    if tz > delta:
        z = tz ** 3
    else:
        z = 3 * delta * delta * (tz - bintercept)

    # Reference illuminant
    color.one = x
    color.two = y
    color.three = z
}

vec3 lab_2_lch( in vec3 c )
{
    cdef color color

    color.one = L
    color.two = ((a * a) + (b * b)) ** 0.5
    color.three = atan2(b, a)
    return color
}

vec3 lch_2_lab( in vec3 c )
{
    cdef double a, b
    cdef color color

    a = C * cos(H)
    b = C * sin(H)

    color.one = L
    color.two = a
    color.three = b
}

cdef inline color _xyz_to_lab(double x, double y, double z):
    cdef double fx, fy, fz
    cdef double L, a, b
    cdef color color

    # convert XYZ to LAB colorspace
    if x > t0:
        fx = x ** third
    else:
        fx = (alpha * x) + bintercept

    if y > t0:
        fy = y ** third
    else:
        fy = (alpha * y) + bintercept

    if z > t0:
        fz = z ** third
    else:
        fz = (alpha * z) + bintercept

    L = (116 * fy) - 16
    a = 500 * (fx - fy)
    b = 200 * (fy - fz)

    color.one = L
    color.two = a
    color.three = b
    return color


cdef inline color _lab_to_lch(double L, double a, double b):
    cdef color color

    color.one = L
    color.two = ((a * a) + (b * b)) ** 0.5
    color.three = atan2(b, a)
    return color


cdef inline color _lch_to_lab(double L, double C, double H):
    cdef double a, b
    cdef color color

    a = C * cos(H)
    b = C * sin(H)

    color.one = L
    color.two = a
    color.three = b
    return color


cdef inline color _lab_to_xyz(double L, double a, double b):
    cdef double x, y, z
    cdef color color

    tx = ((L + 16) / 116.0) + (a / 500.0)
    if tx > delta:
        x = tx ** 3
    else:
        x = 3 * delta * delta * (tx - bintercept)

    ty = (L + 16) / 116.0
    if ty > delta:
        y = ty ** 3
    else:
        y = 3 * delta * delta * (ty - bintercept)

    tz = ((L + 16) / 116.0) - (b / 200.0)
    if tz > delta:
        z = tz ** 3
    else:
        z = 3 * delta * delta * (tz - bintercept)

    # Reference illuminant
    color.one = x
    color.two = y
    color.three = z
    return color


cdef inline color _xyz_to_rgb(double x, double y, double z):
    cdef double rlin, glin, blin, r, g, b
    cdef color color

    # uses reference white d65
    x = x * xn
    z = z * zn

    # XYZ to sRGB
    # expanded matrix multiplication
    rlin = (x * 3.2404542) + (y * -1.5371385) + (z * -0.4985314)
    glin = (x * -0.9692660) + (y * 1.8760108) + (z * 0.0415560)
    blin = (x * 0.0556434) + (y * -0.2040259) + (z * 1.0572252)

    IF SRGB_COMPAND:
        if rlin <= 0.0031308:
            r = 12.92 * rlin
        else:
            r = (1.055 * (rlin ** (1 / 2.4))) - 0.055
        if glin <= 0.0031308:
            g = 12.92 * glin
        else:
            g = (1.055 * (glin ** (1 / 2.4))) - 0.055
        if blin <= 0.0031308:
            b = 12.92 * blin
        else:
            b = (1.055 * (blin ** (1 / 2.4))) - 0.055
    ELSE:
        # Use simplified sRGB
        r = rlin ** (1 / gamma)
        g = glin ** (1 / gamma)
        b = blin ** (1 / gamma)

    # constrain to 0..1 to deal with any float drift
    if r > 1.0:
        r = 1.0
    elif r < 0.0:
        r = 0.0
    if g > 1.0:
        g = 1.0
    elif g < 0.0:
        g = 0.0
    if b > 1.0:
        b = 1.0
    elif b < 0.0:
        b = 0.0

    color.one = r
    color.two = g
    color.three = b

    return color


cdef inline color _xyz_to_luv(double x, double y, double z):
    cdef color color
    cdef double L, u, v, uprime, vprime, denom

    denom = x + (15 * y) + (3 * z)
    uprime = (4 * x) / denom
    vprime = (9 * y) / denom

    y = y / yn

    if y <= t0:
        L = kappa * y
    else:
        L = (116 * (y ** third)) - 16

    u = 13 * L * (uprime - uprime_n)
    v = 13 * L * (vprime - vprime_n)

    color.one = L
    color.two = u
    color.three = v
    return color


cdef inline color _luv_to_xyz(double L, double u, double v):
    cdef color color
    cdef double x, y, z, uprime, vprime

    if L == 0.0:
        color.one = 0.0
        color.two = 0.0
        color.three = 0.0
        return color

    uprime = (u / (13 * L)) + uprime_n
    vprime = (v / (13 * L)) + vprime_n

    if L <= 8.0:
        y = L / kappa
    else:
        y = ((L + 16) / 116.0) ** 3

    x = y * ((9 * uprime) / (4 * vprime))
    z = y * ((12 - (3 * uprime) - (20 * vprime)) / (4 * vprime))

    color.one = x
    color.two = y
    color.three = z
    return color


    cpdef np.ndarray[FLOAT_t, ndim=3] saturate_rgb(np.ndarray[FLOAT_t, ndim=3] arr, double satmult):
    """Convert array of RGB -> LCH, adjust saturation, back to RGB
    A special case of convert_arr with hardcoded color spaces and
    a bit of data manipulation inside the loop.
    """
    cdef double r, g, b
    cdef color c_lch
    cdef color c_rgb

    if arr.shape[0] != 3:
        raise ValueError("The 0th dimension must contain 3 bands")

    I = arr.shape[1]
    J = arr.shape[2]

    cdef np.ndarray[FLOAT_t, ndim=3] out = np.empty(shape=(3, I, J))

    for i in range(I):
        for j in range(J):
            r = arr[0, i, j]
            g = arr[1, i, j]
            b = arr[2, i, j]

            c_lch = _convert(r, g, b, RGB, LCH)
            c_lch.two *= satmult
            c_rgb = _convert(c_lch.one, c_lch.two, c_lch.three, LCH, RGB)

            out[0, i, j] = c_rgb.one
            out[1, i, j] = c_rgb.two
            out[2, i, j] = c_rgb.three

    return out