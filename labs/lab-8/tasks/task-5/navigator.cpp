#include <iostream>

class Map {};

class Screen {};

class Action {
   public:
    virtual void Execute(Screen& screen, Map& map) = 0;

    virtual ~Action() = default;
};

class ShowMapAction : public Action {
   public:
    void Execute(Screen& screen, Map& map) override {
        std::cout << "Show map" << std::endl;
    }
};

class SearchAction : public Action {
   public:
    void Execute(Screen& screen, Map& map) override {
        std::cout << "Search" << std::endl;
    }
};

class FindRouteAction : public Action {
   public:
    FindRouteAction(std::string type) : type_(std::move(type)) {}

    void Execute(Screen& screen, Map& map) override {
        std::cout << "Find route for " << type_ << std::endl;
    }

   private:
    std::string type_;
};

class Context {
   public:
    Context(Screen& screen, Map& map, std::unique_ptr<Action> action)
        : screen_(screen), map_(map), action_(std::move(action)) {}

    void Execute() {
        action_->Execute(screen_, map_);
    }

   private:
    Screen& screen_;
    Map& map_;
    std::unique_ptr<Action> action_;
};

int main() {
    Screen screen;
    Map map;
    auto action = std::make_unique<FindRouteAction>("car");
    Context context{screen, map, std::move(action)};
    context.Execute();
}
