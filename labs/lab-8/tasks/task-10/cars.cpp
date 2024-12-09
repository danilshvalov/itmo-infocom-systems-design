#include <iostream>
#include <memory>
#include <sstream>

class AutoBase {
   public:
    AutoBase() = default;

    AutoBase(std::string name, std::string description, double cost_base)
        : name_(std::move(name)),
          description_(std::move(description)),
          cost_base_(cost_base) {}

    virtual double GetCost() const = 0;

    const std::string& GetName() const {
        return name_;
    }

    const std::string& GetDescription() const {
        return description_;
    }

    virtual std::string ToString() const {
        std::stringstream out;
        out << "Ваш автомобиль: " << name_ << '\n';
        out << "Описание: " << description_ << '\n';
        out << "Стоимость: " << GetCost() << '\n';
        return out.str();
    }

    virtual ~AutoBase() = default;

   protected:
    std::string name_;
    std::string description_;
    double cost_base_ = 0;
};

class Renault : public AutoBase {
    using AutoBase::AutoBase;

    double GetCost() const override {
        return cost_base_ * 1.18;
    }
};

class Bmw : public AutoBase {
    using AutoBase::AutoBase;

    double GetCost() const override {
        return cost_base_ * 3.5;
    }
};

class DecoratorOptions : public AutoBase {
   public:
    DecoratorOptions(std::shared_ptr<AutoBase> auto_ptr, std::string title)
        : auto_(std::move(auto_ptr)), title_(std::move(title)) {}

    const std::shared_ptr<AutoBase>& GetAuto() const {
        return auto_;
    }

   protected:
    std::shared_ptr<AutoBase> auto_;
    std::string title_;
};

class MediaNav : public DecoratorOptions {
   public:
    MediaNav(std::shared_ptr<AutoBase> auto_ptr, std::string title)
        : DecoratorOptions(std::move(auto_ptr), std::move(title)) {
        name_ = auto_->GetName();
        description_ = auto_->GetDescription() + ". " + title_ +
                       ". Обновленная мультимедийная навигационная система";
    }

    double GetCost() const override {
        return auto_->GetCost() + 15.99;
    }
};

class SystemSecurity : public DecoratorOptions {
   public:
    SystemSecurity(std::shared_ptr<AutoBase> auto_ptr, std::string title)
        : DecoratorOptions(std::move(auto_ptr), std::move(title)) {
        name_ = auto_->GetName() + ". Повышенной безопасности";
        description_ = auto_->GetDescription() + ". " + title_ +
                       ". Передние боковые подушки безопасности, ESP - система "
                       "динамической стабилизации автомобиля";
    }

    double GetCost() const override {
        return auto_->GetCost() + 20.99;
    }
};

class AwdSystem : public DecoratorOptions {
   public:
    AwdSystem(std::shared_ptr<AutoBase> auto_ptr, std::string title)
        : DecoratorOptions(std::move(auto_ptr), std::move(title)) {
        name_ = auto_->GetName() + ". Система полного привода";
        description_ = auto_->GetDescription() + ". " + title_ +
                       ". Возможность проехать там, где останавливаются другие";
    }

    double GetCost() const override {
        return auto_->GetCost() + 55.99;
    }
};

class ExtendedWarranty : public DecoratorOptions {
   public:
    ExtendedWarranty(std::shared_ptr<AutoBase> auto_ptr, std::string title)
        : DecoratorOptions(std::move(auto_ptr), std::move(title)) {
        name_ = auto_->GetName();
        description_ = auto_->GetDescription() + ". " + title_ +
                       ". Расширенная гарантия на два года";
    }

    double GetCost() const override {
        return auto_->GetCost() + 55.99;
    }
};

int main() {
    auto reno =
        std::make_shared<Renault>("Рено", "Renault LOGAN Active", 499.0);
    std::cout << reno->ToString() << std::endl;

    auto media = std::make_shared<MediaNav>(reno, "Навигация");
    std::cout << media->ToString() << std::endl;

    auto security = std::make_shared<SystemSecurity>(media, "Безопасность");
    std::cout << security->ToString() << std::endl;
}
