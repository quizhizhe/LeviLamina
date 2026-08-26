#pragma once

#include "mc/_HeaderOutputPredefine.h"
#include "mc/external/render_dragon/resources/ResourceBlockTemplate.h"

namespace mce {

template <typename T>
class ResourcePointer {
public:
    std::shared_ptr<ResourceBlockTemplate<T>> mResourcePointerBlock; // +0x08

    ResourcePointer() = default;
    explicit ResourcePointer(std::shared_ptr<ResourceBlockTemplate<T>> const& block)
    : mResourcePointerBlock(block) {} // [inferred]

    virtual ~ResourcePointer() = default;

    bool isValid(ValidityCheckType validityCheckType) const {
        if (!mResourcePointerBlock) {
            return false;
        }
        return std::get<0>(mResourcePointerBlock->mTrackingBlock.Validate(validityCheckType));
    }

    T*       get() const { return mResourcePointerBlock ? mResourcePointerBlock->get() : nullptr; }
    T*       operator->() const { return get(); }
    T&       operator*() const { return *get(); }
    explicit operator bool() const { return mResourcePointerBlock != nullptr; }
    void     reset() { mResourcePointerBlock.reset(); }

    // 拷贝/移动走 shared_ptr 语义 [inferred]
    ResourcePointer(ResourcePointer const&)            = default;
    ResourcePointer(ResourcePointer&&)                 = default;
    ResourcePointer& operator=(ResourcePointer const&) = default;
    ResourcePointer& operator=(ResourcePointer&&)      = default;
};

} // namespace mce
