/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#ifndef gk_program_h
#define gk_program_h
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "shader.h"
#include "uniform.h"

#include <ds/rb.h>
#include <ds/hash.h>
#include <ds/forward-list-sep.h>
  
#include <OpenGL/gl3.h>
#include <stdio.h>
#include <stdbool.h>

struct GkMaterial;
struct GkLight;
struct GkShader;
struct GkScene;
struct GkContext;

typedef struct GkProgram {
  FListItem         *vertex;
  HTable            *uniforms;
  struct GkShader   *shaders;
  struct GkMaterial *lastMaterial;
  struct GkLight    *lastLight;
  uint32_t           refc;
  GLint              progId;
  GLint              mvpi;
  GLint              mvi;
  GLint              nmi;
  GLint              nmui;
  bool               updtLights;
  bool               updtMaterials;
} GkProgram;

void
gkProgramLogInfo(GLuint progId, FILE * __restrict file);

bool
gkProgramIsValid(GLuint progId);

GkProgram*
gkMakeProgram(GkShader *shaders,
              void (*beforeLinking)(GkProgram *prog, void *data),
              void *userData);

GkProgram*
gkDefaultProgram(void);

GLint
gkCurrentProgram(void);

GK_EXPORT
void
gkUseProgram(struct GkContext *ctx,
             GkProgram        *prog);

GkProgram*
gkGetOrCreatProg(char      *name,
                 GkProgram *(creatCb)(char *name, void *userData),
                 void      *userData);

#ifdef __cplusplus
}
#endif
#endif /* gk_program_h */
