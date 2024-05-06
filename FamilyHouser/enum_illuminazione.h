typedef enum {
	NO_NONE,
	PHONG,
	BLINN,
} ShadingType;
typedef enum {
	RED_PLASTIC,
	EMERALD,
	BRASS,
	SLATE,
	YELLOW,
	NO_MATERIAL
} MaterialType;

typedef enum {
	NONE,
	INTERPOLATING_SHADING,
	PHONG_SHADING,
	NO_TEXTURE,
	WAVE
} ShaderOption;
typedef enum {
	WIRE_FRAME,
	FACE_FILL,
	CULLING_ON,
	CULLING_OFF,
}MenuOption;