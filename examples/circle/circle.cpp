#include "external/imgui.h"
#include <Echo2D.h>

class MyApp : public Engine::Application {
public:
   MyApp () : Engine::Application(800, 600, "HelloThere") {}
   ~MyApp(){}

   glm::vec2 position = {400.0f, 300.0f};
   glm::vec4 color1 = {198.0f, 84.0f, 13.0f, 255.0f};
   glm::vec4 background = {144.0f, 233.0f, 200.0f, 255.0f};

   float radius = 25.0f;

   void Init() override {
      Debug();
      SetFPS(500);
   }
   
   void Update(float dt) override {

   }

   void Render() const override {
      Engine::Renderer::ClearScreenColor(background);
      Engine::Renderer::DrawCircle(radius, position, color1);
   }

   void RenderImGui() override {
      ImGui::Begin("Interseting Menu");
      ImGui::SliderFloat("Radius", &radius, 2.0f, 50.0f);
      ImGui::SliderFloat("Color R", &color1.r, 0.0f, 255.0f);
      ImGui::SliderFloat("Color G", &color1.g, 0.0f, 255.0f);
      ImGui::SliderFloat("Color B", &color1.b, 0.0f, 255.0f);
      ImGui::SliderFloat("Color A", &color1.a, 0.0f, 255.0f);
      ImGui::End();
   }

};



int main (void) {
   MyApp *app = new MyApp();
   app->Run();
   delete app;
}


