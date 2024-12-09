#include <iostream>
#include <random>

class IGame {
   public:
    virtual int Roll() = 0;
};

class Dice : public IGame {
   public:
    Dice() : generator_(std::random_device{}()), distribution_(1, 6) {}

    int Roll() override {
        return distribution_(generator_);
    }

   private:
    std::mt19937 generator_;
    std::uniform_int_distribution<int> distribution_;
};

class Coin {
   public:
    Coin() : generator_(std::random_device{}()), distribution_(1, 2) {}

    int Flip() {
        return distribution_(generator_);
    }

   private:
    std::mt19937 generator_;
    std::uniform_int_distribution<int> distribution_;
};

class AdapterGame : public IGame {
   public:
    AdapterGame(Coin coin) : coin_(std::move(coin)) {}

    int Roll() override {
        return coin_.Flip();
    }

   private:
    Coin coin_;
};

class Gamer {
   public:
    Gamer(std::string name) : name_(std::move(name)) {}

    const std::string& GetName() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

    int GameSession(IGame& game) {
        return game.Roll();
    }

   private:
    std::string name_;
};

int main() {
    Dice dice;
    Gamer g1{"Иван"};
    std::cout << "Выпало очков " << g1.GameSession(dice) << " для игрока "
              << g1.GetName() << std::endl;

    Coin coin;
    AdapterGame adapter{coin};
    std::cout << "Монета показала " << g1.GameSession(adapter) << " для игрока "
              << g1.GetName() << std::endl;
}
