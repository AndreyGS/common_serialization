/**
 * @file cslib/include/common_serialization/Allocators/AllocatorHelpers/IAllocatorHelper.h
 * @author Andrey Grabov-Smetankin <ukbpyh@gmail.com>
 *
 * @section LICENSE
 *
 * Copyright 2023 Andrey Grabov-Smetankin <ukbpyh@gmail.com>
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

namespace common_serialization
{

/// @brief Interface of allocator helpers (CRTP)
/// @details This is interface for convenient usage of IAllocators.
///     In many (or most) scenarios there is no need of direct using of IAllocators.
///     Instead use this one supplying it with suitable IAllocator.
/// @tparam T Type of objects that allocator would allocate and construct
/// @tparam Allocator Class that implement IAllocator interface
/// @tparam AllocatorHelper Most derived class (instance type)
template<typename T, IAllocator Allocator, typename AllocatorHelper>
class IAllocatorHelper
{
public:
    using instance_type = AllocatorHelper;
    using interface_type = IAllocatorHelper<T, Allocator, AllocatorHelper>;

    using value_type = typename Allocator::value_type;
    using pointer = typename Allocator::pointer;
    using size_type = typename Allocator::size_type;
    using difference_type = typename Allocator::difference_type;
    using constructor_allocator = typename Allocator::constructor_allocator;
    using allocator = Allocator;

    /// @brief Shortcut for allocating and subsequent constructing operations
    /// @tparam ...Args Parameters types that go to constructor of every element
    /// @param requestedN Requested number of elements, that need to be allocated and constructed
    /// @param pAllocatedN The actual number of elements that are allocated and constructed
    /// @param ...args Parameters that go to constructor of every element
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    ///     or if object construction process return error.
    template<typename... Args>
    [[nodiscard]] constexpr T* allocateAndConstruct(size_type requestedN, size_type* pAllocatedN, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateAndConstructImpl(requestedN, pAllocatedN, std::forward<Args>(args)...);
    }

    /// @brief Allocate storage for n elements of type T with support to apply 
    ///     internal management (strategy) of allocating storage by actual implementation
    ///     of IAllocatorHelper
    /// @param requestedN Number of elements that storage should be capable to hold
    /// @param pAllocatedN Returned the actual number of elements that allocated storage can hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    [[nodiscard]] constexpr T* allocate(size_type requestedN, size_type* pAllocatedN) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateImpl(requestedN, pAllocatedN);
    }

    /// @brief Allocate storage for n elements of type T
    /// @param n Number of elements that storage should be capable to hold
    /// @return Pointer to allocated storage, nullptr if there is not enough memory
    [[nodiscard]] constexpr T* allocateStrict(size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->allocateStrictImpl(n);
    }

    /// @brief Call constructor with args on memory pointed by p
    /// @tparam ...Args Parameters types that go to constructors
    /// @param p Pointer to memory where object must be created
    /// @param ...args Parameters that are go to constructors
    /// @return Status of operation
    template<typename... Args>
    constexpr Status construct(T* p, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructImpl(p, std::forward<Args>(args)...);
    }

    /// @brief Call constructor for n elements with args on memory pointed by p
    /// @tparam ...Args Parameters types that go to constructors
    /// @param p Pointer to memory where object must be created
    /// @param pNError Pointer on pointer to memory where an error was occurred.
    ///     If there is no error, returned value of pNError is undefined.
    /// @param n Number of elements to construct
    /// @param ...args Parameters that go to constructors
    /// @return Status of operation
    template<typename... Args>
    constexpr Status constructN(T* p, T** pNError, size_type n, Args&&... args) const
    {
        return static_cast<const AllocatorHelper*>(this)->constructNImpl(p, pNError, n, std::forward<Args>(args)...);
    }

    /// @brief Copy elements using copy constructors
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    constexpr Status copy(T* pDest, const T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyDirtyImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Copy elements using copy constructors when
    ///     there is guaranteed no overlapping in memory regions
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    constexpr Status copyNoOverlap(T* pDest, const T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyDirtyNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Copy elements using copy constructors when
    ///     some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    constexpr Status copyDirty(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyDirtyImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Copy elements using copy constructors when
    ///     there is guaranteed no overlapping in memory regions,
    ///     but some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to copy
    /// @return Status of operation
    constexpr Status copyDirtyNoOverlap(T* pDest, T* pDirtyMemoryFinish, const T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->copyDirtyNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Move elements using move constructors
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    constexpr Status move(T* pDest, T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Move elements using move constructors when
    ///     there is guaranteed no overlapping in memory regions
    /// @param pDest Pointer to destination array of elements
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    constexpr Status moveNoOverlap(T* pDest, T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveNoOverlapImpl(pDest, pDest, pSrc, n);
    }

    /// @brief Move elements using move constructors when
    ///     some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    constexpr Status moveDirty(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Move elements using move constructors when
    ///     there is guaranteed no overlapping in memory regions,
    ///     but some part of destination memory already has initialized objects
    /// @param pDest Pointer to destination array of elements
    /// @param pDirtyMemoryFinish Pointer to one of the elements of the destination array
    ///     from which memory is not initialized
    /// @param pSrc Pointer to source array of elements
    /// @param n Number of elements to move
    /// @return Status of operation
    constexpr Status moveDirtyNoOverlap(T* pDest, T* pDirtyMemoryFinish, T* pSrc, size_type n) const
    {
        return static_cast<const AllocatorHelper*>(this)->moveNoOverlapImpl(pDest, pDirtyMemoryFinish, pSrc, n);
    }

    /// @brief Shortcut for destroying and subsequent deallocating operations 
    /// @param p Pointer to storage of elements which must be deallocated
    /// @param n Number of elements to destroy
    constexpr void destroyAndDeallocate(T* p, size_type n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyAndDeallocateImpl(p, n);
    }

    /// @brief Deallocate storage
    /// @param p Pointer to storage
    constexpr void deallocate(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->deallocateImpl(p);
    }

    /// @brief Destroy object
    /// @param p Pointer to object
    constexpr void destroy(T* p) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyImpl(p);
    }

    /// @brief Destroy n objects
    /// @param p Pointer to objects
    /// @param n Number of objects
    constexpr void destroyN(T* p, size_t n) const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->destroyNImpl(p, n);
    }

    /// @brief Get number of elements that can be allocated
    /// @return Number of elements that can be allocated
    constexpr size_t max_size() const noexcept
    {
        return static_cast<const AllocatorHelper*>(this)->max_size_impl();
    }

    /// @brief Get managed Allocator
    /// @return Managed Allocator
    constexpr Allocator& getAllocator() noexcept
    {
        return m_allocator;
    }

    constexpr const Allocator& getAllocator() const noexcept
    {
        return m_allocator;
    }

protected:
    constexpr IAllocatorHelper() { }

    constexpr IAllocatorHelper(const IAllocatorHelper& rhs)
    {
        operator=(rhs);
    }

    constexpr IAllocatorHelper(IAllocatorHelper&& rhs) noexcept
    {
        operator=(std::move(rhs));
    }

    constexpr IAllocatorHelper& operator=(const IAllocatorHelper& rhs)
    {
        m_allocator = rhs.m_allocator;
        return *this;
    }

    constexpr IAllocatorHelper& operator=(IAllocatorHelper&& rhs) noexcept
    {
        m_allocator = std::move(rhs.m_allocator);
        return *this;
    }

private:
    Allocator m_allocator;
};

} // namespace common_serialization
