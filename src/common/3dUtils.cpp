#include <3dUtils.hpp>
#include <componentSystem.hpp> //Access to hashmap

constexpr inline float tValueOffset = 1.0e-6;

void updateVariable(float& currentT, float tSlope, float& currentPosition) {
    const float currentPositionRounded = tSlope < 0 ? 0.5 + glm::floor(currentPosition - 0.5) : 0.5 + glm::ceil(currentPosition - 0.5);
    currentT += (currentPositionRounded - currentPosition) * tSlope;
    currentPosition = currentPositionRounded;
}

glm::ivec3 raycastCamera(glm::vec3& cameraPosition, glm::vec3& cameraAngle, bool backtrace) {
    float tValues[(int) (3 * (blockInteractRange + 1))];
    uint32_t numTValues = 0;
    glm::vec3 position = cameraPosition;

    float tOverX, tOverY, tOverZ;
    float xNextT, yNextT, zNextT;

    //Init all variables
    if (almostZero(cameraAngle.x)) {
        xNextT = blockInteractRange;
    }
    else {
        tOverX = 1 / cameraAngle.x;
        xNextT = 0.0f;
        updateVariable(xNextT, tOverX, cameraPosition.x);
    }
    if (almostZero(cameraAngle.y)) {
        yNextT = blockInteractRange;
    }
    else {
        tOverY = 1 / cameraAngle.y;
        yNextT = 0.0f;
        updateVariable(yNextT, tOverY, cameraPosition.y);
    }
    if (almostZero(cameraAngle.z)) {
        zNextT = blockInteractRange;
    }
    else {
        tOverZ = 1 / cameraAngle.z;
        zNextT = 0.0f;
        updateVariable(zNextT, tOverZ, cameraPosition.z);
    }
    float t = std::min(xNextT, std::min(yNextT, zNextT));

    //Raymarch with ordered T
    //Each iteration
    while (t <= blockInteractRange) {
        tValues[numTValues++] = t;

        if (xNextT < yNextT && xNextT < zNextT) {
            t = xNextT;
            updateVariable(xNextT, tOverX, position.x);
        }

        else if (yNextT < zNextT) { // we already know xNextT is larger than yNextT
            t = yNextT;
            updateVariable(yNextT, tOverY, position.y);
        }
        else {
            t = zNextT;
            updateVariable(zNextT, tOverZ, position.z);
        }
    }

    //Now we have an ordered t-array of integer intersections, for 0 <= t <= blockInteractRange
    for (float t : tValues) {
        //The block starts right after the intersection
        glm::ivec3 candidateBlock = glm::round(cameraPosition + cameraAngle * (t + tValueOffset));
        if (componentExists(candidateBlock)) {
            //If we round to right before the interestion
            if (backtrace)
                return glm::round(cameraPosition + cameraAngle * (t - tValueOffset));
            else
                return glm::round(cameraPosition + cameraAngle * (t + tValueOffset));
        }
    }

    //If we never found a block, we simply return the block at the end of the range
    return glm::round(cameraPosition + cameraAngle * blockInteractRange);
}
