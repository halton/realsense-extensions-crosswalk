// Copyright (c) 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "realsense/enhanced_photography/win/enhanced_photography_instance.h"

#include <string>

// This file is auto-generated by depth_mask.idl
#include "depth_mask.h"  // NOLINT
// This file is auto-generated by depth_refocus.idl
#include "depth_refocus.h"  // NOLINT
// This file is auto-generated by measurement.idl
#include "measurement.h"  // NOLINT
// This file is auto-generated by motion_effect.idl
#include "motion_effect.h"  // NOLINT
// This file is auto-generated by paster.idl
#include "paster.h" // NOLINT
// This file is auto-generated by photo_utils.idl
#include "photo_utils.h" // NOLINT
// This file is auto-generated by segmentation.idl
#include "segmentation.h" // NOLINT
// This file is auto-generated by xdm_utils.idl
#include "xdm_utils.h" // NOLINT

#include "base/json/json_string_value_serializer.h"
#include "realsense/enhanced_photography/win/depth_mask_object.h"
#include "realsense/enhanced_photography/win/depth_photo_object.h"
#include "realsense/enhanced_photography/win/depth_refocus_object.h"
#include "realsense/enhanced_photography/win/measurement_object.h"
#include "realsense/enhanced_photography/win/motion_effect_object.h"
#include "realsense/enhanced_photography/win/paster_object.h"
#include "realsense/enhanced_photography/win/photo_capture_object.h"
#include "realsense/enhanced_photography/win/photo_utils_object.h"
#include "realsense/enhanced_photography/win/segmentation_object.h"
#include "realsense/enhanced_photography/win/xdm_utils_object.h"

namespace realsense {
namespace enhanced_photography {

using namespace xwalk::common; // NOLINT

EnhancedPhotographyInstance::EnhancedPhotographyInstance()
    : handler_(this),
      session_(nullptr),
      store_(&handler_),
      ep_ext_thread_("EPExtensionThread") {
  ep_ext_thread_.Start();
  handler_.Register("measurementConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnMeasurementConstructor,
                 base::Unretained(this)));
  handler_.Register("depthMaskConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnDepthMaskConstructor,
                 base::Unretained(this)));
  handler_.Register("depthPhotoConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnDepthPhotoConstructor,
                 base::Unretained(this)));
  handler_.Register("depthRefocusConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnDepthRefocusConstructor,
                 base::Unretained(this)));
  handler_.Register("motionEffectConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnMotionEffectConstructor,
                 base::Unretained(this)));
  handler_.Register("pasterConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnPasterConstructor,
                 base::Unretained(this)));
  handler_.Register("photoCaptureConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnPhotoCaptureConstructor,
                 base::Unretained(this)));
  handler_.Register("photoUtilsConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnPhotoUtilsConstructor,
                 base::Unretained(this)));
  handler_.Register("segmentationConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnSegmentationConstructor,
                 base::Unretained(this)));
  handler_.Register("XDMUtilsConstructor",
      base::Bind(&EnhancedPhotographyInstance::OnXDMUtilsConstructor,
                 base::Unretained(this)));
}

EnhancedPhotographyInstance::~EnhancedPhotographyInstance() {
  if (session_)
    session_->Release();
  ep_ext_thread_.Stop();
}

void EnhancedPhotographyInstance::HandleMessage(const char* msg) {
  JSONStringValueDeserializer str_deserializer(msg);

  int error_code = 0;
  std::string error_message;
  scoped_ptr<base::Value> value(
      str_deserializer.Deserialize(&error_code, &error_message));
  CHECK(value.get());
  CHECK_EQ(0, error_code);
  CHECK(error_message.empty());

  ep_ext_thread_.message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&EnhancedPhotographyInstance::OnHandleMessage,
                 base::Unretained(this),
                 base::Passed(&value)));
}

void EnhancedPhotographyInstance::HandleBinaryMessage(
    const char* msg, const size_t size) {
  scoped_ptr<base::Value> value = scoped_ptr<base::Value>(
      base::BinaryValue::CreateWithCopiedBuffer(msg, size));

  ep_ext_thread_.message_loop()->PostTask(
    FROM_HERE,
    base::Bind(&EnhancedPhotographyInstance::OnHandleBinaryMessage,
               base::Unretained(this),
               base::Passed(&value)));
}

void EnhancedPhotographyInstance::HandleSyncMessage(const char* msg) {
  JSONStringValueDeserializer str_deserializer(msg);

  int error_code = 0;
  std::string error_message;
  scoped_ptr<base::Value> value(
      str_deserializer.Deserialize(&error_code, &error_message));
  CHECK(value.get());
  CHECK_EQ(0, error_code);
  CHECK(error_message.empty());

  ep_ext_thread_.message_loop()->PostTask(
      FROM_HERE,
      base::Bind(&EnhancedPhotographyInstance::OnHandleSyncMessage,
                 base::Unretained(this),
                 base::Passed(&value)));
}

bool EnhancedPhotographyInstance::IsRSSDKInstalled() {
  if (session_)
    return true;

  session_ = PXCSession::CreateInstance();
  if (!session_) {
    return false;
  }
  return true;
}

void EnhancedPhotographyInstance::OnHandleMessage(scoped_ptr<base::Value> msg) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());
  handler_.HandleMessage(msg.Pass());
}

void EnhancedPhotographyInstance::OnHandleBinaryMessage(
    scoped_ptr<base::Value> msg) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());
  handler_.HandleBinaryMessage(msg.Pass());
}

void EnhancedPhotographyInstance::OnHandleSyncMessage(
    scoped_ptr<base::Value> msg) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());
  handler_.HandleSyncMessage(msg.Pass());
}

void EnhancedPhotographyInstance::OnMeasurementConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::measurement::MeasurementConstructor::
      Params> params(jsapi::measurement::MeasurementConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new MeasurementObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnDepthMaskConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::depth_mask::DepthMaskConstructor::
      Params> params(jsapi::depth_mask::DepthMaskConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new DepthMaskObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnDepthPhotoConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::depth_photo::DepthPhotoConstructor::
      Params> params(jsapi::depth_photo::DepthPhotoConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new DepthPhotoObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnDepthRefocusConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::depth_refocus::DepthRefocusConstructor::
      Params> params(jsapi::depth_refocus::DepthRefocusConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new DepthRefocusObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnMotionEffectConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::motion_effect::MotionEffectConstructor::
      Params> params(jsapi::motion_effect::MotionEffectConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new MotionEffectObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnPasterConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::paster::PasterConstructor::
      Params> params(jsapi::paster::PasterConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new PasterObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnPhotoCaptureConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::photo_capture::PhotoCaptureConstructor::
      Params> params(jsapi::photo_capture::PhotoCaptureConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new PhotoCaptureObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnPhotoUtilsConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());
  scoped_ptr<jsapi::photo_utils::PhotoUtilsConstructor::
      Params> params(jsapi::photo_utils::PhotoUtilsConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new PhotoUtilsObject(this, IsRSSDKInstalled()));
  store_.AddBindingObject(params->object_id, obj.Pass());
}

void EnhancedPhotographyInstance::OnSegmentationConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());

  scoped_ptr<base::Value> result(new base::FundamentalValue(false));
  if (!IsRSSDKInstalled()) {
    info->PostResult(result.Pass());
    return;
  }

  scoped_ptr<jsapi::segmentation::SegmentationConstructor::
      Params> params(jsapi::segmentation::SegmentationConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new SegmentationObject(this));
  store_.AddBindingObject(params->object_id, obj.Pass());

  result.reset(new base::FundamentalValue(true));
  info->PostResult(result.Pass());
}

void EnhancedPhotographyInstance::OnXDMUtilsConstructor(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  DCHECK_EQ(ep_ext_thread_.message_loop(), base::MessageLoop::current());
  scoped_ptr<jsapi::xdm_utils::XDMUtilsConstructor::
      Params> params(jsapi::xdm_utils::XDMUtilsConstructor::
          Params::Create(*info->arguments()));

  scoped_ptr<BindingObject> obj(new XDMUtilsObject(this, IsRSSDKInstalled()));
  store_.AddBindingObject(params->object_id, obj.Pass());
}

void EnhancedPhotographyInstance::AddBindingObject(const std::string& object_id,
    scoped_ptr<BindingObject> obj) {
  store_.AddBindingObject(object_id, obj.Pass());
}

BindingObject* EnhancedPhotographyInstance::GetBindingObjectById(
    const std::string& object_id) {
  return store_.GetBindingObjectById(object_id);
}

}  // namespace enhanced_photography
}  // namespace realsense
