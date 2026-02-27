#pragma once

#include <component.hpp>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace Engine {
    /**
     * Компонент Transform определяет положение, поворот и масштаб объекта в трёхмерном пространстве.
     * Предоставляет матрицу модели, которая может использоваться для рендеринга.
     */
    class Transform : public Component {
        public:
            /**
             * Деструктор по умолчанию.
             */
            ~Transform() {};

            /**
             * Конструктор. Инициализирует переменные класса через Init().
             */
            Transform();

            /**
             * Возвращает матрицу модели, вычисленную на основе position и angle.
             * Матрица обновляется в методе Update().
             * @return Ссылка на внутреннюю матрицу glm::mat4.
             */
            glm::mat4& GetMatrix();

            /**
             * Возвращает позицию трансформа.
             * @return Ссылка на вектор position.
             */
            glm::vec3& GetPosition();

            /**
             * Возвращает указатель на позицию (для использования в некоторых API).
             * @return Указатель на position.
             */
            glm::vec3* GetPositionPtr();

            /**
             * Возвращает угол поворота (в градусах) по осям X, Y, Z.
             * @return Ссылка на вектор angle.
             */
            glm::vec3& GetAngle();

            /**
             * Устанавливает новую позицию трансформа.
             * @param position Новое значение позиции.
             */
            void SetPosition(glm::vec3 position);

            /**
             * Устанавливает новый угол поворота (в градусах).
             * @param angle Новые углы по осям.
             */
            void SetAngle(glm::vec3 angle);

            /**
             * Обновляет матрицу модели на основе текущих position и angle.
             * Вызывается движком каждый кадр.
             */
            void Update();

            /**
             * Метод для обновления интерфейса (например, для редактора).
             */
            void InterfaceUpdate();

            /**
             * Вызывается при активации компонента.
             */
            void Start();

            /**
             * Вызывается при деактивации компонента.
             */
            void End();

            // Макросы рефлексии
            DECLARE_CLASS_VARIABLE(glm::vec3, position, glm::vec3(0.0, 0.0, 0.0));
            DECLARE_CLASS_VARIABLE(glm::vec3, angle, glm::vec3(0.0, 0.0, 0.0));

            /// Матрица модели, обновляемая в Update().
            glm::mat4 matrix = glm::mat4(1.0f);

            // Регистрация переменных для рефлексии
            DECLARE_CLASS_VARIABLES(
                REGISTER_CLASS_VARIABLE(glm::vec3, position);
                REGISTER_CLASS_VARIABLE(glm::vec3, angle);
            )
    };
};
