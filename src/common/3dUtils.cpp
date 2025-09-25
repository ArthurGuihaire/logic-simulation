#include <3dUtils.hpp>
#include <componentSystem.hpp> //Access to hashmap

constexpr inline float tValueOffset = 1.0e-6;

void evaulateDimension(float (&tValues)[(int) (3 * (blockInteractRange + 1))], uint32_t& numTValues, float cameraPositionDim, float cameraAngleDim) {
    if (almostEqual(cameraAngleDim, 0.0f)) return;
    float start = cameraPositionDim;
    float end = cameraPositionDim + blockInteractRange * cameraAngleDim;

    if (end < start) std::swap(start, end);

    const float startVal = glm::floor(start-0.5) + 0.5;
    const float endVal = glm::ceil(end-0.5) + 0.5;

    for (float k = startVal; k <= endVal; k++) {
        tValues[numTValues++] = (k - start) / cameraAngleDim;
    }
}

glm::ivec3 raycastCamera(glm::vec3 &cameraPosition, glm::vec3 &cameraAngle) {
    float tValues[(int) (3 * (blockInteractRange + 1))];
    uint32_t numTValues = 0;

    evaulateDimension(tValues, numTValues, cameraPosition.x, cameraAngle.x);
    evaulateDimension(tValues, numTValues, cameraPosition.y, cameraAngle.y);
    evaulateDimension(tValues, numTValues, cameraPosition.z, cameraAngle.z);

    //Go from near to far
    for (uint32_t i = 0; i < numTValues; i++) {
        glm::ivec3 position = glm::round(cameraPosition + cameraAngle * (tValues[i] + tValueOffset));
        if (componentExists(position)) {
            return glm::round(cameraPosition + cameraAngle * (tValues[i] - tValueOffset));
        }
    }

    return glm::round(cameraPosition + blockInteractRange * cameraAngle);
}

/*
TO REWRITE RAYMARCHER FOR ORDERED T VALUES
start at cameraPosition, each iteration find the minimum distance to reach a voxel boundary based on cameraAngle, and keep iterating through until t is greater than blockInteractRange
*/
