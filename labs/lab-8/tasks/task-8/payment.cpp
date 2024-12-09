#include <iostream>
#include <memory>

struct Receiver {
    bool bank_transfer;
    bool money_transfer;
    bool paypal_transfer;
};

class PaymentHandler {
   public:
    virtual void Handle(const Receiver& reciever) = 0;

    const std::shared_ptr<PaymentHandler>& GetSuccessor() const {
        return successor_;
    }

    void SetSuccessor(std::shared_ptr<PaymentHandler> successor) {
        successor_ = std::move(successor);
    }

   protected:
    std::shared_ptr<PaymentHandler> successor_ = nullptr;
};

class BankPaymentHandler : public PaymentHandler {
   public:
    void Handle(const Receiver& receiver) override {
        if (receiver.bank_transfer) {
            std::cout << "Выполняем банковский перевод" << std::endl;
        } else if (successor_) {
            successor_->Handle(receiver);
        }
    }
};

class MoneyPaymentHandler : public PaymentHandler {
   public:
    void Handle(const Receiver& receiver) override {
        if (receiver.money_transfer) {
            std::cout << "Выполняем перевод через систему денежных переводов"
                      << std::endl;
        } else if (successor_) {
            successor_->Handle(receiver);
        }
    }
};

class PaypalPaymentHandler : public PaymentHandler {
   public:
    void Handle(const Receiver& receiver) override {
        if (receiver.money_transfer) {
            std::cout << "Выполняем перевод через PayPal" << std::endl;
        } else if (successor_) {
            successor_->Handle(receiver);
        }
    }
};

int main() {
    Receiver receiver{false, true, true};
    auto bank = std::make_shared<BankPaymentHandler>();
    auto money = std::make_shared<MoneyPaymentHandler>();
    auto paypal = std::make_shared<PaypalPaymentHandler>();
    bank->SetSuccessor(paypal);
    paypal->SetSuccessor(money);
    bank->Handle(receiver);
}
