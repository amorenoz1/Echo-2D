#include <Echo2D.h>
#include <cstdint>
#include <vector>
#include <cstdlib> // For rand
#include <ctime>   // For seeding rand

const float TRIANGLE_SIZE = 20.0f;
const int NUM_TRIANGLES = 3000; // Number of triangles in the stress test

struct Triangle {
   glm::vec2 position;
   glm::vec2 velocity;
   glm::vec4 color;
};

class MyApp : public Engine::Application {
public:
   MyApp() : Engine::Application(800, 600, "Stress Test"){
      std::srand(static_cast<unsigned int>(std::time(nullptr)));

      // Initialize random triangles
      for (int i = 0; i < NUM_TRIANGLES; ++i) {
         Triangle t;
         t.position = { rand() % 800, rand() % 600 };
         t.velocity = {55.0f, 55.0f}; // -1.6 to +1.6 px/frame
         t.color = { 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), 
            static_cast<unsigned char>(rand() % 256), 
            255 
         };
         triangles.push_back(t);
      }
   }

   ~MyApp() {}

   void Init() override {
      Debug();
      SetFPS(1000);
   }

   void Update(float dt) override {
      for (auto& t : triangles) {
         t.position.x += t.velocity.x * dt;
         t.position.y += t.velocity.y * dt;

         // Bounce off walls
         t.position.x += t.velocity.x * dt;
         t.position.y += t.velocity.y * dt;

         if (t.position.x >= 800.0f) {
            t.position.x = 800.0f;
            t.velocity.x *= -1;
         }

         if (t.position.y <= 0.0f) {
            t.position.y = 0.0f;
            t.velocity.y *= -1;
         }

         if (t.position.x <= 0.0f) {
            t.position.x = 0.0f;
            t.velocity.x *= -1;
         }

         if (t.position.y  >= 600.0f) {
            t.position.y = 600.0f;
            t.velocity.y *= -1;
         }
      }
   }

   void Render() const override {
      for (const auto& t : triangles) {
         drawEquilateralTriangle(t.position, TRIANGLE_SIZE, t.color);
      }
   }

private:
   std::vector<Triangle> triangles;

   void drawEquilateralTriangle(const glm::vec2& center, float sideLength, const glm::vec4& color) const {
      float h = sideLength * 0.866f;

      glm::vec2 p1 = { center.x, center.y - (2.0f / 3.0f) * h };
      glm::vec2 p2 = { center.x - sideLength / 2.0f, center.y + (1.0f / 3.0f) * h };
      glm::vec2 p3 = { center.x + sideLength / 2.0f, center.y + (1.0f / 3.0f) * h };

      Engine::Renderer::DrawTriangle(p1, p2, p3, color);
   }
};

int main() {
   MyApp* app = new MyApp();
   app->Run();
   delete app;
}
