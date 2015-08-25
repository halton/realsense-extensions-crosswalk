// Copyright 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_ENHANCED_PHOTOGRAPHY_DEPTH_PHOTO_OBJECT_H_
#define REALSENSE_ENHANCED_PHOTOGRAPHY_DEPTH_PHOTO_OBJECT_H_

#include "pxcphoto.h" // NOLINT

// This file is auto-generated by enhanced_photography.idl
#include "depth_photo.h" // NOLINT
#include "xwalk/common/event_target.h"

namespace realsense {
namespace enhanced_photography {

using xwalk::common::XWalkExtensionFunctionInfo;
using namespace jsapi::depth_photo; // NOLINT

class DepthPhotoObject : public xwalk::common::BindingObject {
 public:
  explicit DepthPhotoObject(PXCPhoto* photo);
  ~DepthPhotoObject() override;

  PXCPhoto* GetPhoto() { return photo_; }
  void DestroyPhoto();

 private:
  void OnGetColorImage(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnGetDepthImage(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnSetColorImage(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnSetDepthImage(scoped_ptr<XWalkExtensionFunctionInfo> info);

  bool CopyColorImage(PXCImage* pxcimage, jsapi::depth_photo::Image* image);
  bool CopyDepthImage(PXCImage* pxcimage, jsapi::depth_photo::Image* image);

  PXCPhoto* photo_;
};

}  // namespace enhanced_photography
}  // namespace realsense

#endif  // REALSENSE_ENHANCED_PHOTOGRAPHY_DEPTH_PHOTO_OBJECT_H_