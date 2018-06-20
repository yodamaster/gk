/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "../common.h"
#include "../../include/gk/gk.h"
#include "../../include/gk/trackball.h"
#include "tball.h"
#include "mouse.h"

void
gkTrackballMouse(void         *sender,
                 GkScene      *scene,
                 GkMouseButton button,
                 GkMouseState  state,
                 GkPoint       point) {
  gkTrackballMouseWs(&(GkMouseEventStruct) {
    sender,
    scene,
    button,
    state,
    point
  });
}

void
gkTrackballMouseWs(GkMouseEventStruct *event) {
  GkTrackball *tball;
  GkScene     *scene;
  GkSceneImpl *sceneImpl;

  tball = event->sender;
  if (event->button != GK_MOUSE_LEFT)
    return;

  scene     = tball->scene;
  sceneImpl = (GkSceneImpl *)scene;
  switch (event->state) {
    case GK_MOUSE_DOWN:
      tball->start  = event->point;
      tball->moving = true;

      if (tball->cb)
        tball->cb(tball, GK_TRACKBALL_EVENT_BEGIN);

      gkTrackballVec(tball, tball->start, tball->from);

      break;
    case GK_MOUSE_MOVE:
      if (tball->moving == true) {
        vec3  axis, to;
        float angle;

        gkTrackballVec(tball, event->point, to);
        glm_vec_cross(tball->from, to, axis);

        angle = glm_vec_angle(tball->from, to) * tball->velocity;

        glm_vec_rotate_m4(scene->camera->world, axis, axis);
        glm_rotate_atm(tball->trans, tball->center, angle, axis);

        glm_mat4_mul(tball->trans,
                     scene->trans->local,
                     scene->trans->world);

        tball->scene->trans->flags &= ~GK_TRANSF_WORLD_ISVALID;
        scene->flags |= GK_SCENEF_RENDER;
      }
      break;
    case GK_MOUSE_UP: {
      glm_mat4_mul(tball->trans,
                   scene->trans->local,
                   scene->trans->local);
      tball->scene->trans->flags &= ~GK_TRANSF_WORLD_ISVALID;
      scene->flags |= GK_SCENEF_RENDER;

      if (tball->cb)
        tball->cb(tball, GK_TRACKBALL_EVENT_END);
      break;
    }
  }
}