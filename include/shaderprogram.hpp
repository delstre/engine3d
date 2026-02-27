#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

namespace Renderer {
    /**
     * Класс ShaderProgram инкапсулирует OpenGL-шейдерную программу,
     * состоящую из вершинного и фрагментного шейдеров.
     * Предоставляет методы для загрузки шейдеров из файлов,
     * установки uniform-переменных и активации программы.
     */
    class ShaderProgram {
        public:
            /**
             * Конструктор. Создаёт шейдерную программу из файлов вершинного и фрагментного шейдеров.
             * @param vertexShader   Путь к файлу вершинного шейдера.
             * @param fragmentShader Путь к файлу фрагментного шейдера.
             */
            ShaderProgram(const char* vertexShader, const char* fragmentShader);
            
            /**
             * Конструктор по умолчанию. Создаёт неинициализированную программу.
             */
            ShaderProgram();

            // Запрещаем копирование (RAII для OpenGL-ресурсов)
            ShaderProgram(const ShaderProgram&) = delete;

            /**
             * Конструктор перемещения. Передаёт владение OpenGL-программой.
             */
            ShaderProgram(ShaderProgram&& shaderProgram) noexcept;

            /**
             * Деструктор. Удаляет OpenGL-программу.
             */
            ~ShaderProgram();

            // Запрещаем копирующее присваивание
            ShaderProgram& operator=(const ShaderProgram&) = delete;

            /**
             * Оператор перемещающего присваивания.
             */
            ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;

            /**
             * Читает содержимое файла и возвращает как C-строку (выделенную в куче).
             * Вызывающий ответственен за освобождение памяти через delete[].
             * @param filename Имя файла для чтения.
             * @return Указатель на буфер с содержимым файла, заканчивающимся нулём, или nullptr при ошибке.
             */
            char* readFromFile(const char* filename);

            /**
             * Устанавливает значение uniform-переменной типа mat4.
             * @param name   Имя uniform-переменной в шейдере.
             * @param matrix Матрица 4x4 (glm::mat4).
             */
            void setMatrix4(const std::string& name, const glm::mat4& matrix);

            /**
             * Устанавливает значение uniform-переменной типа vec4.
             * @param name   Имя uniform-переменной.
             * @param vector Вектор из четырёх компонентов.
             */
            void setVector4(const std::string& name, const glm::vec4& vector);

            /**
             * Устанавливает значение uniform-переменной типа vec3.
             * @param name   Имя uniform-переменной.
             * @param vector Вектор из трёх компонентов.
             */
            void setVector3(const std::string& name, const glm::vec3& vector);

            /**
             * Устанавливает текстуру для uniform-переменной типа sampler2D.
             * Привязывает текстуру к GL_TEXTURE0 и устанавливает uniform в 0.
             * @param name    Имя uniform-переменной.
             * @param texture OpenGL-идентификатор текстуры.
             */
            void setTexture(const std::string& name, const GLuint& texture);

            /**
             * Устанавливает массив текстур для uniform-массива sampler2D.
             * Каждая текстура привязывается к отдельному текстурному блоку.
             * @param name     Имя uniform-массива.
             * @param textures Вектор OpenGL-идентификаторов текстур.
             */
            void setTextures(const std::string& name, const std::vector<GLuint>& textures);

            /**
             * Устанавливает значение uniform-переменной типа uint (GLuint).
             * @param name Имя uniform-переменной.
             * @param side Значение для передачи.
             */
            void setUint(const std::string& name, const GLuint& side);

            /**
             * Проверяет, успешно ли скомпилирована и слинкована программа.
             * @return true, если программа готова к использованию.
             */
            bool isCompiled() const { return m_IsCompiled; };

            /**
             * Активирует шейдерную программу (вызов glUseProgram).
             * @return Всегда true.
             */
            bool useProgram() const;

        private:
            /**
             * Вспомогательная функция для создания и компиляции одного шейдера.
             * @param source    Исходный код шейдера.
             * @param shaderType Тип шейдера (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER и т.д.).
             * @param shaderID   Ссылка на переменную, куда будет записан ID созданного шейдера.
             * @return true, если компиляция прошла успешно.
             */
            bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);

            bool m_IsCompiled; ///< Флаг успешной компиляции и линковки.
            GLuint m_ID;       ///< OpenGL-идентификатор шейдерной программы.
    };
}
