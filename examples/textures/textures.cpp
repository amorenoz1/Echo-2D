#include <Echo2D.h>
#include <cstdlib>
#include <random>

class MyApp : public Engine::Application {
public:
   MyApp () : Engine::Application(800, 600, "HelloThere") {}
   ~MyApp(){ delete dvdIcon; }


   glm::vec2 velocity = {50.0f, 50.0f};
   glm::vec2 position = {400.0f, 300.0f};
   glm::vec2 dimensions = {100.0f, 100.0f};
   glm::vec4 color1 = {198.0f, 84.0f, 13.0f, 255.0f};
   glm::vec4 background = {144.0f, 233.0f, 200.0f, 255.0f};

   Engine::Texture *dvdIcon = nullptr;

   void Init() override {
      dvdIcon = new Engine::Texture("dvd.png");
   }

   void selectRandomColor() {
      color1.r = (float)(std::rand() % 256);
      color1.g = (float)(std::rand() % 256);
      color1.b = (float)(std::rand() % 256);
   }
   
   void Update(float dt) override {
      position.x += velocity.x * dt;
      position.y += velocity.y * dt;

      if (position.x + dimensions.x >= 800.0f) {
         position.x = 800.0f - dimensions.x;
         velocity.x *= -1;
         selectRandomColor();
      }
      
      if (position.y <= 0.0f) {
         position.y = 0.0f;
         velocity.y *= -1;
         selectRandomColor();
      }

      if (position.x <= 0.0f) {
         position.x = 0.0f;
         velocity.x *= -1;
         selectRandomColor();
      }

      if (position.y + dimensions.y >= 600.0f) {
         position.y = 600.0f - dimensions.y;
         velocity.y *= -1;
         selectRandomColor();
      }
   }

   void Render() const override {
      Engine::Renderer::ClearScreenColor(background);
      Engine::Renderer::DrawRectTexture(dimensions, position, color1, *dvdIcon);
   }

};



int main (void) {
   std::srand(static_cast<unsigned int>(std::time(0)));
   MyApp *app = new MyApp();
   app->Run();
   delete app;
}


