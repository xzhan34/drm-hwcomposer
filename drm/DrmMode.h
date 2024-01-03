/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_DRM_MODE_H_
#define ANDROID_DRM_MODE_H_
#include <cstdint>
#pragma once

#include <xf86drmMode.h>

#include <cstdio>
#include <string>

#include "DrmUnique.h"

namespace android {

class DrmDevice;

class DrmMode {
 public:
  DrmMode() = default;
  explicit DrmMode(drmModeModeInfoPtr m);

  bool operator==(const drmModeModeInfo &m) const;

  uint32_t id() const;
  void set_id(uint32_t id);

  uint32_t clock() const;

  auto &GetRawMode() const {
    return mode_;
  }

  auto GetVRefresh() const {
    if (mode_.clock == 0) {
      return float(mode_.vrefresh);
    }
    // Always recalculate refresh to report correct float rate
    return static_cast<float>(mode_.clock) /
           (float)(mode_.vtotal * mode_.htotal) * 1000.0F;
  }

  auto GetName() const {
    return std::string(mode_.name) + "@" + std::to_string(GetVRefresh());
  }

  auto CreateModeBlob(const DrmDevice &drm) -> DrmModeUserPropertyBlobUnique;

 private:
  uint32_t id_ = 0;

  uint32_t clock_ = 0;

  uint16_t h_display_ = 0;
  uint16_t h_sync_start_ = 0;
  uint16_t h_sync_end_ = 0;
  uint16_t h_total_ = 0;
  uint16_t h_skew_ = 0;

  uint16_t v_display_ = 0;
  uint16_t v_sync_start_ = 0;
  uint16_t v_sync_end_ = 0;
  uint16_t v_total_ = 0;
  uint16_t v_scan_ = 0;
  uint16_t v_refresh_ = 0;

  uint32_t flags_ = 0;
  uint32_t type_ = 0;

  std::string name_;

  drmModeModeInfo mode_;

};
}  // namespace android

#endif  // ANDROID_DRM_MODE_H_
