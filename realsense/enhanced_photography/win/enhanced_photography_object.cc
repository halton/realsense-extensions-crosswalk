// Copyright (c) 2015 Intel Corporation. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "realsense/enhanced_photography/win/enhanced_photography_object.h"

#include <algorithm>
#include <string>

#include "base/bind.h"
#include "base/guid.h"
#include "base/logging.h"
#include "realsense/enhanced_photography/win/depth_photo_object.h"

namespace realsense {
namespace enhanced_photography {

EnhancedPhotographyObject::EnhancedPhotographyObject(
    EnhancedPhotographyInstance* instance)
        : session_(nullptr),
          ep_(nullptr),
          instance_(instance),
          binary_message_size_(0) {
  handler_.Register("measureDistance",
                    base::Bind(&EnhancedPhotographyObject::OnMeasureDistance,
                               base::Unretained(this)));
  handler_.Register("depthRefocus",
                    base::Bind(&EnhancedPhotographyObject::OnDepthRefocus,
                               base::Unretained(this)));
  handler_.Register("computeMaskFromCoordinate",
      base::Bind(&EnhancedPhotographyObject::OnComputeMaskFromCoordinate,
                 base::Unretained(this)));
  handler_.Register("computeMaskFromThreshold",
      base::Bind(&EnhancedPhotographyObject::OnComputeMaskFromThreshold,
                 base::Unretained(this)));

  session_ = PXCSession::CreateInstance();
  session_->CreateImpl<PXCEnhancedPhoto>(&ep_);
}

EnhancedPhotographyObject::~EnhancedPhotographyObject() {
  ReleaseResources();
}

void EnhancedPhotographyObject::CreateDepthPhotoObject(
    PXCPhoto* pxcphoto, jsapi::depth_photo::Photo* photo) {
  DepthPhotoObject* depthPhotoObject = new DepthPhotoObject(instance_);
  depthPhotoObject->GetPhoto()->CopyPhoto(pxcphoto);
  scoped_ptr<BindingObject> obj(depthPhotoObject);
  std::string object_id = base::GenerateGUID();
  instance_->AddBindingObject(object_id, obj.Pass());
  photo->object_id = object_id;
}

void EnhancedPhotographyObject::OnMeasureDistance(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  MeasureData measure_data;
  scoped_ptr<MeasureDistance::Params> params(
      MeasureDistance::Params::Create(*info->arguments()));
  if (!params) {
    info->PostResult(
        MeasureDistance::Results::Create(measure_data, "Malformed parameters"));
    return;
  }

  std::string object_id = params->photo.object_id;
  DepthPhotoObject* depthPhotoObject = static_cast<DepthPhotoObject*>(
      instance_->GetBindingObjectById(object_id));
  if (!depthPhotoObject || !depthPhotoObject->GetPhoto()) {
    info->PostResult(MeasureDistance::Results::Create(measure_data,
        "Invalid Photo object."));
    return;
  }

  DCHECK(ep_);
  PXCPointI32 start;
  PXCPointI32 end;
  start.x = params->start.x;
  start.y = params->start.y;
  end.x = params->end.x;
  end.y = params->end.y;
  PXCEnhancedPhoto::MeasureData data;
  ep_->MeasureDistance(depthPhotoObject->GetPhoto(), start, end, &data);

  measure_data.distance = data.distance;
  measure_data.confidence = data.confidence;
  measure_data.precision = data.precision;
  measure_data.start_point.x = data.startPoint.coord.x;
  measure_data.start_point.y = data.startPoint.coord.y;
  measure_data.start_point.z = data.startPoint.coord.z;
  measure_data.end_point.x = data.endPoint.coord.x;
  measure_data.end_point.x = data.endPoint.coord.y;
  measure_data.end_point.x = data.endPoint.coord.z;
  info->PostResult(MeasureDistance::Results::Create(
      measure_data, std::string()));
}

void EnhancedPhotographyObject::OnDepthRefocus(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  jsapi::depth_photo::Photo photo;
  scoped_ptr<DepthRefocus::Params> params(
      DepthRefocus::Params::Create(*info->arguments()));
  if (!params) {
    info->PostResult(
        DepthRefocus::Results::Create(photo, "Malformed parameters"));
    return;
  }

  std::string object_id = params->photo.object_id;
  DepthPhotoObject* depthPhotoObject = static_cast<DepthPhotoObject*>(
      instance_->GetBindingObjectById(object_id));
  if (!depthPhotoObject || !depthPhotoObject->GetPhoto()) {
    info->PostResult(DepthRefocus::Results::Create(photo,
        "Invalid Photo object."));
    return;
  }

  DCHECK(ep_);
  PXCPointI32 focus;
  focus.x = params->focus.x;
  focus.y = params->focus.y;

  PXCPhoto* pxcphoto;
  if (params->aperture)
    pxcphoto = ep_->DepthRefocus(depthPhotoObject->GetPhoto(),
                                 focus,
                                 *(params->aperture.get()));
  else
    pxcphoto = ep_->DepthRefocus(depthPhotoObject->GetPhoto(), focus);
  if (!pxcphoto) {
    info->PostResult(DepthRefocus::Results::Create(photo,
        "Failed to operate DepthRefocus"));
    return;
  }

  CreateDepthPhotoObject(pxcphoto, &photo);
  info->PostResult(DepthRefocus::Results::Create(photo, std::string()));
  pxcphoto->Release();
}

void EnhancedPhotographyObject::OnComputeMaskFromCoordinate(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  jsapi::depth_photo::MaskImage image;
  scoped_ptr<ComputeMaskFromCoordinate::Params> params(
      ComputeMaskFromCoordinate::Params::Create(*info->arguments()));
  if (!params) {
    info->PostResult(ComputeMaskFromCoordinate::Results::Create(
        image, "Malformed parameters"));
    return;
  }

  std::string object_id = params->photo.object_id;
  DepthPhotoObject* depthPhotoObject = static_cast<DepthPhotoObject*>(
      instance_->GetBindingObjectById(object_id));
  if (!depthPhotoObject || !depthPhotoObject->GetPhoto()) {
    info->PostResult(ComputeMaskFromCoordinate::Results::Create(image,
        "Invalid Photo object."));
    return;
  }

  DCHECK(ep_);
  PXCPointI32 point;
  point.x = params->point.x;
  point.y = params->point.y;

  PXCImage* pxcimage;
  if (params->params) {
    PXCEnhancedPhoto::MaskParams mask_params;

    mask_params.frontObjectDepth = params->params->front_object_depth;
    mask_params.backOjectDepth = params->params->back_object_depth;
    mask_params.nearFallOffDepth = params->params->near_fall_off_depth;
    mask_params.farFallOffDepth = params->params->far_fall_off_depth;
    pxcimage = ep_->ComputeMaskFromCoordinate(depthPhotoObject->GetPhoto(),
                                              point,
                                              &mask_params);
  } else {
    pxcimage = ep_->ComputeMaskFromCoordinate(depthPhotoObject->GetPhoto(),
                                              point);
  }

  if (!CopyMaskImage(pxcimage)) {
    info->PostResult(ComputeMaskFromCoordinate::Results::Create(image,
        "Failed to get image data."));
    return;
  }

  scoped_ptr<base::ListValue> result(new base::ListValue());
  result->Append(base::BinaryValue::CreateWithCopiedBuffer(
      reinterpret_cast<const char*>(binary_message_.get()),
      binary_message_size_));
  info->PostResult(result.Pass());

  pxcimage->Release();
}

void EnhancedPhotographyObject::OnComputeMaskFromThreshold(
    scoped_ptr<XWalkExtensionFunctionInfo> info) {
  jsapi::depth_photo::MaskImage image;
  scoped_ptr<ComputeMaskFromThreshold::Params> params(
      ComputeMaskFromThreshold::Params::Create(*info->arguments()));
  if (!params) {
    info->PostResult(ComputeMaskFromThreshold::Results::Create(
        image, "Malformed parameters"));
    return;
  }

  std::string object_id = params->photo.object_id;
  DepthPhotoObject* depthPhotoObject = static_cast<DepthPhotoObject*>(
      instance_->GetBindingObjectById(object_id));
  if (!depthPhotoObject || !depthPhotoObject->GetPhoto()) {
    info->PostResult(ComputeMaskFromThreshold::Results::Create(image,
        "Invalid Photo object."));
    return;
  }

  DCHECK(ep_);
  PXCImage* pxcimage;
  if (params->params) {
    PXCEnhancedPhoto::MaskParams mask_params;
    mask_params.frontObjectDepth = params->params->front_object_depth;
    mask_params.backOjectDepth = params->params->back_object_depth;
    mask_params.nearFallOffDepth = params->params->near_fall_off_depth;
    mask_params.farFallOffDepth = params->params->far_fall_off_depth;
    pxcimage = ep_->ComputeMaskFromThreshold(depthPhotoObject->GetPhoto(),
                                             params->threshold,
                                             &mask_params);
  } else {
    pxcimage = ep_->ComputeMaskFromThreshold(depthPhotoObject->GetPhoto(),
                                             params->threshold);
  }

  if (!CopyMaskImage(pxcimage)) {
    info->PostResult(ComputeMaskFromThreshold::Results::Create(image,
      "Failed to get image data."));
    return;
  }

  scoped_ptr<base::ListValue> result(new base::ListValue());
  result->Append(base::BinaryValue::CreateWithCopiedBuffer(
      reinterpret_cast<const char*>(binary_message_.get()),
      binary_message_size_));
  info->PostResult(result.Pass());

  pxcimage->Release();
}

bool EnhancedPhotographyObject::CopyMaskImage(PXCImage* mask) {
  if (!mask) return false;

  PXCImage::ImageInfo mask_info = mask->QueryInfo();
  PXCImage::ImageData mask_data;
  if (mask->AcquireAccess(PXCImage::ACCESS_READ,
      mask_info.format, &mask_data) < PXC_STATUS_NO_ERROR) {
    return false;
  }

  size_t requset_size;
  int k = 0;
  if (mask_info.format == PXCImage::PixelFormat::PIXEL_FORMAT_Y8) {
    // binary image message: call_id (i32), width (i32), height (i32),
    // mask data (int8 buffer, size = width * height)
    requset_size = 4 * 3 + mask_info.width * mask_info.height;
    binary_message_.reset(new uint8[requset_size]);
    binary_message_size_ = requset_size;

    int* int_array = reinterpret_cast<int*>(binary_message_.get());
    int_array[1] = mask_info.width;
    int_array[2] = mask_info.height;

    uint8_t* uint8_data_array = reinterpret_cast<uint8_t*>(
        binary_message_.get() + 3 * sizeof(int));
    for (int y = 0; y < mask_info.height; ++y) {
      for (int x = 0; x < mask_info.width; ++x) {
        uint8_t* depth8 = reinterpret_cast<uint8_t*>(
            mask_data.planes[0] + mask_data.pitches[0] * y);
        uint8_data_array[k++] = depth8[x];
      }
    }
  } else if (mask_info.format ==
      PXCImage::PixelFormat::PIXEL_FORMAT_DEPTH_F32) {
    // binary image message: call_id (i32), width (i32), height (i32),
    // mask data (float_t buffer, size = width * height *4)
    requset_size = 3 * 4 + mask_info.width * mask_info.height * 4;
    binary_message_.reset(new uint8[requset_size]);
    binary_message_size_ = requset_size;

    int* int_array = reinterpret_cast<int*>(binary_message_.get());
    int_array[1] = mask_info.width;
    int_array[2] = mask_info.height;

    float_t* float_data_array = reinterpret_cast<float_t*>(
        binary_message_.get() + 3 * sizeof(int));
    for (int y = 0; y < mask_info.height; ++y) {
      for (int x = 0; x < mask_info.width; ++x) {
        float_t* depth32 = reinterpret_cast<float_t*>(
            mask_data.planes[0] + mask_data.pitches[0] * y);
        float_data_array[k++] = depth32[x];
      }
    }
  }

  mask->ReleaseAccess(&mask_data);
  return true;
}

void EnhancedPhotographyObject::ReleaseResources() {
  if (ep_) {
    ep_->Release();
    ep_ = nullptr;
  }
  if (session_) {
    session_->Release();
    session_ = nullptr;
  }
}

}  // namespace enhanced_photography
}  // namespace realsense
