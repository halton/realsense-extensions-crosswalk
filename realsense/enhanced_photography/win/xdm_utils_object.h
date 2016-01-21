// Copyright 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_XDM_UTILS_OBJECT_H_
#define REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_XDM_UTILS_OBJECT_H_

// This file is auto-generated by xdm_utils.idl
#include "xdm_utils.h" // NOLINT

#include <string>

#include "base/files/file_path.h"
#include "realsense/enhanced_photography/win/enhanced_photography_instance.h"
#include "third_party/libpxc/include/pxcenhancedphoto.h"
#include "third_party/libpxc/include/pxcsession.h"
#include "third_party/libpxc/include/pxcphoto.h"

namespace realsense {
namespace enhanced_photography {

using xwalk::common::XWalkExtensionFunctionInfo;
using namespace jsapi::xdm_utils; // NOLINT

class XDMUtilsObject : public xwalk::common::BindingObject {
 public:
  explicit XDMUtilsObject(EnhancedPhotographyInstance* instance,
                          bool isRSSDKInstalled);
  ~XDMUtilsObject() override;

 private:
  void OnIsXDM(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnLoadXDM(scoped_ptr<XWalkExtensionFunctionInfo> info);
  void OnSaveXDM(scoped_ptr<XWalkExtensionFunctionInfo> info);

  void CreateFileWithBinaryValue(const base::BinaryValue& value,
                                 base::FilePath* file_path);

  bool isRSSDKInstalled_;
  EnhancedPhotographyInstance* instance_;
  PXCSession* session_;
  scoped_ptr<uint8[]> binary_message_;
  size_t binary_message_size_;
};

}  // namespace enhanced_photography
}  // namespace realsense

#endif  // REALSENSE_ENHANCED_PHOTOGRAPHY_WIN_XDM_UTILS_OBJECT_H_
