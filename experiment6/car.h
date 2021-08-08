#pragma once
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <queue>

// ���������ƶ��ļ������ܵ�ѡ��
enum Direction {
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_LEFT,
    CAR_RIGHT
};

class Car {
public:
    glm::vec3 Position;
    glm::vec3 Front;
    float Yaw;

    // �洢��Yaw��Ϣ��ʵ��Ư��
    std::queue<float> HistoryYaw;
    int DelayFrameNum = 20;
    float DelayYaw;

    // ʵ�����������뻺ͣ
    // TODO ����һ��speed���ԣ����հ��������deltatime�������ӻ��Сspeed����ֵ��ʵ�ֻ������Ӻ��ʣ�������ʹ�����ڵ���ֵķ������������Yawƫ��Ҳ��ͬ��
    std::queue<glm::vec3> HistoryPosition;
    glm::vec3 DelayPosition;

    float MovementSpeed;
    float TurningSpeed;

    Car(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f))
        : MovementSpeed(0.0f)
        , TurningSpeed(90.0f)
        , Yaw(0.0f)
        , DelayYaw(0.0f)
    {
        Position = position;
        updateFront();
    }

    glm::vec3 getPosition()
    {
        return Position;
    }

    float getYaw()
    {
        return Yaw;
    }

    float getDelayYaw()
    {
        return DelayYaw;
    }

    float getYawDif()
    {
        return Yaw - DelayYaw;
    }

    float getMidValYaw()
    {
        return (DelayYaw + Yaw) / 2;
    }

    glm::vec3 getMidValPosition()
    {
        return (DelayPosition + Position) / 2.0f;
    }

    // ������ͼ����
    glm::mat4 GetViewMatrix(glm::vec3 cameraPosition)
    {
        return glm::lookAt(cameraPosition, Position, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // ���ܼ�������
    void ProcessKeyboard(Direction direction, float deltaTime)
    {
        if (direction == CAR_FORWARD) {
            if (MovementSpeed < 30.0f)MovementSpeed += 0.5f;
            Position += Front * MovementSpeed * deltaTime;
        }

        if (direction == CAR_BACKWARD) {
            if (MovementSpeed > -10.0f)MovementSpeed -= 0.25f;
            Position += Front * MovementSpeed * deltaTime;
        }
        if (direction == CAR_LEFT)
            Yaw += TurningSpeed * deltaTime;
        if (direction == CAR_RIGHT)
            Yaw -= TurningSpeed * deltaTime;
        updateFront();
    }

    int Inertia(float deltaTime) {
        int result;
        if (MovementSpeed > 0.0f) {
            MovementSpeed -= 2.0f;
            result = 1;
        }
        else if (MovementSpeed < 0.0f) {
            MovementSpeed += 2.0f;
            result = 2;
        }
        else return 0;
        Position += Front * MovementSpeed * deltaTime;
        return result;
    }

    // ����DalayYaw
    void UpdateDelayYaw()
    {
        HistoryYaw.push(Yaw);
        while (HistoryYaw.size() > DelayFrameNum) {
            HistoryYaw.pop();
        }
        DelayYaw = HistoryYaw.front();
    }

    // ����DalayYaw
    void UpdateDelayPosition()
    {
        HistoryPosition.push(Position);
        while (HistoryPosition.size() > DelayFrameNum) {
            HistoryPosition.pop();
        }
        DelayPosition = HistoryPosition.front();
    }

private:
    // �����µ� Front ����
    void updateFront()
    {
        glm::vec3 front;
        front.x = -sin(glm::radians(getMidValYaw()));
        front.y = 0.0f;
        front.z = -cos(glm::radians(getMidValYaw()));
        Front = glm::normalize(front);
    }
};