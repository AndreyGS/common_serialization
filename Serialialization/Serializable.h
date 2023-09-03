#include "Containers/Walker.h"

namespace common_serialization
{

template<typename T>
class Serializable
{
public:
    int serialize(RawData& rawData) const noexcept;
    int deserialize(const RawData& rawData);
};


} // namespace common_serialization