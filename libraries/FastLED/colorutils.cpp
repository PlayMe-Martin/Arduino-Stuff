#define __PROG_TYPES_COMPAT__

#include <stdint.h>

#include "hsv2rgb.h"
#include "colorutils.h"


void fill_solid( struct CRGB * pFirstLED, int numToFill,
                const struct CRGB& color)
{
    for( int i = 0; i < numToFill; i++) {
        pFirstLED[i] = color;
    }
}

void fill_rainbow( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 255;
    for( int i = 0; i < numToFill; i++) {
        hsv2rgb_rainbow( hsv, pFirstLED[i]);
        hsv.hue += deltahue;
    }
}


#define saccum87 int16_t

void fill_gradient( CRGB* leds,
                    uint16_t startpos, CHSV startcolor,
                    uint16_t endpos,   CHSV endcolor,
                    TGradientDirectionCode directionCode )
{
    // if the points are in the wrong order, straighten them
    if( endpos < startpos ) {
        uint16_t t = endpos;
        CHSV tc = endcolor;
        startpos = t;
        startcolor = tc;
        endcolor = startcolor;
        endpos = startpos;
    }
    
    saccum87 huedistance87;
    saccum87 satdistance87;
    saccum87 valdistance87;
    
    satdistance87 = (endcolor.sat - startcolor.sat) << 7;
    valdistance87 = (endcolor.val - startcolor.val) << 7;
    
    uint8_t huedelta8 = endcolor.hue - startcolor.hue;
    
    if( directionCode == SHORTEST_HUES ) {
        directionCode = FORWARD_HUES;
        if( huedelta8 > 127) {
            directionCode = BACKWARD_HUES;
        }
    }
    
    if( directionCode == LONGEST_HUES ) {
        directionCode = FORWARD_HUES;
        if( huedelta8 < 128) {
            directionCode = BACKWARD_HUES;
        }
    }
    
    if( directionCode == FORWARD_HUES) {
        huedistance87 = huedelta8 << 7;
    }
    else /* directionCode == BACKWARD_HUES */
    {
        huedistance87 = (uint8_t)(256 - huedelta8) << 7;
        huedistance87 = -huedistance87;
    }
    
    uint16_t pixeldistance = endpos - startpos;
    uint16_t p2 = pixeldistance / 2;
    int16_t divisor = p2 ? p2 : 1;
    saccum87 huedelta87 = huedistance87 / divisor;
    saccum87 satdelta87 = satdistance87 / divisor;
    saccum87 valdelta87 = valdistance87 / divisor;
    
    accum88 hue88 = startcolor.hue << 8;
    accum88 sat88 = startcolor.sat << 8;
    accum88 val88 = startcolor.val << 8;
    for( uint16_t i = startpos; i <= endpos; i++) {
        leds[i] = CHSV( hue88 >> 8, sat88 >> 8, val88 >> 8);
        hue88 += huedelta87;
        sat88 += satdelta87;
        val88 += valdelta87;
    }
}

void fill_gradient_RGB( CRGB* leds,
                   uint16_t startpos, CRGB startcolor,
                   uint16_t endpos,   CRGB endcolor )
{
    // if the points are in the wrong order, straighten them
    if( endpos < startpos ) {
        uint16_t t = endpos;
        CRGB tc = endcolor;
        startpos = t;
        startcolor = tc;
        endcolor = startcolor;
        endpos = startpos;
    }
    
    saccum87 rdistance87;
    saccum87 gdistance87;
    saccum87 bdistance87;
    
    rdistance87 = (endcolor.r - startcolor.r) << 7;
    gdistance87 = (endcolor.g - startcolor.g) << 7;
    bdistance87 = (endcolor.b - startcolor.b) << 7;
        
    uint16_t pixeldistance = endpos - startpos;
    uint16_t p2 = pixeldistance / 2;
    int16_t divisor = p2 ? p2 : 1;
    saccum87 rdelta87 = rdistance87 / divisor;
    saccum87 gdelta87 = gdistance87 / divisor;
    saccum87 bdelta87 = bdistance87 / divisor;
    
    accum88 r88 = startcolor.r << 8;
    accum88 g88 = startcolor.g << 8;
    accum88 b88 = startcolor.b << 8;
    for( uint16_t i = startpos; i <= endpos; i++) {
        leds[i] = CRGB( r88 >> 8, g88 >> 8, b88 >> 8);
        r88 += rdelta87;
        g88 += gdelta87;
        b88 += bdelta87;
    }
}


void fill_gradient( CRGB* leds, uint16_t numLeds, const CHSV& c1, const CHSV& c2, TGradientDirectionCode directionCode )
{
    uint16_t last = numLeds - 1;
    fill_gradient( leds, 0, c1, last, c2, directionCode);
}


void fill_gradient( CRGB* leds, uint16_t numLeds, const CHSV& c1, const CHSV& c2, const CHSV& c3, TGradientDirectionCode directionCode )
{
    uint16_t half = (numLeds / 2);
    uint16_t last = numLeds - 1;
    fill_gradient( leds,    0, c1, half, c2, directionCode);
    fill_gradient( leds, half, c2, last, c3, directionCode);
}

void fill_gradient( CRGB* leds, uint16_t numLeds, const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4, TGradientDirectionCode directionCode )
{
    uint16_t onethird = (numLeds / 3);
    uint16_t twothirds = ((numLeds * 2) / 3);
    uint16_t last = numLeds - 1;
    fill_gradient( leds,         0, c1,  onethird, c2, directionCode);
    fill_gradient( leds,  onethird, c2, twothirds, c3, directionCode);
    fill_gradient( leds, twothirds, c3,      last, c4, directionCode);
}

#if 0
void fill_gradient( const CHSV& c1, const CHSV& c2)
{
    fill_gradient( FastLED[0].leds(), FastLED[0].size(), c1, c2);
}

void fill_gradient( const CHSV& c1, const CHSV& c2, const CHSV& c3)
{
    fill_gradient( FastLED[0].leds(), FastLED[0].size(), c1, c2, c3);
}

void fill_gradient( const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
{
    fill_gradient( FastLED[0].leds(), FastLED[0].size(), c1, c2, c3, c4);
}

void fill_gradient_RGB( const CRGB& c1, const CRGB& c2)
{
    fill_gradient_RGB( FastLED[0].leds(), FastLED[0].size(), c1, c2);
}

void fill_gradient_RGB( const CRGB& c1, const CRGB& c2, const CRGB& c3)
{
    fill_gradient_RGB( FastLED[0].leds(), FastLED[0].size(), c1, c2, c3);
}

void fill_gradient_RGB( const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
{
    fill_gradient_RGB( FastLED[0].leds(), FastLED[0].size(), c1, c2, c3, c4);
}
#endif




void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2)
{
    uint16_t last = numLeds - 1;
    fill_gradient_RGB( leds, 0, c1, last, c2);
}


void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3)
{
    uint16_t half = (numLeds / 2);
    uint16_t last = numLeds - 1;
    fill_gradient_RGB( leds,    0, c1, half, c2);
    fill_gradient_RGB( leds, half, c2, last, c3);
}

void fill_gradient_RGB( CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
{
    uint16_t onethird = (numLeds / 3);
    uint16_t twothirds = ((numLeds * 2) / 3);
    uint16_t last = numLeds - 1;
    fill_gradient_RGB( leds,         0, c1,  onethird, c2);
    fill_gradient_RGB( leds,  onethird, c2, twothirds, c3);
    fill_gradient_RGB( leds, twothirds, c3,      last, c4);
}




void nscale8_video( CRGB* leds, uint16_t num_leds, uint8_t scale)
{
    for( uint16_t i = 0; i < num_leds; i++) {
        leds[i].nscale8_video( scale);
    }
}

void fade_video(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8_video( leds, num_leds, 255 - fadeBy);
}

void fadeLightBy(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8_video( leds, num_leds, 255 - fadeBy);
}


void fadeToBlackBy( CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8( leds, num_leds, 255 - fadeBy);
}

void fade_raw( CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8( leds, num_leds, 255 - fadeBy);
}

void nscale8_raw( CRGB* leds, uint16_t num_leds, uint8_t scale)
{
    nscale8( leds, num_leds, scale);
}

void nscale8( CRGB* leds, uint16_t num_leds, uint8_t scale)
{
    for( uint16_t i = 0; i < num_leds; i++) {
        leds[i].nscale8( scale);
    }
}


CRGB& nblend( CRGB& existing, const CRGB& overlay, fract8 amountOfOverlay )
{
    if( amountOfOverlay == 0) {
        return existing;
    }
    
    if( amountOfOverlay == 255) {
        existing = overlay;
        return existing;
    }
    
    fract8 amountOfKeep = 256 - amountOfOverlay;
    
    existing.red   = scale8_LEAVING_R1_DIRTY( existing.red,   amountOfKeep)
                    + scale8_LEAVING_R1_DIRTY( overlay.red,    amountOfOverlay);
    existing.green = scale8_LEAVING_R1_DIRTY( existing.green, amountOfKeep)
                    + scale8_LEAVING_R1_DIRTY( overlay.green,  amountOfOverlay);
    existing.blue  = scale8_LEAVING_R1_DIRTY( existing.blue,  amountOfKeep)
                    + scale8_LEAVING_R1_DIRTY( overlay.blue,   amountOfOverlay);
    
    cleanup_R1();
    
    return existing;
}



void nblend( CRGB* existing, CRGB* overlay, uint16_t count, fract8 amountOfOverlay)
{
    for( uint16_t i = count; i; i--) {
        nblend( *existing, *overlay, amountOfOverlay);
        existing++;
        overlay++;
    }
}

CRGB blend( const CRGB& p1, const CRGB& p2, fract8 amountOfP2 )
{
    CRGB nu(p1);
    nblend( nu, p2, amountOfP2);
    return nu;
}

CRGB* blend( const CRGB* src1, const CRGB* src2, CRGB* dest, uint16_t count, fract8 amountOfsrc2 )
{
    for( uint16_t i = count; i; i--) {
        dest[i] = blend(src1[i], src2[i], amountOfsrc2);
    }
    return dest;
}




// CRGB HeatColor( uint8_t temperature)
//
// Approximates a 'black body radiation' spectrum for
// a given 'heat' level.  This is useful for animations of 'fire'.
// Heat is specified as an arbitrary scale from 0 (cool) to 255 (hot).
// This is NOT a chromatically correct 'black body radiation'
// spectrum, but it's surprisingly close, and it's fast and small.
//
// On AVR/Arduino, this typically takes around 70 bytes of program memory,
// versus 768 bytes for a full 256-entry RGB lookup table.

CRGB HeatColor( uint8_t temperature)
{
    CRGB heatcolor;
    
    // Scale 'heat' down from 0-255 to 0-191,
    // which can then be easily divided into three
    // equal 'thirds' of 64 units each.
    uint8_t t192 = scale8_video( temperature, 192);
    
    // calculate a value that ramps up from
    // zero to 255 in each 'third' of the scale.
    uint8_t heatramp = t192 & 0x3F; // 0..63
    heatramp <<= 2; // scale up to 0..252
    
    // now figure out which third of the spectrum we're in:
    if( t192 & 0x80) {
        // we're in the hottest third
        heatcolor.r = 255; // full red
        heatcolor.g = 255; // full green
        heatcolor.b = heatramp; // ramp up blue
        
    } else if( t192 & 0x40 ) {
        // we're in the middle third
        heatcolor.r = 255; // full red
        heatcolor.g = heatramp; // ramp up green
        heatcolor.b = 0; // no blue
        
    } else {
        // we're in the coolest third
        heatcolor.r = heatramp; // ramp up red
        heatcolor.g = 0; // no green
        heatcolor.b = 0; // no blue
    }
    
    return heatcolor;
}



CRGB ColorFromPalette( const CRGBPalette16& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
    uint8_t hi4 = index >> 4;
    uint8_t lo4 = index & 0x0F;
    
    //  CRGB rgb1 = pal[ hi4];
    const CRGB* entry = &(pal[0]) + hi4;
    uint8_t red1   = entry->red;
    uint8_t green1 = entry->green;
    uint8_t blue1  = entry->blue;
    
    uint8_t blend = lo4 && (blendType != NOBLEND);
    
    if( blend ) {
        
        if( hi4 == 15 ) {
            entry = &(pal[0]);
        } else {
            entry++;
        }
        
        uint8_t f2 = lo4 << 4;
        uint8_t f1 = 256 - f2;
        
        //    rgb1.nscale8(f1);
        red1   = scale8_LEAVING_R1_DIRTY( red1,   f1);
        green1 = scale8_LEAVING_R1_DIRTY( green1, f1);
        blue1  = scale8_LEAVING_R1_DIRTY( blue1,  f1);
                
        //    cleanup_R1();
        
        //    CRGB rgb2 = pal[ hi4];
        //    rgb2.nscale8(f2);
        uint8_t red2   = entry->red;
        uint8_t green2 = entry->green;
        uint8_t blue2  = entry->blue;
        red2   = scale8_LEAVING_R1_DIRTY( red2,   f2);
        green2 = scale8_LEAVING_R1_DIRTY( green2, f2);
        blue2  = scale8_LEAVING_R1_DIRTY( blue2,  f2);
        
        cleanup_R1();
        
        // These sums can't overflow, so no qadd8 needed.
        red1   += red2;
        green1 += green2;
        blue1  += blue2;

    }
    
    if( brightness != 255) {
        nscale8x3_video( red1, green1, blue1, brightness);
    }
    
    return CRGB( red1, green1, blue1);  
}


CRGB ColorFromPalette( const CRGBPalette256& pal, uint8_t index, uint8_t brightness, TBlendType)
{
    const CRGB* entry = &(pal[0]) + index;

    uint8_t red   = entry->red;
    uint8_t green = entry->green;
    uint8_t blue  = entry->blue;
    
    if( brightness != 255) {
        nscale8x3_video( red, green, blue, brightness);
    }
    
    return CRGB( red, green, blue);
}


void UpscalePalette(const CRGBPalette16& srcpal16, CRGBPalette256& destpal256)
{
    for( int i = 0; i < 256; i++) {
        destpal256[(uint8_t)(i)] = ColorFromPalette( srcpal16, i);
    }
}

#if 0
// replaced by PartyColors_p
void SetupPartyColors(CRGBPalette16& pal)
{
    fill_gradient( pal, 0, CHSV( HUE_PURPLE,255,255), 7, CHSV(HUE_YELLOW - 18,255,255), FORWARD_HUES);
    fill_gradient( pal, 8, CHSV( HUE_ORANGE,255,255), 15, CHSV(HUE_BLUE + 18,255,255), BACKWARD_HUES);
}
#endif

void fill_palette(CRGB* L, uint16_t N, uint8_t startIndex, uint8_t incIndex,
                  const CRGBPalette16& pal, uint8_t brightness, TBlendType blendType)
{
    uint8_t colorIndex = startIndex;
    for( uint16_t i = 0; i < N; i++) {
        L[i] = ColorFromPalette( pal, colorIndex, brightness, blendType);
        colorIndex += incIndex;
    }
}


void fill_palette(CRGB* L, uint16_t N, uint8_t startIndex, uint8_t incIndex,
                  const CRGBPalette256& pal, uint8_t brightness, TBlendType blendType)
{
    uint8_t colorIndex = startIndex;
    for( uint16_t i = 0; i < N; i++) {
        L[i] = ColorFromPalette( pal, colorIndex, brightness, blendType);
        colorIndex += incIndex;
    }
}


