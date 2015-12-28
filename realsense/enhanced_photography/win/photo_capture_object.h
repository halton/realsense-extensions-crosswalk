// Copyright 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_PHOTO_CAPTURE_OBJECT_H_
#define REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_PHOTO_CAPTURE_OBJECT_H_

#include <string>
#include <vector>

// This file is auto-generated by photo_capture.idl
#include "photo_capture.h" // NOLINT

#include "base/message_loop/message_loop_proxy.h"
#include "base/synchronization/lock.h"
#include "base/threading/thread.h"
#include "realsense/enhanced_photography/win/enhanced_photography_instance.h"
#include "third_party/libpxc/include/pxcphoto.h"
#include "third_party/libpxc/include/pxcsensemanager.h"
#include "xwalk/common/event_target.h"

namespace realsense {
namespace enhanced_photography {

using xwalk::common::XWalkExtensionFunctionInfo;
using namespace jsapi::photo_capture; // NOLINT

class PhotoCaptureObject : public xwalk::common::EventTarget {
 public:
  explicit PhotoCaptureObject(EnhancedPhotographyInstance* instance);
  ~PhotoCaptureObject() override;

  // EventTarget implementation.
  void StartEvent(const std::string& type) override;
  void StopEvent(const std::string& type) override;

 private:
  void OnStartPreview(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnStopPreview(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnGetPreviewImage(scoped_ptr<XWalkExtensionFunctionInfo> info);

  // This method will capture a photo from preview and bind it with |photo_|
  void OnTakePhoto(scoped_ptr<XWalkExtensionFunctionInfo> info);

  void ReleaseMainResources();
  void ReleasePreviewResources();

  // Run on ep_preview_thread_
  void OnEnhancedPhotoPreviewPipeline();
  void CaptureOnPreviewThread(
      scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnStopAndDestroyPipeline(
      scoped_ptr<XWalkExtensionFunctionInfo> info);

  enum State {
    IDLE,
    PREVIEW,
  };
  State state_;

  bool on_preview_;

  base::Lock lock_;
  base::Thread ep_preview_thread_;
  scoped_refptr<base::MessageLoopProxy> message_loop_;

  PXCSession* session_;
  PXCSenseManager* sense_manager_;
  PXCPhoto* preview_photo_;
  PXCImage* preview_image_;

  EnhancedPhotographyInstance* instance_;

  scoped_ptr<uint8[]> binary_message_;
  size_t binary_message_size_;
};

}  // namespace enhanced_photography
}  // namespace realsense

#endif  // REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_PHOTO_CAPTURE_OBJECT_H_
