#include "color_zeta_overlap_tests.h"

#include <pbkit/pbkit.h>

#include <utility>

#include "debug_output.h"
#include "pbkit_ext.h"
#include "test_host.h"

#define SET_MASK(mask, val) (((val) << (__builtin_ffs(mask) - 1)) & (mask))

// From pbkit.c, DMA_COLOR is set to channel 9 by default.
// NV097_SET_CONTEXT_DMA_COLOR == NV20_TCL_PRIMITIVE_3D_SET_OBJECT3
const uint32_t kDefaultDMAColorChannel = 9;
// NV097_SET_CONTEXT_DMA_ZETA is set to channel 10 by default.
const uint32_t kDefaultDMAZetaChannel = 10;

static constexpr char kTestName[] = "ZetaEqColor";

ColorZetaOverlapTests::ColorZetaOverlapTests(TestHost &host, std::string output_dir)
    : TestSuite(host, std::move(output_dir), "Color zeta overlap") {
  tests_[kTestName] = [this]() { Test(); };
}

void ColorZetaOverlapTests::Initialize() {
  TestSuite::Initialize();
  CreateGeometry();

  host_.SetXDKDefaultViewportAndFixedFunctionMatrices();
}

void ColorZetaOverlapTests::CreateGeometry() {
  constexpr int kNumTriangles = 4;
  auto buffer = host_.AllocateVertexBuffer(kNumTriangles * 3);

  int index = 0;
  {
    float one[] = {-1.5f, -1.5f, 0.0f};
    float two[] = {-2.5f, 0.6f, 0.0f};
    float three[] = {-0.5f, 0.6f, 0.0f};
    buffer->DefineTriangle(index++, one, two, three);
  }

  {
    float one[] = {0.0f, -1.5f, 5.0f};
    float two[] = {-1.0f, 0.75f, 10.0f};
    float three[] = {2.0f, 0.75f, 20.0f};
    buffer->DefineTriangle(index++, one, two, three);
  }

  {
    float one[] = {5.0f, -2.0f, 30};
    float two[] = {3.0f, 2.0f, 40};
    float three[] = {12.0f, 2.0f, 70};
    buffer->DefineTriangle(index++, one, two, three);
  }

  {
    float one[] = {20.0f, -10.0f, 50};
    float two[] = {12.0f, 10.0f, 125};
    float three[] = {80.0f, 10.0f, 200};
    buffer->DefineTriangle(index++, one, two, three);
  }
}

void ColorZetaOverlapTests::Test() {
//  host_.SetTextureFormat(texture_format);
//  std::string test_name = MakeTestName(texture_format);
//
//  auto &texture_stage = host_.GetTextureStage(0);
//  texture_stage.SetTextureDimensions(host_.GetMaxTextureWidth(), host_.GetMaxTextureHeight());
//
//  SDL_Surface *gradient_surface;
//  int update_texture_result =
//      GenerateGradientSurface(&gradient_surface, (int)host_.GetMaxTextureWidth(), (int)host_.GetMaxTextureHeight());
//  ASSERT(!update_texture_result && "Failed to generate SDL surface");
//
//  update_texture_result = host_.SetTexture(gradient_surface);
//  SDL_FreeSurface(gradient_surface);
//  ASSERT(!update_texture_result && "Failed to set texture");
//
//  uint32_t *p;
//
//  host_.PrepareDraw(0xFE202020);
//
//  // Redirect the color output to the target texture.
//  p = pb_begin();
//  p = pb_push1(p, NV097_SET_SURFACE_PITCH,
//               SET_MASK(NV097_SET_SURFACE_PITCH_COLOR, kTexturePitch) |
//                   SET_MASK(NV097_SET_SURFACE_PITCH_ZETA, kFramebufferPitch));
//  p = pb_push1(p, NV097_SET_CONTEXT_DMA_COLOR, texture_target_ctx_.ChannelID);
//  p = pb_push1(p, NV097_SET_SURFACE_COLOR_OFFSET, 0);
//  // TODO: Investigate if this is actually necessary. Morrowind does this after changing offsets.
//  p = pb_push1(p, NV097_NO_OPERATION, 0);
//  p = pb_push1(p, NV097_WAIT_FOR_IDLE, 0);
//  pb_end(p);
//
//  auto shader = std::make_shared<PrecalculatedVertexShader>();
//  host_.SetVertexShaderProgram(shader);
//
//  host_.SetWindowClip(kTextureWidth - 1, kTextureHeight - 1);
//
//  host_.SetVertexBuffer(render_target_vertex_buffer_);
//  bool swizzle = true;
//  host_.SetSurfaceFormat(TestHost::SCF_A8R8G8B8, TestHost::SZF_Z24S8, kTextureWidth, kTextureHeight, swizzle);
//  if (!swizzle) {
//    // Linear targets should be cleared to avoid uninitialized memory in regions not explicitly drawn to.
//    host_.Clear(0xFF000000, 0, 0);
//  }
//
//  host_.DrawArrays();
//
//  texture_stage.SetTextureDimensions(kTextureWidth, kTextureHeight);
//  host_.SetVertexShaderProgram(nullptr);
//  host_.SetXDKDefaultViewportAndFixedFunctionMatrices();
//
//  host_.SetWindowClip(host_.GetFramebufferWidth() - 1, host_.GetFramebufferHeight() - 1);
//  host_.SetTextureFormat(GetTextureFormatInfo(NV097_SET_TEXTURE_FORMAT_COLOR_SZ_A8R8G8B8));
//
//  p = pb_begin();
//  p = pb_push1(p, NV097_SET_SURFACE_PITCH,
//               SET_MASK(NV097_SET_SURFACE_PITCH_COLOR, kFramebufferPitch) |
//                   SET_MASK(NV097_SET_SURFACE_PITCH_ZETA, kFramebufferPitch));
//  p = pb_push1(p, NV097_SET_CONTEXT_DMA_COLOR, kDefaultDMAColorChannel);
//  p = pb_push1(p, NV097_SET_SURFACE_COLOR_OFFSET, 0);
//  pb_end(p);
//
//  host_.SetRawTexture(render_target_, kTextureWidth, kTextureHeight, 1, kTexturePitch, 4, false);
//
//  host_.SetVertexBuffer(framebuffer_vertex_buffer_);
//  host_.PrepareDraw(0xFE202020);
//  host_.DrawArrays();
//
//  pb_print("N: %s\n", texture_format.name);
//  pb_print("F: 0x%x\n", texture_format.xbox_format);
//  pb_print("SZ: %d\n", texture_format.xbox_swizzled);
//  pb_print("C: %d\n", texture_format.require_conversion);
//  pb_print("W: %d\n", host_.GetMaxTextureWidth());
//  pb_print("H: %d\n", host_.GetMaxTextureHeight());
//  pb_print("P: %d\n", texture_format.xbox_bpp * host_.GetMaxTextureWidth() / 8);
//  pb_draw_text_screen();

  host_.FinishDraw(allow_saving_, output_dir_, kTestName);

  host_.SetWindowClip(host_.GetFramebufferWidth(), host_.GetFramebufferHeight());
  host_.SetViewportOffset(0.531250f, 0.531250f, 0, 0);
  host_.SetViewportScale(0, -0, 0, 0);
}
