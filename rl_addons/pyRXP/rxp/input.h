#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "charset.h"
#include "stdio16.h"
#include "dtd.h"

/* Typedefs */

typedef struct input_source *InputSource;

/* Input sources */

XML_API InputSource SourceFromFILE16(const char8 *description, FILE16 *file16);
XML_API InputSource SourceFromStream(const char8 *description, FILE *file);
XML_API InputSource EntityOpen(Entity e);
XML_API InputSource NewInputSource(Entity e, FILE16 *f16);
XML_API void SourceClose(InputSource source);
XML_API int SourceTell(InputSource s);
XML_API int SourceSeek(InputSource s, int byte_offset);
XML_API int SourceLineAndChar(InputSource s, int *linenum, int *charnum);
XML_API void SourcePosition(InputSource s, Entity *entity, int *byte_offset);
XML_API int get_with_fill(InputSource s);
XML_API void determine_character_encoding(InputSource s);

struct input_source {
    Entity entity;		/* The entity from which the source reads */

    FILE16 *file16;

    Char *line;
    int line_alloc, line_length;
    int next;

    int seen_eoe;
    int complicated_utf8_line;
    int bytes_consumed;
    int bytes_before_current_line;
    int line_end_was_cr;

    int line_number;
    int not_read_yet;

    struct input_source *parent;

    int nextin;
    int insize;
    unsigned char inbuf[4096];

    int seen_error;
    char error_msg[100];

    int cached_line_char;	/* cached data for SourceTell */
    int cached_line_byte;
};

/* EOE used to be -2, but that doesn't work if Char is signed char */
#define XEOE (-999)

/* Use SUB (ctl-Z) for an illegal character */
#define BADCHAR 26

#define at_eol(s) ((s)->next == (s)->line_length)
#define get(s)    (at_eol(s) ? get_with_fill(s) : (s)->line[(s)->next++])
#define unget(s)  ((s)->seen_eoe ? (s)->seen_eoe= 0 : (s)->next--)

#endif /* INPUT_H */