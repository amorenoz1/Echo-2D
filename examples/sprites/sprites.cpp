#include <Echo2D.h>

class Sprite : public Echo2D::Application {
public:

   Sprite() : Echo2D::Application(800, 600, "Spritesheet Test"){

   }

   ~Sprite() {

   }

   void Init() override {
      RunSheet = new Echo2D::Spritesheet("assets/Sprites/RUN.png", 96, 96);
      IdleSheet = new Echo2D::Spritesheet("assets/Sprites/IDLE.png", 96, 96);
      HurtSheet = new Echo2D::Spritesheet("assets/Sprites/HURT.png", 96, 96);
      AttackSheet = new Echo2D::Spritesheet("assets/Sprites/ATTACK.png", 96, 96);

   }

   void Update(float dt) override {
      Timer0 += dt;
      Timer1 += dt;
      Timer2 += dt;
      Timer3 += dt;

      if (Timer0 >= FrameInterval) {
         Count0 = (Count0 + 1) % 9;  // Run has 9 frames
         Timer0 = 0.0f;
      }
      if (Timer1 >= FrameInterval) {
         Count1 = (Count1 + 1) % 10; // Idle has 10 frames
         Timer1 = 0.0f;
      }
      if (Timer2 >= FrameInterval) {
         Count2 = (Count2 + 1) % 4;  // Hurt has 4 frames
         Timer2 = 0.0f;
      }
      if (Timer3 >= FrameInterval) {
         Count3 = (Count3 + 1) % 7;  // Attack has 7 frames
         Timer3 = 0.0f;
      }
   }

   void Render() const override {
      Echo2D::Renderer::ClearScreenColor(Gray);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos1, White, *RunSheet, Count0, 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos2, White, *IdleSheet, Count1, 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos3, White, *HurtSheet, Count2, 0);
      Echo2D::Renderer::DrawRectSprite(Dimensions, Pos4, White, *AttackSheet, Count3, 0);

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

   int Count0 = 0;
   int Count1 = 0;
   int Count2 = 0;
   int Count3 = 0;

   float Timer0 = 0.0f;
   float Timer1 = 0.0f;
   float Timer2 = 0.0f;
   float Timer3 = 0.0f;

   float FrameInterval = 0.05f; // 100ms per frame (10 FPS)
};

int main() {
   Sprite *app = new Sprite();
   app->Run();
   delete app;
}
