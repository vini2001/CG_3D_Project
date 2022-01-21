#include "GVertice.hpp"

GVertice::GVertice(v3 coords, v3 colors, v2 texture, v3 normal) {
    this->coords = coords;
    this->colors = colors;
    this->texture = texture;
    this->normal = normal;
}