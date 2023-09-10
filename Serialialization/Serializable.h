#include "Containers/Walker.h"

namespace common_serialization
{

template<typename T>
class Serializable
{
    static const uint64_t m_nameHash;
    static const uint32_t m_versionThis;
    static const uint32_t m_versionThisAndDependencies;
    static const Vector<uint64_t> m_ancestors;
    static const Vector<uint64_t> m_members;
public:
    int serialize(RawData& rawData) const noexcept { return 0; }
    int deserialize(const RawData& rawData) { return 0; }
    [[nodiscard]] static constexpr uint64_t getHash() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionThis() noexcept;
    [[nodiscard]] static constexpr uint32_t getVersionThisAndDependencies() noexcept;
};

constexpr Vector<uint64_t> getVector()
{
    Vector<uint64_t> vec;
    vec.push_back()
}

template<typename T>
constexpr Serializable<T>::uint64_t m_nameHash = 0;
template<typename T>
constexpr Serializable<T>::uint32_t m_versionThis = 0;
template<typename T>
constexpr Serializable<T>::uint32_t m_versionThisAndDependencies = 0;
template<typename T>
constexpr Serializable<T>::uint64_t[5] m_ancestors = { 0 };
template<typename T>
constexpr Serializable<T>::uint64_t[5] m_members = { 0 };

template<typename T>
[[nodiscard]] constexpr uint64_t Serializable<T>::getHash() noexcept
{
    return m_nameHash;
}

template<typename T>
[[nodiscard]] constexpr uint32_t Serializable<T>::getVersionThis() noexcept
{
    return m_versionThis;
}

template<typename T>
[[nodiscard]] constexpr uint32_t Serializable<T>::getVersionThisAndDependencies() noexcept
{
    return m_versionThisAndDependencies;
}

} // namespace common_serialization