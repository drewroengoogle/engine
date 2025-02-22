// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

#include "impeller/aiks/image.h"
#include "impeller/aiks/image_filter.h"
#include "impeller/aiks/paint.h"
#include "impeller/aiks/picture.h"
#include "impeller/core/sampler_descriptor.h"
#include "impeller/entity/entity.h"
#include "impeller/entity/entity_pass.h"
#include "impeller/entity/geometry/geometry.h"
#include "impeller/entity/geometry/vertices_geometry.h"
#include "impeller/geometry/matrix.h"
#include "impeller/geometry/path.h"
#include "impeller/geometry/point.h"
#include "impeller/geometry/vector.h"
#include "impeller/typographer/text_frame.h"

namespace impeller {

struct CanvasStackEntry {
  Matrix transform;
  // |cull_rect| is conservative screen-space bounds of the clipped output area
  std::optional<Rect> cull_rect;
  size_t clip_depth = 0u;
  Entity::RenderingMode rendering_mode = Entity::RenderingMode::kDirect;
  bool contains_clips = false;
};

enum class PointStyle {
  /// @brief Points are drawn as squares.
  kRound,

  /// @brief Points are drawn as circles.
  kSquare,
};

class Canvas {
 public:
  struct DebugOptions {
    /// When enabled, layers that are rendered to an offscreen texture
    /// internally get a translucent checkerboard pattern painted over them.
    ///
    /// Requires the `IMPELLER_DEBUG` preprocessor flag.
    bool offscreen_texture_checkerboard = false;
  } debug_options;

  Canvas();

  explicit Canvas(Rect cull_rect);

  explicit Canvas(IRect cull_rect);

  ~Canvas();

  void Save();

  void SaveLayer(const Paint& paint,
                 std::optional<Rect> bounds = std::nullopt,
                 const std::shared_ptr<ImageFilter>& backdrop_filter = nullptr);

  bool Restore();

  size_t GetSaveCount() const;

  void RestoreToCount(size_t count);

  const Matrix& GetCurrentTransform() const;

  const std::optional<Rect> GetCurrentLocalCullingBounds() const;

  void ResetTransform();

  void Transform(const Matrix& transform);

  void Concat(const Matrix& transform);

  void PreConcat(const Matrix& transform);

  void Translate(const Vector3& offset);

  void Scale(const Vector2& scale);

  void Scale(const Vector3& scale);

  void Skew(Scalar sx, Scalar sy);

  void Rotate(Radians radians);

  void DrawPath(Path path, const Paint& paint);

  void DrawPaint(const Paint& paint);

  void DrawLine(const Point& p0, const Point& p1, const Paint& paint);

  void DrawRect(Rect rect, const Paint& paint);

  void DrawRRect(Rect rect, Point corner_radii, const Paint& paint);

  void DrawCircle(Point center, Scalar radius, const Paint& paint);

  void DrawPoints(std::vector<Point>,
                  Scalar radius,
                  const Paint& paint,
                  PointStyle point_style);

  void DrawImage(const std::shared_ptr<Image>& image,
                 Point offset,
                 const Paint& paint,
                 SamplerDescriptor sampler = {});

  void DrawImageRect(const std::shared_ptr<Image>& image,
                     Rect source,
                     Rect dest,
                     const Paint& paint,
                     SamplerDescriptor sampler = {});

  void ClipPath(
      Path path,
      Entity::ClipOperation clip_op = Entity::ClipOperation::kIntersect);

  void ClipRect(
      const Rect& rect,
      Entity::ClipOperation clip_op = Entity::ClipOperation::kIntersect);

  void ClipRRect(
      const Rect& rect,
      Point corner_radii,
      Entity::ClipOperation clip_op = Entity::ClipOperation::kIntersect);

  void DrawPicture(const Picture& picture);

  void DrawTextFrame(const std::shared_ptr<TextFrame>& text_frame,
                     Point position,
                     const Paint& paint);

  void DrawVertices(const std::shared_ptr<VerticesGeometry>& vertices,
                    BlendMode blend_mode,
                    const Paint& paint);

  void DrawAtlas(const std::shared_ptr<Image>& atlas,
                 std::vector<Matrix> transforms,
                 std::vector<Rect> texture_coordinates,
                 std::vector<Color> colors,
                 BlendMode blend_mode,
                 SamplerDescriptor sampler,
                 std::optional<Rect> cull_rect,
                 const Paint& paint);

  Picture EndRecordingAsPicture();

 private:
  std::unique_ptr<EntityPass> base_pass_;
  EntityPass* current_pass_ = nullptr;
  std::deque<CanvasStackEntry> transform_stack_;
  std::optional<Rect> initial_cull_rect_;

  void Initialize(std::optional<Rect> cull_rect);

  void Reset();

  EntityPass& GetCurrentPass();

  size_t GetClipDepth() const;

  void ClipGeometry(const std::shared_ptr<Geometry>& geometry,
                    Entity::ClipOperation clip_op);

  void IntersectCulling(Rect clip_bounds);
  void SubtractCulling(Rect clip_bounds);

  void Save(bool create_subpass,
            BlendMode = BlendMode::kSourceOver,
            const std::shared_ptr<ImageFilter>& backdrop_filter = nullptr);

  void RestoreClip();

  bool AttemptDrawBlurredRRect(const Rect& rect,
                               Scalar corner_radius,
                               const Paint& paint);

  Canvas(const Canvas&) = delete;

  Canvas& operator=(const Canvas&) = delete;
};

}  // namespace impeller
