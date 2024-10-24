#include <glm/glm.hpp>
#include <boost/serialization/serialization.hpp>

namespace boost {
namespace serialization {

// Внешняя функция для сериализации glm::vec3
template<class Archive>
void serialize(Archive& ar, glm::vec3& vec, const unsigned int version) {
    ar & vec.x;
    ar & vec.y;
    ar & vec.z;
}

}  // namespace serialization
}  // namespace boost
