#include "pupil_data_formats.h"

Vector3::Vector3(mpack_node_t array) {
    x = mpack_node_double(mpack_node_array_at(array, 0));
    y = mpack_node_double(mpack_node_array_at(array, 1));
    z = mpack_node_double(mpack_node_array_at(array, 2));
}

std::ostream &operator<<(std::ostream &os, const Vector3 &vec) {
    return os << "V3{" << vec.x << ", " << vec.y << ", " << vec.z << "}";
}

Vector2::Vector2(mpack_node_t array) {
    x = mpack_node_double(mpack_node_array_at(array, 0));
    y = mpack_node_double(mpack_node_array_at(array, 1));
}

std::ostream &operator<<(std::ostream &os, const Vector2 &vec) {
    return os << "V2{" << vec.x << ", " << vec.y << "}";
}

GazeBinocular::GazeBinocular(mpack_node_t map) {
    confidence = mpack_node_double(mpack_node_map_cstr(map, "confidence"));
    norm_pos = Vector2(mpack_node_map_cstr(map, "norm_pos"));
    timestamp = mpack_node_double(mpack_node_map_cstr(map, "timestamp"));

    gaze_normals = {
        Vector3(mpack_node_array_at(mpack_node_map_cstr(map, "gaze_normals_3d"),
                                    0)),
        Vector3(mpack_node_array_at(mpack_node_map_cstr(map, "gaze_normals_3d"),
                                    1)),
    };
    eye_centers = {
        Vector3(
            mpack_node_array_at(mpack_node_map_cstr(map, "eye_centers_3d"), 0)),
        Vector3(
            mpack_node_array_at(mpack_node_map_cstr(map, "eye_centers_3d"), 1)),
    };

    gaze_point = Vector3(mpack_node_map_cstr(map, "gaze_point_3d"));
}

std::ostream &operator<<(std::ostream &os, const GazeBinocular &gaze) {
    return os << "GB{ c:" << gaze.confidence << ", np:" << gaze.norm_pos
              << ", ts:" << gaze.timestamp << ", gn:["
              << gaze.gaze_normals.first << ", " << gaze.gaze_normals.second
              << "], ec:[" << gaze.eye_centers.first << ", "
              << gaze.eye_centers.second << "], gp:" << gaze.gaze_point << " }";
}