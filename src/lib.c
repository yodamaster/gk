/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "common.h"
#include "model.h"
#include "common.h"

#include "shader/shader.h"
#include "program/program.h"
#include "program/vertex_input.h"

#include "vertex.h"

void
GK_CONSTRUCTOR
gk__init(void);

void
GK_DESTRUCTOR
gk__cleanup(void);

void
GK_CONSTRUCTOR
gk__init() {
  gk_verta_init();
  gk_vertinp_init();
  gk_shaders_init();
  gk_prog_init();
}

void
GK_DESTRUCTOR
gk__cleanup() {
  gk_prog_deinit();
  gk_shaders_deinit();
  gk_vertinp_deinit();
  gk_verta_deinit();
}
