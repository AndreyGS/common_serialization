/**
 * @file common_serialization/allocation_manager_interfaces/IAllocationManager.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023-2024 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include <common_serialization/allocator_interfaces/allocator_interface.h>

namespace common_serialization
{

/// @brief Interface of allocator helpers (CRTP)
/// @details This is interface for convenient usage of IAllocators.
///     In many (or most) scenarios there is no need of direct using of IAllocators.
///     Instead use this one supplying it with suitable IAllocator.
/// @tparam _Allocator Allocator
/// @tparam _AllocationManager Most derived class (instance type)
template<IAllocatorImpl _Allocator, typename _AllocationManager>
class IAllocationManager
{
public:
    using allocator_type = _Allocator;
    using value_type = typename allocator_type::value_type;
    using pointer = typename allocator_type::pointer;
    using const_pointer = const value_type*;
    using size_type = typename allocator_type::size_type;
    using difference_type = typename allocator_type::difference_type;
    using constructor_allocator = typename allocator_type::constructor_allocator;

    /// @brief Shortcut for allocating and subsequent constructing operations
    /// @tparam ...Args Parameters types that go to constructor of every element
    /// @param requestedN Requested number of elements, that need to be allocated and constructed
    /// @param pAllocatedN The actual number of elements that are allocated and constructed
    /// @param ...args Parameters that go to constructor of every element
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    ///     or if object construction process return error.
    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateAndConstruct(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
    {
        return static_cast<const _AllocationManager*>(this)->allocateAndConstructImpl(requestedN, pAllocatedN, std::forward<Args>(args)...);
    }

    /// @brief Allocate storage for n elements of type _T with support to apply 
    ///     internal management (strategy) of allocating storage by actual implementation
    ///     of IAllocationManager
    /// @param requestedN Number of elements that storage should be capable to hold
    /// @param pAllocatedN Returned the actual number of elements that allocated storage can hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocate(size_type requestedN, size_type* pAllocatedN) const
    {
        return static_cast<const _AllocationManager*>(this)->allocateImpl(requestedN, pAllocatedN);
    }

    /// @brief Allocate storage for n elements of type _T
    /// @param n Number of elements that storage should be capable to hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    AGS_CS_ALWAYS_INLINE constexpr [[nodiscard]] pointer allocateStrict(size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->allocateStrictImpl(n);
    }

    /// @brief Call ctor with args on memory pointed by p
    /// @tparam ...Args Parameters types that go to ctors
    /// @param p Pointer to memory where object must be created
    /// @param ...args Parameters that are go to ctors
    /// @return Status of operation
    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr Status construct(pointer p, Args&&... args) const
    {
        return static_cast<const _AllocationManager*>(this)->constructImpl(p, std::forward<Args>(args)...);
    }

    /// @brief Call ctor for n elements with args on memory pointed by p
    /// @tparam ...Args Parameters types that go to ctors
    /// @param p Pointer to memory where object must be created
    /// @param pNError Pointer on pointer to memory where an error was occurred.
    ///     If there is no error, returned value of pNError is undefined.
    /// @param n Number of elements to construct
    /// @param ...args Parameters that go to ctors
    /// @return Status of operation
    template<typename... Args>
    AGS_CS_ALWAYS_INLINE constexpr Status constructN(pointer p, pointer* pNError, size_type n, Args&&... args) const
    {
        return static_cast<const _AllocationManager*>(this)->constructNImpl(p, pNError, n, std::forward<Args>(args)...);
    }

    /// @brief Copy elements using copy ctors
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status copy(pointer pDest, const_pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->copyDirtyImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Copy elements using copy ctors when
    ///     there is guaranteed no overlapping in memory regions
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status copyNoOverlap(pointer pDest, const_pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->copyDirtyNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Copy elements using copy ctors when
    ///     some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status copyDirty(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->copyDirtyImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Copy elements using copy ctors when
    ///     there is guaranteed no overlapping in memory regions,
    ///     but some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status copyDirtyNoOverlap(pointer pDest, pointer pDirtyMemoryFinish, const_pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->copyDirtyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Move elements using move ctors
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status move(pointer pDest, pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->moveImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Move elements using move ctors when
    ///     there is guaranteed no overlapping in memory regions
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status moveNoOverlap(pointer pDest, pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->moveNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Move elements using move ctors when
    ///     some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status moveDirty(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->moveImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Move elements using move ctors when
    ///     there is guaranteed no overlapping in memory regions,
    ///     but some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    AGS_CS_ALWAYS_INLINE constexpr Status moveDirtyNoOverlap(pointer pDest, pointer pDirtyMemoryFinish, pointer pSrc, size_type n) const
    {
        return static_cast<const _AllocationManager*>(this)->moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Shortcut for destroying and subsequent deallocating operations 
    /// @param p Pointer to storage of elements which must be deallocated
    /// @param n Number of elements to destroy
    AGS_CS_ALWAYS_INLINE constexpr void destroyAndDeallocate(pointer p, size_type n) const noexcept
    {
        return static_cast<const _AllocationManager*>(this)->destroyAndDeallocateImpl(p, n);
    }

    /// @brief Deallocate storage
    /// @param p Pointer to storage
    AGS_CS_ALWAYS_INLINE constexpr void deallocate(pointer p) const noexcept
    {
        return static_cast<const _AllocationManager*>(this)->deallocateImpl(p);
    }

    /// @brief Destroy object
    /// @param p Pointer to object
    AGS_CS_ALWAYS_INLINE constexpr void destroy(pointer p) const noexcept
    {
        return static_cast<const _AllocationManager*>(this)->destroyImpl(p);
    }

    /// @brief Destroy n objects
    /// @param p Pointer to objects
    /// @param n Number of objects
    AGS_CS_ALWAYS_INLINE constexpr void destroyN(pointer p, size_t n) const noexcept
    {
        return static_cast<const _AllocationManager*>(this)->destroyNImpl(p, n);
    }

    /// @brief Get number of elements that can be allocated
    /// @return Number of elements that can be allocated
    AGS_CS_ALWAYS_INLINE constexpr size_t max_size() const noexcept
    {
        return static_cast<const _AllocationManager*>(this)->max_size_impl();
    }

    /// @brief Get managed allocator
    /// @return Managed allocator
    AGS_CS_ALWAYS_INLINE constexpr allocator_type& getAllocator() noexcept
    {
        return m_allocator;
    }

    AGS_CS_ALWAYS_INLINE constexpr const allocator_type& getAllocator() const noexcept
    {
        return m_allocator;
    }

private:
    allocator_type m_allocator;
};

template<typename _T>
concept IAllocationManagerImpl = std::is_base_of_v<IAllocationManager<typename _T::allocator_type, normalize_t<_T>>, normalize_t<_T>>;

} // namespace common_serialization
