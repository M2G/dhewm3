#include "sys/platform.h"
#include "renderer/tr_local.h"
#include "renderer/glsl_program.h"

interactionProgram_t interactionProg;

static GLuint R_CompileShaderStage( GLenum stage, const char *source, const char *debugName ) {
	GLuint shader = qglCreateShader( stage );
	qglShaderSource( shader, 1, &source, NULL );
	qglCompileShader( shader );

	GLint compiled = GL_FALSE;
	qglGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled ) {
		char log[4096];
		qglGetShaderInfoLog( shader, sizeof(log), NULL, log );
		common->Warning( "GLSL compile error in %s:\n%s", debugName, log );
		qglDeleteShader( shader );
		return 0;
	}
	return shader;
}

static bool R_ReadTextFile( const char *relativePath, idStr &out ) {
	void *buffer = NULL;
	int len = fileSystem->ReadFile( relativePath, &buffer );
	if ( len <= 0 || buffer == NULL ) {
		common->Warning( "R_ReadTextFile: couldn't load %s", relativePath );
		return false;
	}
	out.Append( (const char *)buffer, len );
	fileSystem->FreeFile( buffer );
	return true;
}

static bool R_LoadGLSLProgram( const char *vertexPath, const char *fragmentPath, interactionProgram_t &prog ) {
	idStr vertexSrc, fragmentSrc;
	if ( !R_ReadTextFile( vertexPath, vertexSrc ) ) return false;
	if ( !R_ReadTextFile( fragmentPath, fragmentSrc ) ) return false;

	GLuint vs = R_CompileShaderStage( GL_VERTEX_SHADER, vertexSrc.c_str(), vertexPath );
	if ( !vs ) return false;

	GLuint fs = R_CompileShaderStage( GL_FRAGMENT_SHADER, fragmentSrc.c_str(), fragmentPath );
	if ( !fs ) { qglDeleteShader( vs ); return false; }

	GLuint program = qglCreateProgram();
	qglAttachShader( program, vs );
	qglAttachShader( program, fs );

	qglBindAttribLocation( program, 0, "in_position" );
	qglBindAttribLocation( program, 1, "in_color" );
	qglBindAttribLocation( program, 8, "in_texcoord" );
	qglBindAttribLocation( program, 9, "in_normal" );
	qglBindAttribLocation( program, 10, "in_tangent0" );
	qglBindAttribLocation( program, 11, "in_tangent1" );

	qglLinkProgram( program );

	GLint linked = GL_FALSE;
	qglGetProgramiv( program, GL_LINK_STATUS, &linked );
	qglDeleteShader( vs );
	qglDeleteShader( fs );
	if ( !linked ) {
		char log[4096];
		qglGetProgramInfoLog( program, sizeof(log), NULL, log );
		common->Warning( "GLSL link error (%s + %s):\n%s", vertexPath, fragmentPath, log );
		qglDeleteProgram( program );
		return false;
	}

	prog.program = program;

	prog.loc_modelViewProj  = qglGetUniformLocation( program, "u_modelViewProj" );
	prog.loc_lightOrigin    = qglGetUniformLocation( program, "u_localLightOrigin" );
	prog.loc_viewOrigin     = qglGetUniformLocation( program, "u_localViewOrigin" );
	prog.loc_lightProjS     = qglGetUniformLocation( program, "u_lightProjectionS" );
	prog.loc_lightProjT     = qglGetUniformLocation( program, "u_lightProjectionT" );
	prog.loc_lightProjQ     = qglGetUniformLocation( program, "u_lightProjectionQ" );
	prog.loc_lightFalloffS  = qglGetUniformLocation( program, "u_lightFalloffS" );
	prog.loc_bumpMatrixS    = qglGetUniformLocation( program, "u_bumpMatrixS" );
	prog.loc_bumpMatrixT    = qglGetUniformLocation( program, "u_bumpMatrixT" );
	prog.loc_diffuseMatrixS = qglGetUniformLocation( program, "u_diffuseMatrixS" );
	prog.loc_diffuseMatrixT = qglGetUniformLocation( program, "u_diffuseMatrixT" );
	prog.loc_specMatrixS    = qglGetUniformLocation( program, "u_specularMatrixS" );
	prog.loc_specMatrixT    = qglGetUniformLocation( program, "u_specularMatrixT" );
	prog.loc_colorModulate  = qglGetUniformLocation( program, "u_colorModulate" );
	prog.loc_colorAdd       = qglGetUniformLocation( program, "u_colorAdd" );
	prog.loc_diffuseModifier  = qglGetUniformLocation( program, "u_diffuseModifier" );
	prog.loc_specularModifier = qglGetUniformLocation( program, "u_specularModifier" );

	qglUseProgram( program );
	GLint loc;
	if ( (loc = qglGetUniformLocation( program, "u_bumpMap" ))       >= 0 ) qglUniform1i( loc, 1 );
	if ( (loc = qglGetUniformLocation( program, "u_falloffMap" ))    >= 0 ) qglUniform1i( loc, 2 );
	if ( (loc = qglGetUniformLocation( program, "u_projectionMap" )) >= 0 ) qglUniform1i( loc, 3 );
	if ( (loc = qglGetUniformLocation( program, "u_diffuseMap" ))    >= 0 ) qglUniform1i( loc, 4 );
	if ( (loc = qglGetUniformLocation( program, "u_specularMap" ))   >= 0 ) qglUniform1i( loc, 5 );
	qglUseProgram( 0 );

	prog.valid = true;
	return true;
}

shadowProgram_t shadowProg;

static bool R_LoadShadowGLSLProgram( const char *vertexPath, const char *fragmentPath, shadowProgram_t &prog ) {
	idStr vertexSrc, fragmentSrc;
	if ( !R_ReadTextFile( vertexPath, vertexSrc ) ) return false;
	if ( !R_ReadTextFile( fragmentPath, fragmentSrc ) ) return false;

	GLuint vs = R_CompileShaderStage( GL_VERTEX_SHADER, vertexSrc.c_str(), vertexPath );
	if ( !vs ) return false;

	GLuint fs = R_CompileShaderStage( GL_FRAGMENT_SHADER, fragmentSrc.c_str(), fragmentPath );
	if ( !fs ) { qglDeleteShader( vs ); return false; }

	GLuint program = qglCreateProgram();
	qglAttachShader( program, vs );
	qglAttachShader( program, fs );

	// un seul attribut: la position (en vec4, le .w porte le flag extrusion)
	qglBindAttribLocation( program, 0, "attr_Vertex" );

	qglLinkProgram( program );

	GLint linked = GL_FALSE;
	qglGetProgramiv( program, GL_LINK_STATUS, &linked );
	qglDeleteShader( vs );
	qglDeleteShader( fs );
	if ( !linked ) {
		char log[4096];
		qglGetProgramInfoLog( program, sizeof(log), NULL, log );
		common->Warning( "GLSL link error (%s + %s):\n%s", vertexPath, fragmentPath, log );
		qglDeleteProgram( program );
		return false;
	}

	prog.program = program;
	prog.loc_modelViewProj = qglGetUniformLocation( program, "u_modelViewProjectionMatrix" );
	prog.loc_localLightOrigin = qglGetUniformLocation( program, "u_lightOrigin" );
	prog.valid = true;

	return true;
}

void R_InitGLSLPrograms( void ) {
	memset( &interactionProg, 0, sizeof(interactionProg) );
	R_LoadGLSLProgram(
		"glprogs/interaction_dhewm3.vertex.glsl",
		"glprogs/interaction_dhewm3.pixel.glsl",
		interactionProg
	);

	memset( &shadowProg, 0, sizeof(shadowProg) );
	R_LoadShadowGLSLProgram(
		"glprogs/shadow_dhewm3.vertex.glsl",
		"glprogs/shadow_dhewm3.pixel.glsl",
		shadowProg
	);
}