#include <functional>
#include <iostream>

class Drive {
   public:
    using TwistHandler = std::function<void(Drive&)>;

    Drive(TwistHandler twist_handler)
        : twist_handler_(std::move(twist_handler)),
          twist_("Исходная позиция") {}

    const std::string& GetTwist() const {
        return twist_;
    }

    void SetTwist(std::string twist) {
        twist_ = std::move(twist);
        twist_handler_(*this);
    }

    void TurnLeft() {
        SetTwist("Поворот налево");
    }

    void TurnRight() {
        SetTwist("Поворот направо");
    }

    void Stop() {
        SetTwist("Стоп");
    }

    std::string ToString() const {
        return "Привод: " + twist_;
    }

   private:
    TwistHandler twist_handler_;
    std::string twist_;
};

class Power {
   public:
    using PowerHandler = std::function<void(Power&)>;

    Power(PowerHandler power_handler)
        : power_handler_(std::move(power_handler)) {}

    int GetPower() const {
        return power_;
    }

    void SetPower(int power) {
        power_ = power;
        power_handler_(*this);
    }

    std::string ToString() const {
        return "Задана мощность " + std::to_string(power_) + "w";
    }

   private:
    PowerHandler power_handler_;
    int power_ = 0;
};

class Notification {
   public:
    using MessageHandler = std::function<void(Notification&)>;

    Notification(MessageHandler message_handler)
        : message_handler_(std::move(message_handler)) {}

    const std::string& GetMessage() const {
        return message_;
    }

    void StartNotification() {
        SetMessage("Операция началась");
    }

    void StopNotification() {
        SetMessage("Операция завершена");
    }

    std::string ToString() const {
        return "Информация: " + message_;
    }

   private:
    void SetMessage(std::string message) {
        message_ = std::move(message);
        message_handler_(*this);
    }

    MessageHandler message_handler_;
    std::string message_;
};

class Microvawe {
   public:
    Microvawe(Drive drive, Power power, Notification notification)
        : drive_(std::move(drive)),
          power_(std::move(power)),
          notification_(std::move(notification)) {}

    void Defrost() {
        notification_.StartNotification();
        power_.SetPower(1000);
        drive_.TurnRight();
        drive_.TurnRight();
        power_.SetPower(500);
        drive_.Stop();
        drive_.TurnLeft();
        drive_.TurnLeft();
        power_.SetPower(200);
        drive_.Stop();
        power_.SetPower(0);
        notification_.StopNotification();
    }

    void Cook() {
        notification_.StartNotification();
        power_.SetPower(1000);
        drive_.TurnRight();
        drive_.TurnRight();
        drive_.TurnRight();
        drive_.TurnRight();
        drive_.TurnLeft();
        drive_.TurnLeft();
        drive_.TurnLeft();
        drive_.TurnLeft();
        drive_.Stop();
        notification_.StopNotification();
    }

   private:
    Drive drive_;
    Power power_;
    Notification notification_;
};

int main() {
    Drive drive{
        [](Drive& drive) { std::cout << drive.ToString() << std::endl; }};
    Power power{
        [](Power& power) { std::cout << power.ToString() << std::endl; }};
    Notification notification{[](Notification& notification) {
        std::cout << notification.ToString() << std::endl;
    }};
    Microvawe microvawe{drive, power, notification};

    std::cout << "Разморозка" << std::endl;
    microvawe.Defrost();

    std::cout << std::endl;

    std::cout << "Приготовление" << std::endl;
    microvawe.Cook();
}
