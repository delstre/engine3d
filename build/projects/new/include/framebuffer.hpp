#pragma once

#include <iostream>

namespace Renderer {
    class FrameBuffer {
        public:
            FrameBuffer(float width, float height);
            ~FrameBuffer();
            unsigned int getFrameTexture();
            void RescaleFrameBuffer(float width, float height);
            float GetAspectRatio() const;
            void Bind() const;
            void Unbind() const;
        private:
            unsigned int fbo;
            unsigned int texture;
            unsigned int rbo;

            float width;
            float height;
    };
}


