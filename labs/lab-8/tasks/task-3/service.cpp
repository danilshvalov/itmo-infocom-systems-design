#include <iostream>

class TransportService {
   public:
    TransportService(std::string name) : name_(std::move(name)) {}

    const std::string& GetName() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

    virtual double GetCostTransportation(double distance) const = 0;

    virtual double GetCostBusinessTransportation(double distance) const = 0;

    virtual ~TransportService() = default;

   private:
    std::string name_;
};

class TransportCompany {
   public:
    TransportCompany(std::string name) : name_(std::move(name)) {}

    const std::string& GetName() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

    virtual std::unique_ptr<TransportService> CreateService(std::string name,
                                                            int,
                                                            double) = 0;

    virtual ~TransportCompany() = default;

   private:
    std::string name_;
};

class TaxiService : public TransportService {
   public:
    TaxiService(std::string name, int category, double business_coef)
        : TransportService(std::move(name)),
          category_(category),
          business_coef_(business_coef) {}

    int GetCategory() const {
        return category_;
    }

    void SetCategory(int category) {
        category_ = category;
    }

    double GetCostTransportation(double distance) const override {
        return distance * category_;
    }

    double GetCostBusinessTransportation(double distance) const override {
        return distance * category_ * business_coef_;
    }

   private:
    int category_;
    double business_coef_;
};

class TaxiTransportCompany : public TransportCompany {
   public:
    TaxiTransportCompany(std::string name)
        : TransportCompany(std::move(name)) {}

    std::unique_ptr<TransportService> CreateService(
        std::string name,
        int category,
        double business_coef) override {
        return std::make_unique<TaxiService>(std::move(name), category,
                                             business_coef);
    }
};

class Shipping : public TransportService {
   public:
    Shipping(std::string name, int tariff, double business_coef)
        : TransportService(std::move(name)),
          tariff_(tariff),
          business_coef_(business_coef) {}

    int GetTariff() const {
        return tariff_;
    }

    void SetTariff(int category) {
        tariff_ = category;
    }

    double GetCostTransportation(double distance) const override {
        return distance * tariff_;
    }

    double GetCostBusinessTransportation(double distance) const override {
        return distance * tariff_ * business_coef_;
    }

   private:
    int tariff_;
    double business_coef_;
};

class ShippingTransportCompany : public TransportCompany {
   public:
    ShippingTransportCompany(std::string name)
        : TransportCompany(std::move(name)) {}

    std::unique_ptr<TransportService>
    CreateService(std::string name, int tariff, double business_coef) override {
        return std::make_unique<TaxiService>(std::move(name), tariff,
                                             business_coef);
    }
};

void Print(TransportService& service, double distance) {
    std::cout << "Компания: " << service.GetName()
              << ", расстояние: " << distance
              << ", стоимость: " << service.GetCostTransportation(distance)
              << ", бизнес-стоимость: "
              << service.GetCostBusinessTransportation(distance) << std::endl;
}

int main() {
    TaxiTransportCompany taxi_company{"Служба такси"};
    auto taxi_service = taxi_company.CreateService("Такси", 1, 1.5);
    Print(*taxi_service, 15.5);

    ShippingTransportCompany shipping_company{"Служба перевозок"};
    auto shipping_service =
        shipping_company.CreateService("Грузоперевозки", 2, 2.5);
    Print(*shipping_service, 150.5);
}
