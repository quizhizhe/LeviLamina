#pragma once

#include "mc/_HeaderOutputPredefine.h"

#include "mc/deps/minecraft_renderer/resources/ResourceServiceTextureDescription.h"
#include "mc/external/render_dragon/resources/BufferDescription.h"
#include "mc/external/render_dragon/resources/ImageDescription.h"
#include "mc/external/render_dragon/resources/MaterialDescription.h"
#include "mc/external/render_dragon/resources/TextureDescription.h"


#include "mc/deps/core/checked_resource_service/PerFrameHandleTracker.h"
#include "mc/deps/core/checked_resource_service/UncheckedHandleTracker.h"

namespace dragon {
class ResolvedTextureResource;
class ResolvedImageResource;
class ResolvedMaterialResource;
struct ResolvedVertexBufferResource;
struct ResolvedIndexBufferResource;
struct ResolvedShaderBufferResource;
struct ResolvedAccelerationStructureResource;
} // namespace dragon


namespace mce {

class Texture;
class ImmediateBuffer;

template <typename T>
class ClientResourcePointer;

template <typename T>
class ServerResourcePointer;

class Buffer;

template <typename T>
struct ResourceHandleTrackerValidator; // 主模板不定义，强制每个 T 必须显式特化

template <typename T>
struct ResourceBlockTemplate {
    std::shared_ptr<typename ResourceHandleTrackerValidator<T>::DescriptionType> mDebugInfoBlock;
    typename ResourceHandleTrackerValidator<T>::TrackerType                      mTrackingBlock;
    std::unique_ptr<T>                                                           mResource;

    ResourceBlockTemplate() : mDebugInfoBlock(), mTrackingBlock(), mResource(std::make_unique<T>()) {}
    ~ResourceBlockTemplate() = default;

    void reset();
    T*   get() const;
};

// ---- Texture ----
template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedTextureResource> {
    using DescriptionType = dragon::TextureDescription;
    using TrackerType     = PerFrameHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

// ---- Image ----
template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedImageResource> {
    using DescriptionType = dragon::ImageDescription;
    using TrackerType     = PerFrameHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

// ---- ImmediateBuffer ----
template <>
struct ResourceHandleTrackerValidator<mce::ImmediateBuffer> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = PerFrameHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

// ---- Client 端 Texture handle（variant 包装版）----
template <>
struct ResourceHandleTrackerValidator<
    std::variant<std::monostate, mce::Texture, mce::ClientResourcePointer<dragon::ResolvedTextureResource>>> {
    using DescriptionType = mce::ResourceServiceTextureDescription;
    using TrackerType     = PerFrameHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

// ---- Client 端 Buffer handle（variant 包装 Vertex/Index）----
template <>
struct ResourceHandleTrackerValidator<std::variant<
    std::monostate,
    mce::Buffer,
    mce::ClientResourcePointer<dragon::ResolvedVertexBufferResource>,
    mce::ClientResourcePointer<dragon::ResolvedIndexBufferResource>>> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = PerFrameHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

// ---- 以下五个：内部直连资源，一律 UncheckedHandleTracker ----

template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedVertexBufferResource> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = UncheckedHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedIndexBufferResource> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = UncheckedHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedShaderBufferResource> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = UncheckedHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedAccelerationStructureResource> {
    using DescriptionType = dragon::BufferDescription;
    using TrackerType     = UncheckedHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

template <>
struct ResourceHandleTrackerValidator<dragon::ResolvedMaterialResource> {
    using DescriptionType = dragon::MaterialDescription;
    using TrackerType     = UncheckedHandleTracker;

    static DescriptionType const& extractDebugBlock(std::shared_ptr<DescriptionType> const& debugInfoBlock) {
        static DescriptionType defaultDebugBlock;
        return debugInfoBlock ? *debugInfoBlock : defaultDebugBlock;
    }
};

} // namespace mce
