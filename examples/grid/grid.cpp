#include "engine/Font.h"
#include <Echo2D.h>
#include <vector>

class MyApp : public Engine::Application {
public:
   MyApp () : Engine::Application(800, 600, "HelloThere") {}
   ~MyApp(){ 
      for (Engine::Texture* tex : Textures) {
         delete tex;
      }
      delete font;
   }


   glm::vec2 position = {0.0f, 0.0f};
   glm::vec2 position1 = {400.0f, 500.0f};
   glm::vec2 dimensions = {30.0f, 30.0f};
   glm::vec4 color1 = {255.0f, 255.0f, 255.0f, 255.0f};
   glm::vec4 color2 = {255.0f, 255.0f, 0.0f, 255.0f};
   glm::vec4 background = {144.0f, 233.0f, 200.0f, 255.0f};

   std::vector<Engine::Texture*> Textures;

   Engine::Font *font = nullptr;

   void Init() override {
      Textures.push_back(new Engine::Texture("assets/dvd.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_292.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_293.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_294.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_295.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_296.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_297.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_298.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_299.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_300.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_301.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_302.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_303.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_304.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_305.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_306.png"));
      Textures.push_back(new Engine::Texture("assets/sprite_307.png"));
      font = new Engine::Font("assets/myFont.ttf", 48);

      Debug();
      SetFPS(500);
   }

   void Update(float dt) override {
   }

   void Render() const override {
      Engine::Renderer::ClearScreenColor(background);

      for (int i = 0; i < 10; i++) {
         for (int j = 0; j < 17; j++) {
            float x = (position.x + dimensions.x + 10.0f) * j;
            float y = (position.y + dimensions.y + 10.0f) * i;
            Engine::Renderer::DrawRectTexture(dimensions, (glm::vec2){x, y}, color1, *Textures.at(j));
         }
      }
      Engine::Renderer::DrawText("(C) LearnOpenGL.com", position1, *font, color2, 0.5f);
   }

};

int main (void) {
   MyApp *app = new MyApp();
   app->Run();
   delete app;
}
