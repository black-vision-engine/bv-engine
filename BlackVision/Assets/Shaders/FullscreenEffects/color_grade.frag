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


vec4 
cdef inline color _rgb_to_xyz(double r, double g, double b):
    cdef double rl, gl, bl
    cdef color color

    # convert RGB to linear scale
    IF SRGB_COMPAND:
        if r <= 0.04045:
            rl = r / 12.92
        else:
            rl = ((r + 0.055) / 1.055) ** 2.4
        if g <= 0.04045:
            gl = g / 12.92
        else:
            gl = ((g + 0.055) / 1.055) ** 2.4
        if b <= 0.04045:
            bl = b / 12.92
        else:
            bl = ((b + 0.055) / 1.055) ** 2.4
    ELSE:
        # Use "simplified sRGB"
        rl = r ** gamma
        gl = g ** gamma
        bl = b ** gamma

    # matrix mult for srgb->xyz,
    # includes adjustment for reference white
    x = ((rl * 0.4124564) + (gl * 0.3575761) + (bl * 0.1804375)) / xn
    y = ((rl * 0.2126729) + (gl * 0.7151522) + (bl * 0.0721750))
    z = ((rl * 0.0193339) + (gl * 0.1191920) + (bl * 0.9503041)) / zn

    color.one = x
    color.two = y
    color.three = z
    return color


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