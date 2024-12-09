#include <iostream>

class HairCut {
   public:
    virtual ~HairCut() = default;

    void TemplateMethod() {
        InitTools();
        InitPlace();
        Progress();
        Cleanup();
    }

   private:
    virtual void Progress() = 0;

    void InitTools() {
        std::cout << "Init tools" << std::endl;
    }

    void InitPlace() {
        std::cout << "Init place" << std::endl;
    }

    void Cleanup() {
        std::cout << "Do cleanup" << std::endl;
    }
};

class ShortHaircut : public HairCut {
   private:
    void Progress() override {
        std::cout << "Make short haircut" << std::endl;
    }
};

class PermHaircut : public HairCut {
   private:
    void Progress() override {
        std::cout << "Make perm haircut" << std::endl;
    }
};

int main() {
    ShortHaircut haircut;
    haircut.TemplateMethod();
}
