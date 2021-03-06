/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// This file defines the attributes used in the TensorFlow dialect.

#ifndef TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_ATTRIBUTES_H_
#define TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_ATTRIBUTES_H_

#include "llvm/ADT/StringRef.h"
#include "mlir/IR/BuiltinAttributes.h"  // from @llvm-project
#include "mlir/IR/BuiltinTypes.h"  // from @llvm-project
#include "mlir/IR/MLIRContext.h"  // from @llvm-project
#include "mlir/IR/OpDefinition.h"  // from @llvm-project

namespace mlir {
namespace TF {

namespace detail {

struct ShapeAttrStorage;

}  // namespace detail

class ShapeAttr : public Attribute::AttrBase<ShapeAttr, Attribute,
                                             detail::ShapeAttrStorage> {
 public:
  using Base::Base;

  // Get or create a shape attribute. If shape is llvm::None, then it is
  // unranked. Otherwise it is ranked. And for ranked shapes, the value of the
  // dimension size must be >= -1. The value of -1 means the dimension is
  // dynamic. Otherwise, the dimension is static.
  static ShapeAttr get(MLIRContext* context,
                       llvm::Optional<ArrayRef<int64_t>> shape);

  // Get or create a shape attribute from a ShapedType type.
  static ShapeAttr get(MLIRContext* context, ShapedType shaped_type);

  llvm::Optional<ArrayRef<int64_t>> getValue() const;

  bool hasRank() const;

  // If this is ranked, return the rank. Otherwise, abort.
  int64_t getRank() const;

  // If this is ranked, return the shape. Otherwise, abort.
  ArrayRef<int64_t> getShape() const;

  // If this is unranked type or any dimension has unknown size (<0), it doesn't
  // have static shape. If all dimensions have known size (>= 0), it has static
  // shape.
  bool hasStaticShape() const;
};

// Parse one of the ODS defined TensorFlow attribute.
// TODO(aminim): migrate the other attributes.
OptionalParseResult ParseTensorFlowAttribute(MLIRContext* context,
                                             DialectAsmParser& parser,
                                             llvm::StringRef mnemonic,
                                             Type type, Attribute& value);

// Print one of the ODS defined TensorFlow attribute.
// TODO(aminim): migrate the other attributes.
void printTensorFlowAttribute(Attribute attr, DialectAsmPrinter& os);

}  // namespace TF
}  // namespace mlir

#define GET_ATTRDEF_CLASSES
#include "tensorflow/compiler/mlir/tensorflow/ir/tf_attributes.h.inc"

#endif  // TENSORFLOW_COMPILER_MLIR_TENSORFLOW_IR_TF_ATTRIBUTES_H_
