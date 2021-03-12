#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <util/window_initializer.h>

#include <util/buffers.h>
#include <util/camera.h>
#include <util/shader.h>
#include <util/model.h>

using namespace glm;

namespace {
    class MyScenario : public CameraRotation::IScenario {
    public:
        MyScenario() {
            x = r * sin(M_PI * theta / 360) * cos(M_PI * phi / 720);
            y = r * sin(M_PI * theta / 360) * sin(M_PI * phi / 720);
            z = r * cos(M_PI * theta / 360);
        }
        ~MyScenario() override = default;

        glm::vec3 getStartPosition() const override {
            return glm::vec3(3 * sin(M_PI * 0 / 360) * cos(M_PI * 0 / 720),
                             3 * sin(M_PI * 0 / 360) * sin(M_PI * 0 / 720),
                             3 * cos(M_PI * 0 / 360));
        }
        glm::vec3 getNewPosition() override {
            ++theta;
            ++phi;
            x = r * sin(M_PI * theta / 360) * cos(M_PI * phi / 720);
            y = r * sin(M_PI * theta / 360) * sin(M_PI * phi / 720);
            z = r * cos(M_PI * theta / 360);

            return glm::vec3(x, y, z);
        }

    private:
        float r = 3;
        float theta = 0;
        float phi = 0;

        float x;
        float y;
        float z;
    };

    void GenerateHex(Model& hexagon) {
        float hexagon_radius = 1.f / 4.f;
        glm::mat4 rotate60;
        rotate60 = glm::rotate(rotate60, glm::radians(60.f), glm::vec3(0.f, 0.f, 1.f));
        auto current_point = glm::vec4(0.f, hexagon_radius * 1.5f, hexagon_radius, 1.f);

        for (size_t i = 0; i < 6; ++i) {
            auto next_point = current_point * rotate60;
            hexagon.AddTriangleWithColor(glm::vec3(0.f, 0.f, hexagon_radius),
                                         glm::vec3(current_point.x, current_point.y, current_point.z),
                                         glm::vec3(next_point.x, next_point.y, next_point.z),

                                         glm::vec4(166.f / 255.f, 98.f / 255.f, 10.f / 255.f, 1.f),
                                         glm::vec4(1.f / 255.f, 159.f / 255.f, 211.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f));

            hexagon.AddTriangleWithColor(glm::vec3(0.f, 0.f, -hexagon_radius),
                                         glm::vec3(current_point.x, current_point.y, -current_point.z),
                                         glm::vec3(next_point.x, next_point.y, -next_point.z),

                                         glm::vec4(166.f / 255.f, 98.f / 255.f, 10.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f),
                                         glm::vec4(1.f / 255.f, 159.f / 255.f, 211.f / 255.f, 1.f));

            hexagon.AddTriangleWithColor(glm::vec3(current_point.x, current_point.y, current_point.z),
                                         glm::vec3(next_point.x, next_point.y, next_point.z),
                                         glm::vec3(current_point.x, current_point.y, -current_point.z),

                                         glm::vec4(28.f / 255.f, 107.f / 255.f, 164.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f));
            hexagon.AddTriangleWithColor(glm::vec3(next_point.x, next_point.y, -next_point.z),
                                         glm::vec3(current_point.x, current_point.y, -current_point.z),
                                         glm::vec3(next_point.x, next_point.y, next_point.z),

                                         glm::vec4(28.f / 255.f, 107.f / 255.f, 164.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f),
                                         glm::vec4(255.f / 255.f, 157.f / 255.f, 30.f / 255.f, 1.f));

            current_point = next_point;
        }
    }
}

int main() {
    WindowInitializer init;
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint program_id = Shader::LoadShaders( "VertexShader.glsl", "FragmentShader.glsl" );

    Model hexagon;
    GenerateHex(hexagon);

    std::vector<GLfloat> global_vertex_buffer;
    std::vector<GLfloat> global_color_buffer;
    hexagon.PointsToBuffer(global_vertex_buffer);
    hexagon.ColorsToBuffer(global_color_buffer);
    Buffers::VBOBindScoped vertex_vbo(global_vertex_buffer);
    Buffers::VBOBindScoped color_vbo(global_color_buffer);

    MyScenario scenario;
    CameraRotation::Camera cam(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f),
                               glm::vec3(0, 0, 0), // Camera is at (4,3,3), in World Space
                               glm::vec3(0, 0, 0), // and looks at the origin
                               glm::vec3(0, 1, 0),  // Head is up (set to 0,-1,0 to look upside-down));
                               glm::mat4(1.0f),
                                static_cast<CameraRotation::IScenario*>(&scenario));


    GLuint matrix_id_1 = glGetUniformLocation(program_id, "MVP");

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        {
            Buffers::VAOEnableScoped scoped_vertex(0);
            Buffers::VAOEnableScoped scoped_color(1);

            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo.GetId());
            glVertexAttribPointer(
                    scoped_vertex.GetId(),     // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                        // size
                    GL_FLOAT,                 // type
                    GL_FALSE,                 // normalized?
                    0,                        // stride
                    (void *) 0                // array buffer offset
            );

            glBindBuffer(GL_ARRAY_BUFFER, color_vbo.GetId());
            glVertexAttribPointer(
                    scoped_color.GetId(),     // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                        // size
                    GL_FLOAT,                 // type
                    GL_FALSE,                 // normalized?
                    0,                        // stride
                    (void *) 0                // array buffer offset
            );

            glUseProgram(program_id);
            glUniformMatrix4fv(matrix_id_1, 1, GL_FALSE, &cam.GetMVP()[0][0]);
            glDrawArrays(GL_TRIANGLES, hexagon.GetPosPoint(),  hexagon.TriangleCount() * 3);
        }

        cam.RebuildMVP();
        glfwSwapBuffers(init.GetWindowPtr());
        glfwPollEvents();
    } while (glfwGetKey(init.GetWindowPtr(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(init.GetWindowPtr()) == 0);

    glDeleteProgram(program_id);

    return 0;
}
