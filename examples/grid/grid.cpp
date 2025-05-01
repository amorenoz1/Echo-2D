#include "engine/Camera.h"
#include "engine/Font.h"
#include "engine/InputHandler.h"
#include "engine/Renderer.h"
#include <Echo2D.h>
#include <vector>

class MyApp : public Echo2D::Application {
public:
   MyApp () : Echo2D::Application(800, 600, "HelloThere") {}
   ~MyApp(){ 
      for (Echo2D::Texture* tex : Textures) {
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

   std::vector<Echo2D::Texture*> Textures;
   Echo2D::Camera2D *my_camera = nullptr;
   Echo2D::Font *font = nullptr;
   float camera_speed = 20.0f;



   void Init() override {
      Textures.push_back(new Echo2D::Texture("assets/dvd.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_292.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_293.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_294.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_295.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_296.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_297.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_298.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_299.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_300.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_301.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_302.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_303.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_304.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_305.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_306.png"));
      Textures.push_back(new Echo2D::Texture("assets/sprite_307.png"));
      font = new Echo2D::Font("assets/myFont.ttf", 48);
      my_camera = new Echo2D::Camera2D(800, 600);
      
      Echo2D::Renderer::AddCamera2D(*my_camera);

      Debug();
      SetFPS(500);
   }

   void Update(float dt) override {
      const float move_amount = camera_speed * dt;

      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_W)) {
         my_camera->Move({0.0f, -move_amount}); // Down
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_S)) {
         my_camera->Move({0.0f, +move_amount}); // Up
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_A)) {
         my_camera->Move({-move_amount, 0.0f}); // Left
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_D)) {
         my_camera->Move({+move_amount, 0.0f}); // Right
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_O)) {
         my_camera->Zoom(1.025f); // Zoom in
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_P)) {
         my_camera->Zoom(0.975f); // Zoom out
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_Q)) {
         my_camera->Rotate(+1.0f); // Rotate CCW
      }
      if (Echo2D::KeyListener::isKeyPressed(GLFW_KEY_E)) {
         my_camera->Rotate(-1.0f); // Rotate CW
      }
   }

   void Render() const override {
      Echo2D::Renderer::ClearScreenColor(background);

      for (int i = 0; i < 10; i++) {
         for (int j = 0; j < 17; j++) {
            float x = (position.x + dimensions.x + 10.0f) * j;
            float y = (position.y + dimensions.y + 10.0f) * i;
            Echo2D::Renderer::DrawRectTexture(dimensions, (glm::vec2){x, y}, color1, *Textures.at(j));
         }
      }
      Echo2D::Renderer::DrawText("(C) LearnOpenGL.com", position1, *font, color2, 0.5f);
   }

};

int main (void) {
   MyApp *app = new MyApp();
   app->Run();
   delete app;
}
