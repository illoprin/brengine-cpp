// Font rasterizer test

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_truetype.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>

int main()
{
	/* load font file */
    long size;
    unsigned char* fontBuffer;
    
    FILE* fontFile = fopen("assets/fonts/AnotherCastle.ttf", "rb");
    fseek(fontFile, 0, SEEK_END);
    size = ftell(fontFile); /* how long is the file ? */
    fseek(fontFile, 0, SEEK_SET); /* reset */
    
    fontBuffer = (unsigned char*)malloc(size);
    
    fread(fontBuffer, size, 1, fontFile);
    fclose(fontFile);

    /* prepare font */
    stbtt_fontinfo info;
    if (!stbtt_InitFont(&info, fontBuffer, 0))
    {
        printf("failed\n");
    };
	printf("Font loaded!\n");
    
    int b_w = 512; /* bitmap width */
    int b_h = 512; /* bitmap height */
    int l_h = 20; /* line height */

    /* create a bitmap for the phrase */
    unsigned char* bitmap = (unsigned char*)calloc(b_w * b_h, sizeof(unsigned char));
    
    /* calculate font scaling */
    float scale = stbtt_ScaleForPixelHeight(&info, l_h);

    int x = 0;
       
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    
    ascent = roundf(ascent * scale);
    descent = roundf(descent * scale);
    
    unsigned char i;
    for (i = 0; i < 128; ++i)
    {
        /* how wide is this character */
        int ax;
		int lsb;
        stbtt_GetCodepointHMetrics(&info, i, &ax, &lsb);
        /* (Note that each Codepoint call has an alternative Glyph version which caches the work required to lookup the character word[i].) */

        /* get bounding box for character (may be offset to account for chars that dip above or below the line) */
        int c_x1, c_y1, c_x2, c_y2;
        stbtt_GetCodepointBitmapBox(&info, i, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
        
        /* compute y (different characters have different heights) */
        int y = ascent + c_y1;
        
        /* render character (stride and offset is important here) */
        int byteOffset = x + roundf(lsb * scale) + (y * b_w);
        stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, b_w, scale, scale, i);

        /* advance x */
        x += roundf(ax * scale);
        
        /* add kerning */
        int kern;
        kern = stbtt_GetCodepointKernAdvance(&info, i, i + 1);
        x += roundf(kern * scale);
    }
    
    /* save out a 1 channel image */
    stbi_write_png("tmp/font_bitmap.png", b_w, b_h, 1, bitmap, b_w);
	printf("Bitmap writed!\n");
	
    /*
     Note that this example writes each character directly into the target image buffer.
     The "right thing" to do for fonts that have overlapping characters is
     MakeCodepointBitmap to a temporary buffer and then alpha blend that onto the target image.
     See the stb_truetype.h header for more info.
    */
    
    free(fontBuffer);
    free(bitmap);
	return 0;
};
