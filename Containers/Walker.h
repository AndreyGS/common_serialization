#include "Vector.h"

template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawHeapAllocator>>>
class Walker
{
public:
    using size_typeype = typename Vector<T, AllocatorHelper>::size_type;

    using iterator = VectorIterator<Vector<T, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<T, AllocatorHelper>>;

    constexpr Walker() noexcept;
    constexpr Walker(const Walker& rhs);
    constexpr Walker(Walker&& rhs) noexcept;
    constexpr Walker& operator=(const Walker& rhs);
    constexpr Walker& operator=(Walker&& rhs) noexcept;
    constexpr ~Walker() noexcept;
    
    constexpr bool reserve(size_typeype n);

    constexpr size_type push_back(const T& value);
    constexpr size_type push_back(T&& value);
    constexpr size_type push_back_n(const T* p, size_type n);

    constexpr size_type replace(size_type offset, const T* p, size_type n);

    constexpr size_type insert(size_type offset, const T* p, size_type n);
    template<typename ItSrc>
    constexpr size_type insert(size_type offset, ItSrc srcBegin, ItSrc srcEnd);

    constexpr size_type erase(size_type offset, size_type n);
    
    [[nodiscard]] constexpr T* data() noexcept;
    [[nodiscard]] constexpr const T* data() const noexcept;
    [[nodiscard]] constexpr T& operator[](size_type offset);
    [[nodiscard]] constexpr const T& operator[](size_type offset) const;

    [[nodiscard]] constexpr size_type size() const noexcept;
    [[nodiscard]] constexpr size_type max_size() const noexcept;
    [[nodiscard]] constexpr size_type capacity() const noexcept;

    constexpr void clear() noexcept;
    constexpr void invalidate() noexcept;
    [[nodiscard]] constexpr T* release() noexcept;

    constexpr AllocatorHelper& getAllocatorHelper() noexcept;
    constexpr AllocatorHelper& getAllocatorHelper() const noexcept;

    constexpr Vector<T, AllocatorHelper>& getVector() noexcept;
    constexpr const Vector<T, AllocatorHelper>& getVector() const noexcept;

    constexpr size_type tell() const noexcept;
    constexpr void seek(size_type offset) const noexcept;

    constexpr size_type write(const T* p, size_type n);
    template<typename ItSrc>
    constexpr size_type write(ItSrc srcBegin, ItSrc srcEnd);

    constexpr inline size_type read(T* p, size_type n);

private:
    Vector<T, AllocatorHelper> m_vector;
    size_typeype m_offset = 0;
};

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker() noexcept { };

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker(const Walker& rhs)
    : m_vector(rhs.m_vector), m_offset(rhs.m_offset)
{ }

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::Walker(Walker&& rhs) noexcept
    : m_vector(std::move(rhs.m_vector)), m_offset(std::move(rhs.m_offset))
{ }

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>& Walker<T, AllocatorHelper>::operator=(const Walker& rhs)
{
    if (this != &rhs)
    {
        m_vector = rhs.m_vector;
        m_offset = rhs.m_offset;
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>& Walker<T, AllocatorHelper>::operator=(Walker&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_vector = std::move(rhs.m_vector);
        m_offset = std::move(rhs.m_offset);
    }

    return *this;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::~Walker() noexcept { }

template<typename T, typename AllocatorHelper>
constexpr bool Walker<T, AllocatorHelper>::reserve(size_typeype n)
{
    return m_vector.reserve(n);
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back(const T& value)
{
    m_vector.push_back(value);
    return m_offset = m_vector.size();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back(T&& value)
{
    m_vector.push_back(std::move(value));
    return m_offset = m_vector.size();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::push_back_n(const T* p, size_type n)
{
    m_vector.push_back_n(p, n);
    return m_offset = m_vector.size();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::replace(size_type offset, const T* p, size_type n)
{
    return m_offset = m_vector.replace(offset, p, n);
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::insert(size_type offset, const T* p, size_type n)
{
    return m_offset = m_vector.insert(offset, p, n);
}

template<typename T, typename AllocatorHelper>
template<typename ItSrc>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::insert(size_type offset, ItSrc srcBegin, ItSrc srcEnd)
{
    return m_offset = m_vector.insert(m_vector.begin() + offset, srcBegin, srcEnd).getPointer() - m_vector.data();
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::erase(size_type offset, size_type n)
{
    return m_offset = m_vector.erase(offset, n);
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Walker<T, AllocatorHelper>::data() noexcept
{
    return m_vector.data();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T* Walker<T, AllocatorHelper>::data() const noexcept
{
    return m_vector.data();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T& Walker<T, AllocatorHelper>::operator[](size_type offset)
{
    return m_vector[offset];
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr const T& Walker<T, AllocatorHelper>::operator[](size_type offset) const
{
    return m_vector[offset];
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::size() const noexcept
{
    return m_vector.size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::max_size() const noexcept
{
    return m_vector.max_size();
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::capacity() const noexcept
{
    return m_vector.capacity();
}

template<typename T, typename AllocatorHelper>
constexpr void Walker<T, AllocatorHelper>::clear() noexcept
{
    m_vector.clear(), m_offset = 0;
}

template<typename T, typename AllocatorHelper>
constexpr void Walker<T, AllocatorHelper>::invalidate() noexcept
{
    m_vector.invalidate(), m_offset = 0;
}

template<typename T, typename AllocatorHelper>
[[nodiscard]] constexpr T* Walker<T, AllocatorHelper>::release() noexcept
{
    m_vector.release(), m_offset = 0;
}

template<typename T, typename AllocatorHelper>
constexpr AllocatorHelper& Walker<T, AllocatorHelper>::getAllocatorHelper() noexcept
{
    return m_vector.getAllocatorHelper();
}

template<typename T, typename AllocatorHelper>
constexpr AllocatorHelper& Walker<T, AllocatorHelper>::getAllocatorHelper() const noexcept
{
    return m_vector.getAllocatorHelper();
}

template<typename T, typename AllocatorHelper>
constexpr Vector<T, AllocatorHelper>& Walker<T, AllocatorHelper>::getVector() noexcept
{
    return m_vector;
}

template<typename T, typename AllocatorHelper>
constexpr const Vector<T, AllocatorHelper>& Walker<T, AllocatorHelper>::getVector() const noexcept
{
    return m_vector;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::tell() const noexcept
{
    return m_offset;
}

constexpr void Walker<T, AllocatorHelper>::seek(size_type offset) const noexcept
{
    m_offset = offset;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::write(const T* p, size_type n)
{
    size_type oldOffset = m_offset;
    m_offset = replace(m_offset, p, n);
    return m_offset - oldOffset;
}

template<typename T, typename AllocatorHelper>
template<typename ItSrc>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::write(ItSrc srcBegin, ItSrc srcEnd)
{
    size_type oldSize = m_vector.size();
    insert(m_offset, srcBegin, srcEnd);
    size_type n = m_vector.size() - oldSize;
    m_offset += n
        return n;
}

template<typename T, typename AllocatorHelper>
constexpr Walker<T, AllocatorHelper>::size_type Walker<T, AllocatorHelper>::read(T* p, size_type n)
{
    size_type nReal = m_vector.copy_n(m_offset, n, p) - p;
    m_offset += nReal;
    return nReal;
}
