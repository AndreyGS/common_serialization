#include "Containers/Vector.h"

namespace common_serialization
{

template<typename T>
class Serializable
{
public:
    int serialize(RawArray& rawArray) const noexcept;
    int deserialize(const RawArray& rawArray);
};


} // namespace common_serialization