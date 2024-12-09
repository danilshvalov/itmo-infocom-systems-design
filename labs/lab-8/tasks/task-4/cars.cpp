#include <iostream>
#include <memory>

class AbstractEngine {
   public:
    AbstractEngine(int max_speed) : max_speed_(max_speed) {}

    int GetMaxSpeed() const {
        return max_speed_;
    }

    void SetMaxSpeed(int max_speed) {
        max_speed_ = max_speed;
    }

    virtual ~AbstractEngine() = default;

   private:
    int max_speed_;
};

class FordEngine : public AbstractEngine {
   public:
    FordEngine() : AbstractEngine(220) {}
};

class AudiEngine : public AbstractEngine {
   public:
    AudiEngine() : AbstractEngine(250) {}
};

class AbstractCarBody {
   public:
    AbstractCarBody(std::string name) : name_(std::move(name)) {}

    const std::string& GetName() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

    virtual ~AbstractCarBody() = default;

   private:
    std::string name_;
};

class SedanCarBody : public AbstractCarBody {
   public:
    SedanCarBody() : AbstractCarBody("Седан") {}
};

class PickupCarBody : public AbstractCarBody {
   public:
    PickupCarBody() : AbstractCarBody("Пикап") {}
};

class AbstractCar {
   public:
    AbstractCar(std::string name) : name_(std::move(name)) {}

    const std::string& GetName() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

    virtual int GetMaxSpeed(const AbstractEngine& engine) const = 0;

    virtual const std::string& GetCarBodyType(
        const AbstractCarBody& body) const = 0;

    virtual ~AbstractCar() = default;

   private:
    std::string name_;
};

class FordCar : public AbstractCar {
   public:
    FordCar(std::string name) : AbstractCar(std::move(name)) {}

    int GetMaxSpeed(const AbstractEngine& engine) const override {
        return engine.GetMaxSpeed();
    }

    const std::string& GetCarBodyType(
        const AbstractCarBody& body) const override {
        return body.GetName();
    }
};

class AudiCar : public AbstractCar {
   public:
    AudiCar(std::string name) : AbstractCar(std::move(name)) {}

    int GetMaxSpeed(const AbstractEngine& engine) const override {
        return engine.GetMaxSpeed();
    }

    const std::string& GetCarBodyType(
        const AbstractCarBody& body) const override {
        return body.GetName();
    }
};

class CarFactory {
   public:
    virtual std::unique_ptr<AbstractCar> CreateCar() = 0;
    virtual std::unique_ptr<AbstractEngine> CreateEngine() = 0;
    virtual std::unique_ptr<AbstractCarBody> CreateCarBody() = 0;

    virtual ~CarFactory() = default;
};

class FordFactory : public CarFactory {
   public:
    std::unique_ptr<AbstractCar> CreateCar() override {
        return std::make_unique<FordCar>("Ford");
    }

    std::unique_ptr<AbstractEngine> CreateEngine() override {
        return std::make_unique<FordEngine>();
    }

    std::unique_ptr<AbstractCarBody> CreateCarBody() override {
        return std::make_unique<PickupCarBody>();
    }
};

class AudiFactory : public CarFactory {
   public:
    static AudiFactory& GetFactory() {
        return factory_;
    }

    std::unique_ptr<AbstractCar> CreateCar() override {
        return std::make_unique<AudiCar>("Audi");
    }

    std::unique_ptr<AbstractEngine> CreateEngine() override {
        return std::make_unique<AudiEngine>();
    }

    std::unique_ptr<AbstractCarBody> CreateCarBody() override {
        return std::make_unique<SedanCarBody>();
    }

   private:
    AudiFactory() = default;

    static AudiFactory factory_;
};

AudiFactory AudiFactory::factory_ = AudiFactory();

class Client {
   public:
    Client(CarFactory& factory) {
        car_ = factory.CreateCar();
        engine_ = factory.CreateEngine();
        body_ = factory.CreateCarBody();
    }

    int RunMaxSpeed() const {
        return car_->GetMaxSpeed(*engine_);
    }

    const std::string& GetCarBodyType() const {
        return car_->GetCarBodyType(*body_);
    }

   private:
    std::unique_ptr<AbstractCar> car_;
    std::unique_ptr<AbstractEngine> engine_;
    std::unique_ptr<AbstractCarBody> body_;
};

int main() {
    AudiFactory& factory = AudiFactory::GetFactory();
    Client c1{factory};

    std::cout << "Максимальная скорость составляет " << c1.RunMaxSpeed()
              << " км/ч" << std::endl;
    std::cout << "Тип кузова: " << c1.GetCarBodyType() << std::endl;
}
