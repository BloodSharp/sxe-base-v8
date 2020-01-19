void PM_InitTextureTypes(struct playermove_s *ppmove);
char GetTextureType(pmtrace_t *tr, float *vecSrc, float *vecEnd);

#define CTEXTURESMAX		512			// max number of textures loaded
#define CBTEXTURENAMEMAX	13			// only load first n chars of name

#define CHAR_TEX_CONCRETE	'C'			// texture types
#define CHAR_TEX_METAL		'M'
#define CHAR_TEX_DIRT		'D'
#define CHAR_TEX_VENT		'V'
#define CHAR_TEX_GRATE		'G'
#define CHAR_TEX_TILE		'T'
#define CHAR_TEX_SLOSH		'S'
#define CHAR_TEX_WOOD		'W'
#define CHAR_TEX_COMPUTER	'P'
#define CHAR_TEX_GLASS		'Y'
#define CHAR_TEX_FLESH		'F'