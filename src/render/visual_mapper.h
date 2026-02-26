#ifndef VISUAL_MAPPER_H
#define VISUAL_MAPPER_H

#include <string>
#include <vector>

namespace render {

class VisualMapper {
public:
    static std::string getColorForWeight(int weight);
    static int getSizeForDegree(int degree);
};

} // namespace render

#endif // VISUAL_MAPPER_H
