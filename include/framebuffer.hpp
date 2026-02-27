#pragma once

#include <iostream>

namespace Renderer {
    /**
     * Класс FrameBuffer инкапсулирует объект framebuffer OpenGL.
     * Позволяет рендерить сцену в текстуру, которая затем может быть использована
     * для пост-обработки или отображения в интерфейсе.
     */
    class FrameBuffer {
        public:
            /**
             * Конструктор. Создаёт framebuffer с заданными размерами, включая
             * текстуру цвета и renderbuffer для глубины и трафарета.
             * @param width  Ширина framebuffer'а в пикселях.
             * @param height Высота framebuffer'а в пикселях.
             */
            FrameBuffer(float width, float height);

            /**
             * Деструктор. Освобождает ресурсы OpenGL (fbo, текстура, rbo).
             */
            ~FrameBuffer();

            /**
             * Возвращает идентификатор текстуры, в которую выполняется рендеринг.
             * @return OpenGL-идентификатор текстуры (GLuint).
             */
            unsigned int getFrameTexture();

            /**
             * Изменяет размер framebuffer'а. Пересоздаёт хранилища текстуры и renderbuffer.
             * @param width  Новая ширина.
             * @param height Новая высота.
             */
            void RescaleFrameBuffer(float width, float height);

            /**
             * Возвращает соотношение сторон (ширина / высота).
             * @return Значение aspect ratio.
             */
            float GetAspectRatio() const;

            /**
             * Привязывает framebuffer для рендеринга.
             * Все последующие операции рендеринга будут направлены в этот буфер.
             */
            void Bind() const;

            /**
             * Отвязывает текущий framebuffer, возвращая рендеринг в основной буфер (экран).
             */
            void Unbind() const;

        private:
            unsigned int fbo;      ///< Идентификатор объекта framebuffer.
            unsigned int texture;  ///< Идентификатор текстуры цвета.
            unsigned int rbo;      ///< Идентификатор renderbuffer для глубины и трафарета.

            float width;           ///< Текущая ширина framebuffer'а.
            float height;          ///< Текущая высота framebuffer'а.
    };
}
