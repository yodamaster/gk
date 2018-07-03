/*
 * Copyright (c), Recep Aslantas.
 *
 * MIT License (MIT), http://opensource.org/licenses/MIT
 * Full license can be found in the LICENSE file
 */

#include "rn_animator.h"
#include "../../../include/gk/gk.h"
#include "../../../include/gk/animation.h"
#include "../../../include/gk/animation-keyframe.h"

#include <tm/tm.h>
#include <limits.h>

void
gkRunAnim(GkSceneImpl *sceneImpl) {
  FListItem   *animItem;
  GkAnimation *anim;
  GkValue      v, vd;
  tm_interval  time, endTime;
  float        t, ease;

  if (!(animItem = sceneImpl->anims))
    return;

  time  = sceneImpl->pub.startTime;
  v.val = vd.val = NULL;

  do {
    anim = animItem->data;

    /* 0 means that animation must start with scene rendering */
    if (anim->beginTime == 0)
      anim->beginTime = time;

    /* remove animation */
    if (anim->beginTime > time
        || anim->nRepeat <= anim->nPlayed) {
      continue;
    }

    endTime = anim->beginTime + anim->duration;
    t       = glm_percentc(anim->beginTime, endTime, time);
    ease    = anim->fnTiming ? anim->fnTiming(t) : t;

    if (anim->isKeyFrame) {
      GkKeyFrameAnimation *kfa;

      kfa = (GkKeyFrameAnimation *)anim;

      if (!anim->isReverse) {
        gkInterpolate(0,
                      ease,
                      anim->from,
                      anim->to,
                      &v);
      } else {
        gkInterpolate(0,
                      ease,
                      anim->to,
                      anim->from,
                      &v);
      }
    } else {
      if (!anim->isReverse) {
        gkValueLerp(anim->from, anim->to, ease, &v);
      } else {
        gkValueLerp(anim->to, anim->from, ease, &v);
      }
    }

    gkValueSub(&v, anim->delta, &vd);
    gkValueCopy(&v, anim->delta);

    anim->fnAnimator(anim, &v, &vd);

    if (t == 1.0f) {
      if (!anim->autoReverse) {
        anim->nPlayed++;
      } else {
        if (anim->isReverse)
          anim->nPlayed++;

        if (anim->nPlayed < anim->nRepeat)
          anim->beginTime = time;

        anim->isReverse = !anim->isReverse;
      }

      if (anim->nRepeat == UINT_MAX)
        anim->beginTime = time;
    }
  } while ((animItem = animItem->next));
}