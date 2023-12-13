#include <tuple>
#include <ostream>

#include <mpack/mpack.h>

struct Vector3
{
    Vector3() = default;
    Vector3(mpack_node_t array);
    double x{0.0};
    double y{0.0};
    double z{0.0};
};

std::ostream& operator<<(std::ostream& os, const Vector3& vec);

struct Vector2
{
    Vector2() = default;
    Vector2(mpack_node_t array);
    double x{0.0};
    double y{0.0};
};

std::ostream& operator<<(std::ostream& os, const Vector2& vec);

struct GazeBinocular
{
    GazeBinocular() = default;
    GazeBinocular(mpack_node_t map);
    double confidence;
    Vector2 norm_pos;
    double timestamp;
    std::pair<Vector3, Vector3> gaze_normals;
    std::pair<Vector3, Vector3> eye_centers;
    Vector3 gaze_point;
};

std::ostream& operator<<(std::ostream& os, const GazeBinocular& gaze);