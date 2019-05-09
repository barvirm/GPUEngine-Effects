#pragma once
#include <Effects/Effects_export.h>

namespace msg {
    class EFFECTS_EXPORT VisualizationTechnique {
    public:
        virtual ~VisualizationTechnique() = default;

        virtual void update() = 0;
        virtual void draw() = 0;
    };
}

