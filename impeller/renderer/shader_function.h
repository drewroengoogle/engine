// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <string>
#include "flutter/fml/hash_combine.h"
#include "flutter/fml/macros.h"
#include "impeller/base/comparable.h"
#include "impeller/core/shader_types.h"

namespace impeller {

class ShaderFunction : public Comparable<ShaderFunction> {
 public:
  // |Comparable<ShaderFunction>|
  virtual ~ShaderFunction();

  ShaderStage GetStage() const;

  const std::string& GetName() const;

  // |Comparable<ShaderFunction>|
  std::size_t GetHash() const override;

  // |Comparable<ShaderFunction>|
  bool IsEqual(const ShaderFunction& other) const override;

 protected:
  ShaderFunction(UniqueID parent_library_id,
                 std::string name,
                 ShaderStage stage);

 private:
  UniqueID parent_library_id_;
  std::string name_;
  ShaderStage stage_;

  ShaderFunction(const ShaderFunction&) = delete;

  ShaderFunction& operator=(const ShaderFunction&) = delete;
};

}  // namespace impeller
