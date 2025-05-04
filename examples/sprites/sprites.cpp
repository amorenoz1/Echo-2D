#include <Echo2D.h>

class Sprite : public Echo2D::Application {
public:

   Sprite() : Echo2D::Application(800, 600, "Spritesheet Test"){

   }

   ~Sprite() {

   }

   void Init() override {
      RunSheet = new Echo2D::Spritesheet("assets/Sprites/RUN.png", 96, 96, 0.05f, 9);
      IdleSheet = new Echo2D::Spritesheet("assets/Sprites/IDLE.png", 96, 96, 0.05f, 10);
      HurtSheet = new Echo2D::Spritesheet("assets/Sprites/HURT.png", 96, 96, 0.1f, 4);
      AttackSheet = new Echo2D::Spritesheet("assets/Sprites/ATTACK.png", 96, 96, 0.05f, 7);

   }

   void Update(float dt) override {
      RunSheet->SpriteUpdate(dt);
      IdleSheet->SpriteUpdate(dt);
      HurtSheet->SpriteUpdate(dt);
      AttackSheet->SpriteUpdate(dt);
   }

   void Render() const override {
      Echo2D::Renderer::ClearScreenColor(Gray);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos1, White, *RunSheet, RunSheet->GetCount(), 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos2, White, *IdleSheet, IdleSheet->GetCount(), 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos3, White, *HurtSheet, HurtSheet->GetCount(), 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos4, White, *AttackSheet, AttackSheet->GetCount(), 0);

   };

   void RenderImGui() override {

   };

private:
   Echo2D::Spritesheet *RunSheet = nullptr;
   Echo2D::Spritesheet *IdleSheet = nullptr;
   Echo2D::Spritesheet *HurtSheet = nullptr;
   Echo2D::Spritesheet *AttackSheet = nullptr;
   glm::vec2 Dimensions = {192.0f, 192.0f}; // enlarged sprites
   glm::vec2 Pos1 = {400.0f - Dimensions.x / 2.0f - 100.0f, 300.0f - Dimensions.y / 2.0f}; 
   glm::vec2 Pos2 = {400.0f - Dimensions.x / 2.0f + 100.0f, 300.0f - Dimensions.y / 2.0f}; 
   glm::vec2 Pos3 = {400.0f - Dimensions.x / 2.0f, 300.0f - Dimensions.y / 2.0f - 100.0f}; 
   glm::vec2 Pos4 = {400.0f - Dimensions.x / 2.0f, 300.0f - Dimensions.y / 2.0f + 100.0f}; 


   glm::vec4 White = {255.0f, 255.0f, 255.0f, 255.0f};
   glm::vec4 Gray = {128.0f, 128.0f, 128.0f, 255.0f};

};

int main() {
   Sprite *app = new Sprite();
   app->Run();
   delete app;
}
