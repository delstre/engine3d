#pragma once

#include <math.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <cfloat>


namespace Engine {
    class Object;
    class Scene;

    class Camera {
        public:
            Camera() = delete;

            /**
             * Конструктор камеры.
             * @param scene Указатель на сцену, к которой принадлежит камера.
             * Сохраняет указатель и инициализирует матрицы проекции и вида.
             */
            Camera(Scene* scene);

            int wid, hei;
            glm::vec4 planes[6];
            glm::vec3 position;
            glm::vec3 front;
            glm::vec3 up;
            glm::vec3 right;
            glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
            glm::mat4 projection;
            glm::mat4 view;
            glm::mat4 mvp;
            float zoom = 45.0f;
            float yaw = 0.0f;
            float pitch = 0.0f;;
            float speed = 2.5f;
            float sensitivity = 0.1f;
            bool constrainPitch = true;

            /**
             * Обновляет матрицы проекции и вида на основе текущих параметров камеры
             * (позиция, направление, угол обзора) и соотношения сторон сцены.
             * Вычисляет результирующую матрицу MVP (projection * view).
             */
            void UpdateMVP();

            /**
             * Преобразует экранные координаты мыши в мировое направление луча,
             * исходящего из камеры.
             * @param mouseX Координата мыши по оси X (в пикселях).
             * @param mouseY Координата мыши по оси Y (в пикселях).
             * @param width  Ширина окна просмотра (в пикселях).
             * @param height Высота окна просмотра (в пикселях).
             * @return Нормализованный вектор направления луча в мировом пространстве.
             */
            glm::vec3 GetCursorDirection(double mouseX, double mouseY, double width, double height);

            /**
             * Выполняет обновление состояния камеры: пересчитывает матрицы и векторы.
             * Должен вызываться каждый кадр перед рендерингом.
             */
            void Think();

            /**
             * Обновляет векторы направления (front, right, up) на основе текущих
             * углов yaw и pitch. Используется после изменения углов.
             */
            void updateCameraVectors();
            
            /**
             * Обрабатывает движение мыши для изменения ориентации камеры.
             * @param xoffset Смещение мыши по горизонтали.
             * @param yoffset Смещение мыши по вертикали.
             * Изменяет углы yaw и pitch с учётом чувствительности (sensitivity).
             * При включённом ограничении (constrainPitch) pitch ограничивается диапазоном [-89, 89].
             */
            void ProcessMouseMovement(float xoffset, float yoffset);
            
            /**
             * Перемещает камеру в заданном направлении.
             * @param direction Код клавиши (GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D),
             *                  определяющий направление движения.
             * @param deltaTime Время, прошедшее с предыдущего кадра (в секундах).
             *                  Используется для нормирования скорости.
             */
            void ProcessKeyboardInput(int direction, float deltaTime);

            /**
             * Опрашивает состояние клавиш WASD и вызывает ProcessKeyboardInput
             * для каждой нажатой клавиши. Должен вызываться каждый кадр.
             * @param window Указатель на окно GLFW.
             * @param deltaTime Время, прошедшее с предыдущего кадра.
             */
            void Control(GLFWwindow* window, float deltaTime);
        private:
            Scene* scene;
    };
}

