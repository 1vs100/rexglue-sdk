// Geometry shaders that expand Xenos point, rectangle and quad lists for the
// DXBC translator, emitted directly as shader model 5.1 bytecode.
#pragma once

#include <cstdint>
#include <functional>
#include <vector>

#include <rex/assert.h>
#include <rex/graphics/pipeline/shader/dxbc_translator.h>

namespace rex::graphics {

enum class DxbcGeometryShaderType : uint32_t {
  kNone,
  kPointList,
  kRectangleList,
  kQuadList,
};

union DxbcGeometryShaderKey {
  uint32_t key;
  struct {
    DxbcGeometryShaderType type : 2;
    uint32_t interpolator_count : 5;
    uint32_t user_clip_plane_count : 3;
    uint32_t user_clip_plane_cull : 1;
    uint32_t has_vertex_kill_and : 1;
    uint32_t has_point_size : 1;
    uint32_t has_point_coordinates : 1;
    // PA_CL_CLIP_CNTL::ps_ucp_mode for point primitives.
    uint32_t point_ps_ucp_mode : 2;
  };

  DxbcGeometryShaderKey() : key(0) { static_assert_size(*this, sizeof(key)); }

  struct Hasher {
    size_t operator()(const DxbcGeometryShaderKey& key) const {
      return std::hash<uint32_t>{}(key.key);
    }
  };
  bool operator==(const DxbcGeometryShaderKey& other_key) const { return key == other_key.key; }
  bool operator!=(const DxbcGeometryShaderKey& other_key) const { return !(*this == other_key); }
};

// False when the primitive type needs no geometry shader.
bool GetDxbcGeometryShaderKey(DxbcGeometryShaderType geometry_shader_type,
                              DxbcShaderTranslator::Modification vertex_shader_modification,
                              DxbcShaderTranslator::Modification pixel_shader_modification,
                              DxbcGeometryShaderKey& key_out);
void CreateDxbcGeometryShader(DxbcGeometryShaderKey key, std::vector<uint32_t>& shader_out);

}  // namespace rex::graphics
