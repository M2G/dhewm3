#ifndef __GLSL_PROGRAM_H__
#define __GLSL_PROGRAM_H__

struct interactionProgram_t {
	GLuint	program;

	GLint	loc_modelViewProj;
	GLint	loc_lightOrigin;
	GLint	loc_viewOrigin;
	GLint	loc_lightProjS;
	GLint	loc_lightProjT;
	GLint	loc_lightProjQ;
	GLint	loc_lightFalloffS;
	GLint	loc_bumpMatrixS;
	GLint	loc_bumpMatrixT;
	GLint	loc_diffuseMatrixS;
	GLint	loc_diffuseMatrixT;
	GLint	loc_specMatrixS;
	GLint	loc_specMatrixT;
	GLint	loc_colorModulate;
	GLint	loc_colorAdd;

	GLint	loc_diffuseModifier;
	GLint	loc_specularModifier;

	GLint	loc_bumpMap;
	GLint	loc_falloffMap;
	GLint	loc_projectionMap;
	GLint	loc_diffuseMap;
	GLint	loc_specularMap;

	bool	valid;
};

void R_InitGLSLPrograms( void );

extern interactionProgram_t interactionProg;

struct shadowProgram_t {
	GLuint	program;
	GLint	loc_modelViewProj;
	GLint	loc_localLightOrigin;
	bool	valid;
};

extern shadowProgram_t shadowProg;

struct diffuseMapProgram_t {
	GLuint	program;
	GLint	loc_modelViewProj;
	GLint	loc_colorModulate;
	GLint	loc_colorAdd;
	GLint	loc_glColor;
	bool	valid;
};

extern diffuseMapProgram_t diffuseMapProg;

#endif
