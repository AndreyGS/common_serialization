#include "Vector.h"

template<typename T, typename AllocatorHelper = StrategicAllocatorHelper<T, ConstructorNoexceptAllocator<T, RawHeapAllocator>>>
class Walker
{
public:
    using size_typeype = typename Vector<T, AllocatorHelper>::size_type;

    using iterator = VectorIterator<Vector<T, AllocatorHelper>>;
    using const_iterator = ConstVectorIterator<Vector<T, AllocatorHelper>>;

    constexpr inline Walker() noexcept { };
    
    constexpr inline Walker(const Walker& rhs)
        : m_vector(rhs.m_vector), m_offset(rhs.m_offset)
    { }

    constexpr inline Walker(Walker&& rhs) noexcept
        : m_vector(std::move(rhs.m_vector)), m_offset(std::move(rhs.m_offset))
    { }

    constexpr Walker& operator=(const Walker& rhs)
    {
        if (this != &rhs)
        {
            m_vector = rhs.m_vector;
            m_offset = rhs.m_offset;
        }

        return *this;
    }

    constexpr Walker& operator=(Walker&& rhs) noexcept
    {
        if (this != &rhs)
        {
            m_vector = std::move(rhs.m_vector);
            m_offset = std::move(rhs.m_offset);
        }

        return *this;
    }
    constexpr inline ~Walker() noexcept { }
    
    constexpr inline bool reserve(size_typeype n)
    {
        return m_vector.reserve(n);
    }

    constexpr inline size_type push_back(const T& value)
    {
        m_vector.push_back(value);
        return m_offset = m_vector.size();
    }

    constexpr inline size_type push_back(T&& value)
    {
        m_vector.push_back(std::move(value));
        return m_offset = m_vector.size();
    }

    constexpr inline size_type push_back_n(const T* p, size_type n)
    {
        m_vector.push_back_n(p, n);
        return m_offset = m_vector.size();
    }

    constexpr inline size_type replace(size_type offset, const T* p, size_type n)
    {
        return m_offset = m_vector.replace(offset, p, n);
    }

    constexpr inline size_type insert(size_type offset, const T* p, size_type n)
    {
        return m_offset = m_vector.insert(offset, p, n);
    }

    template<typename It_src>
    constexpr size_type insert(size_type offset, It_src srcBegin, It_src srcEnd)
    {
        return m_offset = m_vector.insert(m_vector.begin() + offset, srcBegin, srcEnd).getPointer() - m_vector.data();
    }

    constexpr size_type erase(size_type offset, size_type n)
    {
        return m_offset = m_vector.erase(offset, n);
    }
    
    [[nodiscard]] constexpr inline T* data() noexcept
    {
        return m_vector.data();
    }

    [[nodiscard]] constexpr inline const T* data() const noexcept
    {
        return m_vector.data();
    }

    [[nodiscard]] constexpr inline T& operator[](size_type offset)
    {
        return m_vector[offset];
    }

    [[nodiscard]] constexpr inline const T& operator[](size_type offset) const
    {
        return m_vector[offset];
    }

    [[nodiscard]] constexpr inline size_type size() const noexcept
    {
        return m_vector.size();
    }

    [[nodiscard]] constexpr inline size_type max_size() const noexcept
    {
        return m_vector.max_size();
    }

    [[nodiscard]] constexpr inline size_type capacity() const noexcept
    {
        return m_vector.capacity();
    }

    constexpr void clear() noexcept
    {
        m_vector.clear(), m_offset = 0;
    }

    constexpr void invalidate() noexcept
    {
        m_vector.invalidate(), m_offset = 0;
    }

    [[nodiscard]] constexpr T* release() noexcept
    {
        m_vector.release(), m_offset = 0;
    }

    constexpr inline AllocatorHelper& getAllocatorHelper() noexcept
    {
        return m_vector.getAllocatorHelper();
    }

    constexpr inline AllocatorHelper& getAllocatorHelper() const noexcept
    {
        return m_vector.getAllocatorHelper();
    }

    constexpr inline Vector<T, AllocatorHelper>& getVector() noexcept
    {
        return m_vector;
    }

    constexpr inline const Vector<T, AllocatorHelper>& getVector() const noexcept
    {
        return m_vector;
    }

    constexpr inline size_type getOffset() const noexcept
    {
        return m_offset;
    }

    constexpr inline void setOffset(size_type offset) const noexcept
    {
        m_offset = offset;
    }

    constexpr inline size_type write(const T* p, size_type n)
    {
        return insert(m_offset, p, n);
    }

    template<typename It_src>
    constexpr inline size_type write(It_src srcBegin, It_src srcEnd)
    {
        return insert(m_offset, srcBegin, srcEnd);
    }

    constexpr inline size_type read(T* p, size_type n)
    {
        return 0;// insert(m_offset, p, n);
    }

private:
    Vector<T, AllocatorHelper> m_vector;
    size_typeype m_offset = 0;
};
